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
		APCA_BaseComponent APCA_Data = APCA_BaseComponent.Cast(playerEntity.FindComponent(APCA_BaseComponent));
		APCA_Data.SetPlayerID(playerId);
		
		
		return super.HandlePlayerKilled(playerId, playerEntity, killerEntity, killer);
	}
	
	
	override void TryRespawn(RplId playableId, int playerId)
	{
		
		
		// Go to spectator
		SwitchToInitialEntity(playerId);
		
		// Wait for respawn time and respawn
		int respawnTime = 0;
		if (playableId != RplId.Invalid())
		{
			PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
			PS_PlayableComponent playableComponent = playableManager.GetPlayableById(playableId);
			if (!playableComponent)
				return;
			FactionAffiliationComponent factionAffiliationComponent = playableComponent.GetFactionAffiliationComponent();
			Faction faction = factionAffiliationComponent.GetDefaultAffiliatedFaction();
			FactionKey factionKey = faction.GetFactionKey();
			PS_FactionRespawnCount factionRespawns = GetFactionRespawnCount(factionKey);
			respawnTime = factionRespawns.m_iTime;
			ChimeraCharacter character = playableComponent.GetOwnerCharacter();
			if (character)
			{	
				IEntity entity = IEntity.Cast(character);
				APCA_BaseComponent APCA_Data = APCA_BaseComponent.Cast(entity.FindComponent(APCA_BaseComponent));
				APCA_Data.SetRespawning(true);
			}
	
			// Get the players current inventory
			array<IEntity> playerItems = GetInventory(playableId);
			
			// Respawn = respawn time - 60 second waves
			if (factionRespawns.m_bWaveMode)
			{
				respawnTime = respawnTime - Math.Mod(GetGame().GetWorld().GetWorldTime(), 60000);
			}
			
			GetGame().GetCallqueue().CallLater(TryRespawnAfterSpectator, respawnTime, false, playableId, playerId, playerItems);
		}
		
	}
	
	void ForceRespawn()
	{
		PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
		array<PS_PlayableComponent> allPlayers = playableManager.GetPlayablesSorted();
		for(int i = 0; i < allPlayers.Count(); i++)
		{
			ChimeraCharacter character = allPlayers[i].GetOwnerCharacter();
			IEntity entity = IEntity.Cast(character);
			if (character)
			{			
				// Check they're waiting to respawn		
				APCA_BaseComponent APCA_Data = APCA_BaseComponent.Cast(entity.FindComponent(APCA_BaseComponent));
				if(APCA_Data.GetRespawning())
				{
					// Get their current inventory
					array<IEntity> playerItems = GetInventory(allPlayers[i].GetId());
					
					// Respawn them
					TryRespawnAfterSpectator(allPlayers[i].GetId(), APCA_Data.GetPlayerID(), playerItems);
				}
			}
			
		}
	}
	
	array<IEntity> GetInventory(RplId playableId)
	{
		// Save players inventory
		array<IEntity> playerItems = {};
		PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
		
		if (playableId != RplId.Invalid())
		{
			PS_PlayableComponent playableComponent = playableManager.GetPlayableById(playableId);
			ChimeraCharacter character = playableComponent.GetOwnerCharacter();
			IEntity entity = IEntity.Cast(character);
			if (character)
			{	
				SCR_CharacterControllerComponent m_CharacterController = SCR_CharacterControllerComponent.Cast(character.GetCharacterController());
				if (m_CharacterController)
				{
					SCR_InventoryStorageManagerComponent m_InventoryManager = SCR_InventoryStorageManagerComponent.Cast(m_CharacterController.GetInventoryStorageManager());		
					if (m_InventoryManager)
					{
						
						array<IEntity> tempItems = {};
						//Get the current items from all storages
						m_InventoryManager.GetItems( tempItems );
				
						//Remove dupes (Caused by gadget slots like flashlight on vest)
						for(int i = 0; i < tempItems.Count(); i++)
						{
							
							bool isUnique = true;
							for(int j = 0; j < playerItems.Count(); j++)
							{
								if(playerItems[j] == tempItems[i])
									isUnique = false;
							}
							if(isUnique) 
								playerItems.Insert(tempItems[i]);
						}
						
						// Now add whatever it was they dropped. Not needed if unconcious players do not drop their weapons
						/*SCR_CharacterDamageManagerComponent newDamageManagerComponent = SCR_CharacterDamageManagerComponent.Cast(playerEntity.FindComponent(SCR_CharacterDamageManagerComponent));
						IEntity droppedWeapon = newDamageManagerComponent.GetDroppedItem();
						playerItems.Insert(droppedWeapon);*/
					}
				}
			}
		}
		
		return playerItems;
	}
	
	void TryRespawnAfterSpectator(RplId playableId, int playerId, array<IEntity> playerItems)
	{
		PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
		if (playableId != RplId.Invalid())
		{
			PS_PlayableComponent playableComponent = playableManager.GetPlayableById(playableId);
			if (!playableComponent)
				return;
			
			ChimeraCharacter character = playableComponent.GetOwnerCharacter();
			IEntity entity = IEntity.Cast(character);
			APCA_BaseComponent APCA_Data = APCA_BaseComponent.Cast(entity.FindComponent(APCA_BaseComponent));
			if(!APCA_Data.GetRespawning())
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
				APCA_Data.SetRespawning(false);
				GetGame().GetCallqueue().CallLater(RespawnWithItems, time, false, playerId, respawnData, playerItems);
				return;
			}
		}
		return;
	}
	
	void RespawnWithItems(int playerId, PS_RespawnData respawnData, array<IEntity> playerItems)
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
		ChimeraCharacter character = playableComponentNew.GetOwnerCharacter();
		
		// Delay the equipment swap to fix the belt getting stuck for some reason
		GetGame().GetCallqueue().CallLater(SwapInventory, 2000, false, entity, playerItems);
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
	
	protected void DeleteChildrens(IEntity entity, bool deleteRoot)
	{
		if (!entity || !entity.FindComponent(InventoryItemComponent))
			return;

		IEntity child = entity.GetChildren();
		while (child)
		{
			IEntity sibling = child.GetSibling();
			DeleteChildrens(child, true);
			child = sibling;
		}

		if (!entity.IsDeleted() && deleteRoot)
			delete entity;
	}
	
	void SwapInventory(IEntity entity, array<IEntity> playerItems)
	{
		PS_PlayableComponent playableComponent = PS_PlayableComponent.Cast(entity.FindComponent(PS_PlayableComponent));
		ChimeraCharacter character = playableComponent.GetOwnerCharacter();
		
		// Clear and readd inventory
		if (character)
		{	
			SCR_CharacterControllerComponent m_CharacterController = SCR_CharacterControllerComponent.Cast(character.GetCharacterController());
			if (m_CharacterController)
			{
				SCR_InventoryStorageManagerComponent m_InventoryManager = SCR_InventoryStorageManagerComponent.Cast(m_CharacterController.GetInventoryStorageManager());	
				
				if (m_InventoryManager)
				{
					// Clear current items
					array<IEntity> currentItems = {};
					array<BaseInventoryStorageComponent> storages = {};
					
					m_InventoryManager.GetItems( currentItems );
					
					m_InventoryManager.GetStorages( storages );
					foreach(BaseInventoryStorageComponent storage : storages)
					{
						int numSlots = storage.GetSlotsCount();
						array<InventoryItemComponent> slotItems = {};
						storage.GetOwnedItems(slotItems);
						
						for(int i = 0; i < numSlots; i++)
						{
							// This gets items in storage but not attached stuff
							InventoryStorageSlot slot = storage.GetSlot(i);
							IEntity attachedEntity = slot.GetAttachedEntity();
							if(attachedEntity)
							{
								//PrintFormat("APCA - attachedEntity: %1", attachedEntity);
								SCR_EntityHelper.DeleteEntityAndChildren(attachedEntity);
							}
						}
					}
					
					PrintFormat("APCA - Clearing Items: %1", currentItems);

					//DeleteChildrens(entity, false);
					
					// Add old items
					//PrintFormat("APCA - Adding Items: %1", playerItems);
					for(int i = 0; i < playerItems.Count(); i++)
					{
				
						EntitySpawnParams params = new EntitySpawnParams;
						params.TransformMode = ETransformMode.WORLD;
						params.Transform[3] = entity.GetOrigin();
						IEntity newItem = GetGame().SpawnEntityPrefabEx(playerItems[i].GetPrefabData().GetPrefabName(), false, GetGame().GetWorld(), params);
						
						if(m_InventoryManager.CanInsertItem(newItem))
						{
							m_InventoryManager.InsertItem(newItem);
						}
						else
						{
							SCR_EntityHelper.DeleteEntityAndChildren(newItem);
						}
					}
				}
			}
		}
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
		destination[0] = destination[0] + Math.RandomFloat(-0.5, 0.5);
		destination[2] = destination[2] + Math.RandomFloat(-0.5, 0.5);
		entity.GetWorldTransform(transform);
		transform[3] = destination;
		
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