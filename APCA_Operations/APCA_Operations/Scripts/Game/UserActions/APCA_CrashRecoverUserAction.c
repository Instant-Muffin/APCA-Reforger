class APCA_CrashRecoverUserAction : ScriptedUserAction
{
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		vector transform[4];
		SCR_AIGroup player_group = SCR_AIGroup.Cast(pUserEntity);
		array<AIAgent> agents = {};	
		
		IEntity tele_target = player_group.GetLeaderAgent();
		
		tele_target.GetTransform(transform);
	
		
		BaseGameEntity baseGameEntity = BaseGameEntity.Cast(pUserEntity);
		if (baseGameEntity && !BaseVehicle.Cast(baseGameEntity))
		{
			baseGameEntity.Teleport(transform);
		}
		else
		{
			pUserEntity.SetWorldTransform(transform);
		}
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
