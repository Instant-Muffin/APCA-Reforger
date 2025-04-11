// Based on BON Action Anim
//------------------------------------------------------------------------------------------------
class APCA_FollowMeUserAction : ScriptedUserAction
{
	SCR_AIGroup group;
	//------------------------------------------------------------------------------------------------
	/*override bool CanBeShownScript(IEntity user)
	{
		ChimeraCharacter ownerChar = ChimeraCharacter.Cast(GetOwner());
		if (!ownerChar)
			return false;

		return true;
	}*/
	
	//------------------------------------------------------------------------------------------------
	/*override bool CanBePerformedScript(IEntity user)
	{
		ChimeraCharacter userChar = ChimeraCharacter.Cast(user);
		if (!userChar)
			return false;
		
		return true;
	}*/
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		ChimeraAIControlComponent AIComp = ChimeraAIControlComponent.Cast(pOwnerEntity.FindComponent(ChimeraAIControlComponent));
		AIAgent AI_Agent = AIComp.GetAIAgent();
		group = SCR_AIGroup.Cast(AI_Agent);
		if (!group)
		{
			//Print("There is a group");
			group = SCR_AIGroup.Cast(AI_Agent.GetParentGroup());
			if (group)
			{
				// Clear Current Waypoints
				array<AIWaypoint> aiWaypoints = {};
				group.GetWaypoints(aiWaypoints);
				foreach(AIWaypoint currentwp : aiWaypoints)
				{
					group.RemoveWaypoint(currentwp);
				}
				
				// Add Follow Waypoint
				vector origin = pUserEntity.GetOrigin();
				EntitySpawnParams SpawnParams = new EntitySpawnParams();
				SpawnParams.TransformMode = ETransformMode.WORLD;
				SpawnParams.Transform[3] = origin;		
				Resource followWPPrefab = Resource.Load("{A0509D3C4DD4475E}Prefabs/AI/Waypoints/AIWaypoint_Follow.et");
				AIWaypoint waypointFollow = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(followWPPrefab, null, SpawnParams)); 
				
				if (!waypointFollow)
					return;	
				
				group.AddWaypoint(waypointFollow);
			}
		}
	}
	
	void UpdateWaypoints()
	{
	
	}
}
