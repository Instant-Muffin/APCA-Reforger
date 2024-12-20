class APCA_RespawnAllUserAction : ScriptedUserAction
{
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RespawnAllPlayers();
	}
	
	void RespawnAllPlayers()
	{
		APCA_GameModeCoop gameMode = APCA_GameModeCoop.Cast(GetGame().GetGameMode());
		gameMode.ForceRespawn();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return true;
	}
};
