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
			Type().ToString(), requestComponent.GetPlayerId(), typename.EnumToString(SCR_ESpawnResult, response));
		#endif
		
		if(m_playerLoadout) 
		{
			//GetPlayerLoadoutComponent_S(requestComponent.GetPlayerId()).GetLoadout() = m_playerLoadout;
		}else 
		{
			//m_playerLoadout = GetPlayerLoadoutComponent_S(requestComponent.GetPlayerId()).GetLoadout();
			//m_playerInitialLoadout = m_playerLoadout;
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
			Type().ToString(), playerId, playerEntity, killerEntity, killer.GetInstigatorPlayerID());
		#endif
		
		//m_playerLoadout = GetPlayerLoadoutComponent_S(playerId).GetLoadout();

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
		//PrintFormat("APCA - %1::OnPlayerEntityLost_S(playerId: %2)", Type().ToString(), playerId);
		#endif
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPlayerSpawnFailed_S(int playerId)
	{
		super.OnPlayerSpawnFailed_S(playerId);

		int delay = Math.RandomFloat(900, 1100);
	}
};
