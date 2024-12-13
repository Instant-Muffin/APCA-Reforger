//------------------------------------------------------------------------------------------------
/*
	Authority:
		Object responsible for defining respawn logic.

		This object receives callbacks from parent SCR_RespawnSystemComponent that can be used
		to either spawn the player on the authority side or just notify the remote player that
		they can process to spawn, or any combination based on derived implementations.
*/
[BaseContainerProps(category: "Respawn")]
class APCA_SpawnLogic: SCR_SpawnLogic
{
	protected SCR_BasePlayerLoadout m_playerInitialLoadout;
	protected SCR_BasePlayerLoadout m_playerLoadout;
	
	//------------------------------------------------------------------------------------------------
	override void OnPlayerSpawnResponse_S(SCR_SpawnRequestComponent requestComponent, SCR_ESpawnResult response)
	{
		#ifdef _ENABLE_RESPAWN_LOGS
		PrintFormat("%1::OnPlayerSpawnResponse_S(playerId: %2, response: %3)",
			Type().ToString(), requestComponent.GetPlayerId(), typename.EnumToString(SCR_ESpawnResult, response));
		#endif
		
		if(m_playerLoadout) 
		{
			GetPlayerLoadoutComponent_S(requestComponent.GetPlayerId()).GetLoadout() = m_playerLoadout;
		}else 
		{
			m_playerLoadout = GetPlayerLoadoutComponent_S(requestComponent.GetPlayerId()).GetLoadout();
			m_playerInitialLoadout = m_playerLoadout;
		}
		

		if (response != SCR_ESpawnResult.OK)
		{
			OnPlayerSpawnFailed_S(requestComponent.GetPlayerId());
		}
	}
	
	
	//------------------------------------------------------------------------------------------------
	override void OnPlayerKilled_S(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer)
	{
		#ifdef _ENABLE_RESPAWN_LOGS
		PrintFormat("%1::OnPlayerKilled_S(playerId: %2, playerEntity: %3, killerEntity: %4, killerId: %5)",
			Type().ToString(), playerId, playerEntity, killerEntity, killer.GetInstigatorPlayerID());
		#endif
		
		m_playerLoadout = GetPlayerLoadoutComponent_S(playerId).GetLoadout();

		OnPlayerEntityLost_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Called whenever provided player loses controlled entity, this can occur e.g.
		when a player dies or their entity is deleted.
	*/
	override protected void OnPlayerEntityLost_S(int playerId)
	{
		#ifdef _ENABLE_RESPAWN_LOGS
		PrintFormat("%1::OnPlayerEntityLost_S(playerId: %2)", Type().ToString(), playerId);
		#endif
		
		//DoSpawn(playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPlayerSpawnFailed_S(int playerId)
	{
		super.OnPlayerSpawnFailed_S(playerId);

		int delay = Math.RandomFloat(900, 1100);
		//GetGame().GetCallqueue().CallLater(DoSpawn, delay, false, playerId);
	}

	/*void DoSpawn(int playerId) 
	{
		Faction faction = GetPlayerFactionComponent_S(playerId).GetAffiliatedFaction();
		if (!faction)
		{
			OnPlayerSpawnFailed_S(playerId);
			return;
		}
		
		SCR_SpawnPoint point = SCR_SpawnPoint.GetRandomSpawnPointForFaction(faction.GetFactionKey());
		Spawn(playerId, m_playerLoadout, point);
	}

	
	void Spawn(int playerId, SCR_BasePlayerLoadout loadout, SCR_SpawnPoint point)
	{		
			
		//SCR_SpawnPointSpawnData data = new SCR_SpawnPointSpawnData(loadout.GetLoadoutResource(), point.GetRplId());
		ResourceName prefab = "{84B40583F4D1B7A3}Prefabs/Characters/Factions/INDFOR/FIA/Character_FIA_Rifleman.et";
		SCR_FreeSpawnData data = new SCR_FreeSpawnData(prefab, "0 0 0");
			
		if (!GetPlayerRespawnComponent_S(playerId).RequestSpawn(data))
		{
			// Try again later
		}
		
		Faction faction = GetPlayerFactionComponent_S(playerId).GetAffiliatedFaction();
		if (!faction)
		{
			OnPlayerSpawnFailed_S(playerId);
			Print("Faction!");
			return;
		}

		if (!loadout)
		{
			OnPlayerSpawnFailed_S(playerId);
			Print("Loadout!");
			return;
		}

		if (!point)
		{
			OnPlayerSpawnFailed_S(playerId);
			Print("Point!");
			return;
		}

		if (GetPlayerRespawnComponent_S(playerId).CanSpawn(data))
		{
			DoSpawn(playerId);
			Print("Do Spawn!");
		}
		else
			OnPlayerSpawnFailed_S(playerId);
	}*/
	

	//------------------------------------------------------------------------------------------------
	/*protected void Spawn(int playerId)
	{
		// Player is disconnecting (and disappearance of controlled entity started this feedback loop).
		// Simply ignore such requests as it would create unwanted entities.
		int indexOf = m_DisconnectingPlayers.Find(playerId);
		if (indexOf != -1)
		{
			m_DisconnectingPlayers.Remove(indexOf);
			return;
		}
		
		array<Faction> factions = {};
		GetGame().GetFactionManager().GetFactionsList(factions);		
		
		Faction targetFaction;
		if (!GetForcedFaction(targetFaction))
			targetFaction = factions.GetRandomElement();

		SCR_BasePlayerLoadout targetLoadout;
		if (!GetForcedLoadout(targetLoadout))
			targetLoadout = GetGame().GetLoadoutManager().GetRandomFactionLoadout(targetFaction);		
		

		Faction faction = GetPlayerFactionComponent_S(playerId).GetAffiliatedFaction();
		if (!faction)
		{
			OnPlayerSpawnFailed_S(playerId);
			return;
		}

		SCR_BasePlayerLoadout loadout = GetPlayerLoadoutComponent_S(playerId).GetLoadout();
		if (!loadout)
		{
			OnPlayerSpawnFailed_S(playerId);
			return;
		}

		SCR_SpawnPoint point = SCR_SpawnPoint.GetRandomSpawnPointForFaction(faction.GetFactionKey());
		if (!point)
		{
			OnPlayerSpawnFailed_S(playerId);
			return;
		}

		SCR_SpawnPointSpawnData data = new SCR_SpawnPointSpawnData(loadout.GetLoadoutResource(), point.GetRplId());
		if (GetPlayerRespawnComponent_S(playerId).CanSpawn(data))
			DoSpawn(playerId, data);
		else
			OnPlayerSpawnFailed_S(playerId);
	}*/
};
