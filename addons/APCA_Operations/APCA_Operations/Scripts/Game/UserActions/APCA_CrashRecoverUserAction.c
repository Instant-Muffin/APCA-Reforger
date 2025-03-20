class APCA_CrashRecoverUserAction : ScriptedUserAction
{
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{		
		ChimeraAIControlComponent PlayerComponent = ChimeraAIControlComponent.Cast(pUserEntity.FindComponent(ChimeraAIControlComponent));
		AIAgent Player_Agent = PlayerComponent.GetAIAgent();
		AIGroup Player_Group = Player_Agent.GetParentGroup();
		if (Player_Group)
		{
			Print("There is a group");
		
			IEntity tele_target = Player_Group.GetLeaderAgent();
			
			vector transform[4]; 
			vector destination = tele_target.GetOrigin();
			pUserEntity.GetWorldTransform(transform);
			transform[3] = destination;
	
		
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
		else
		{
			Print("There is no group...");
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
