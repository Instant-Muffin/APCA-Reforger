// Coop game mode
// Open lobby on game start

class APCA_GameModeCoopClass: PS_GameModeCoopClass
{
};


class APCA_GameModeCoop : PS_GameModeCoop
{
	// ------------------------------------------ Events ------------------------------------------
	
	protected override bool HandlePlayerKilled(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer)
	{
		m_OnHandlePlayerKilled.Invoke(playerId, playerEntity, killerEntity, killer);
		
		return super.HandlePlayerKilled(playerId, playerEntity, killerEntity, killer);
	}
	
	
	override void TryRespawn(RplId playableId, int playerId)
	{
		Print("Trying Spectator");
		
		// Go to spectator
		SwitchToInitialEntity(playerId);
		
		
		// Wait for respawn time and respawn
		int respawnTime = 0;
		PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
		if (playableId != RplId.Invalid())
		{
			PS_PlayableComponent playableComponent = playableManager.GetPlayableById(playableId);
			if (!playableComponent)
				return;
			FactionAffiliationComponent factionAffiliationComponent = playableComponent.GetFactionAffiliationComponent();
			Faction faction = factionAffiliationComponent.GetDefaultAffiliatedFaction();
			FactionKey factionKey = faction.GetFactionKey();
			PS_FactionRespawnCount factionRespawns = GetFactionRespawnCount(factionKey);
			respawnTime = factionRespawns.m_iTime;
			if (factionRespawns.m_bWaveMode)
					respawnTime = factionRespawns.m_iTime - Math.Mod(GetGame().GetWorld().GetWorldTime(), respawnTime); 
		}
		GetGame().GetCallqueue().CallLater(TryRespawnAfterSpectator, respawnTime, false, playableId, playerId);
	}
	
	void TryRespawnAfterSpectator(RplId playableId, int playerId)
	{
		Print("Trying Post Spawn");
		PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
		if (playableId != RplId.Invalid())
		{
			PS_PlayableComponent playableComponent = playableManager.GetPlayableById(playableId);
			if (!playableComponent)
				return;
			
			FactionAffiliationComponent factionAffiliationComponent = playableComponent.GetFactionAffiliationComponent();
			Faction faction = factionAffiliationComponent.GetDefaultAffiliatedFaction();
			FactionKey factionKey = faction.GetFactionKey();
			PS_FactionRespawnCount factionRespawns = GetFactionRespawnCount(factionKey);
			if (!factionRespawns || factionRespawns.m_iCount == 0)
			{
				return;
			}
			ResourceName prefabToSpawn = playableComponent.GetNextRespawn(factionRespawns.m_iCount == -1);
			if (factionRespawns.m_iCount > 0)
				factionRespawns.m_iCount--;
			if (prefabToSpawn != "")
			{
				int time = 3000;
				if (playerId > 0)
					playableComponent.OpenRespawnMenu(time);
				
				PS_RespawnData respawnData = new PS_RespawnData(playableComponent, prefabToSpawn);
				GetGame().GetCallqueue().CallLater(Respawn, time, false, playerId, respawnData);
				return;
			}
		}
		return;
	}
	
	override void Respawn(int playerId, PS_RespawnData respawnData)
	{
		Resource resource = Resource.Load(respawnData.m_sPrefabName);
		EntitySpawnParams params = new EntitySpawnParams();
		Math3D.MatrixCopy(respawnData.m_aSpawnTransform, params.Transform);
		IEntity entity = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
		SCR_AIGroup aiGroup = m_playableManager.GetPlayerGroupByPlayable(respawnData.m_iId);
		SCR_AIGroup playabelGroup = aiGroup.GetSlave();
		playabelGroup.AddAIEntityToGroup(entity);
		
		PS_PlayableComponent playableComponentNew = PS_PlayableComponent.Cast(entity.FindComponent(PS_PlayableComponent));
		playableComponentNew.SetPlayable(true);
		
		GetGame().GetCallqueue().Call(SwitchToSpawnedEntity, playerId, respawnData, entity, 4);
	}
	
	override void SwitchToSpawnedEntity(int playerId, PS_RespawnData respawnData, IEntity entity, int frameCounter)
	{
		if (frameCounter > 0) // Await four frames
		{		
			GetGame().GetCallqueue().Call(SwitchToSpawnedEntity, playerId, respawnData, entity, frameCounter - 1);
			return;
		}
		
		PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
		
		PS_PlayableComponent playableComponent = PS_PlayableComponent.Cast(entity.FindComponent(PS_PlayableComponent));
		RplId playableId = playableComponent.GetId();
		
		playableComponent.CopyState(respawnData);
		if (playerId > 0)
		{
			playableManager.SetPlayerPlayable(playerId, playableId);
			playableManager.ForceSwitch(playerId);
		}
		
		TeleportToSpawn(entity);
	}
	
	void TeleportToSpawn(IEntity entity)
	{
		PS_PlayableComponent playableComponent = PS_PlayableComponent.Cast(entity.FindComponent(PS_PlayableComponent));
		FactionAffiliationComponent factionAffiliationComponent = playableComponent.GetFactionAffiliationComponent();
		Faction faction = factionAffiliationComponent.GetDefaultAffiliatedFaction();
		SCR_SpawnPoint point = SCR_SpawnPoint.GetRandomSpawnPointForFaction(faction.GetFactionKey());
		vector destination;
		vector rot;
		vector transform[4]; 
		point.GetPositionAndRotation(destination, rot);
		entity.GetWorldTransform(transform);
		transform[3] = destination;
		
		Print("Teleport?");
		PrintFormat("transform - %1", transform);
		
		BaseGameEntity baseGameEntity = BaseGameEntity.Cast(entity);
		if (baseGameEntity && !BaseVehicle.Cast(baseGameEntity))
		{
			baseGameEntity.Teleport(transform);
		}
		else
		{
			entity.SetWorldTransform(transform);
		}
	}
};