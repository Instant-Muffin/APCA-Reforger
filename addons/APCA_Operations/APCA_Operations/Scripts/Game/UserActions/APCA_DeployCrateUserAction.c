
class APCA_DeployCrateUserAction : ScriptedUserAction
{
	[Attribute("Crate Resource Name")]
	protected string crateResourceName;
	
	[Attribute("Failure Hint")]
	protected ref SCR_HintUIInfo failHint;
	
	
	//------------------------------------------------------------------------------------------------
	override event bool CanBePerformedScript(IEntity user)
	{
		bool canPerform = false;
		SetCannotPerformReason("Altitude too high");
		
		vector origin = user.GetOrigin();
		if(origin[1] - GetGame().GetWorld().GetSurfaceY(origin[0], origin[2]) < 2)
			canPerform = true;
		
		return canPerform;
	}
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		EntitySpawnParams SpawnParams = new EntitySpawnParams();
		SpawnParams.TransformMode = ETransformMode.WORLD;
		
		vector origin = pOwnerEntity.GetOrigin();
		vector target_pos;
		
		bool EmptySpace = SCR_WorldTools.FindEmptyTerrainPosition(target_pos, origin, 4);
		if(!EmptySpace)
		{
			if(SCR_HintManagerComponent.GetInstance().GetCurrentHint() != failHint)
			{
				SCR_HintManagerComponent.ShowHint(failHint);
			}
			return;
		}
		SpawnParams.Transform[3] = target_pos;
		
		IEntity crate = IEntity.Cast(GetGame().SpawnEntityPrefab(Resource.Load(crateResourceName), pOwnerEntity.GetWorld(), SpawnParams));
		AudioSystem.PlaySound("{939D0013DBA65B1F}Sounds/Vehicles/_SharedData/Crashes/Truck/Samples/Crash_Truck_Body_6.wav");
	}
	
}
