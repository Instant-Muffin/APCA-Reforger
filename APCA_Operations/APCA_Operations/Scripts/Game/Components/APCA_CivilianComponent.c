[ComponentEditorProps(category: "APCA/Civilian", description: "TODO")]
class APCA_CivilianComponentClass : ScriptComponentClass
{
}

class APCA_CivilianComponent : ScriptComponent
{
	IEntity groupEntity;
	SCR_AIGroup group;
	//ref array<IEntity> worldEntities = {};
	ref array<SCR_MapDescriptorComponent> locationMapComponents = {};
	
	//---------------------------------------------------------------------------------------------------	
	override void OnPostInit(IEntity owner)
	{
		groupEntity = owner;
		group = SCR_AIGroup.Cast(groupEntity);
		
		super.OnPostInit(owner);
		
		AddWaypoints();
	}
	
	// Courtesy of Bacon
	bool CollectMapDescriptors() {
        
        SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
        if (!core)
            return false;
        
        set<SCR_EditableEntityComponent> entities = new set<SCR_EditableEntityComponent>;
        
        core.GetAllEntities(entities, true, true);
        
        SCR_MapDescriptorComponent mapDescriptor;
        int compType;
        string enumCategory;
        foreach (SCR_EditableEntityComponent comp : entities) {
            mapDescriptor = SCR_MapDescriptorComponent.Cast(comp.GetOwner().FindComponent(SCR_MapDescriptorComponent));
            if (!mapDescriptor)
                continue;
            locationMapComponents.Insert(mapDescriptor);
        }
        
        return true;
    }
	
	/*protected bool QueryCallback(IEntity entity)
	{
		SCR_MapDescriptorComponent mapDescriptorComponent = SCR_MapDescriptorComponent.Cast(entity.FindComponent(SCR_MapDescriptorComponent));
		if(mapDescriptorComponent)
		{
			locationMapComponents.Insert(entity);
			Print("APCA CIV - Found Location");
		}
		Print("APCA CIV - Non location entity");
		// Continue querying if distance didn't drop to zero
		return true;
	}*/
	
	//------------------------------------------------------------------------------------------------
	//! Return list of location entities
	void GetLocationList()
	{
		BaseWorld world = GetGame().GetWorld();
		if (world)
		{
			CollectMapDescriptors();
			//world.GetActiveEntities(worldEntities);
			//world.QueryEntitiesBySphere(groupEntity.GetOrigin(), 5000000, QueryCallback);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void AddWaypoints()
	{
		if(group) 
		{
			ArmaReforgerScripted game = GetGame();
			if(!game.InPlayMode())
			{
				return;
			}
			
			//Clear Waypoints
			/*array<AIWaypoint> currentWaypoints = {};
			group.GetWaypoints(currentWaypoints);
			foreach(AIWaypoint currentwp : currentWaypoints)
			{
				group.RemoveWaypoint(currentwp);
			}*/
			
			
			//Add Get In Waypoint
			vector origin = groupEntity.GetOrigin();
			EntitySpawnParams SpawnParams = new EntitySpawnParams();
			SpawnParams.TransformMode = ETransformMode.WORLD;
			SpawnParams.Transform[3] = origin;
			AIWaypoint wp = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{62DAF65820BF9857}Prefabs/AI/Waypoints/AIWaypoint_GetInNearest.et"), groupEntity.GetWorld(), SpawnParams));
			group.AddWaypoint(wp);
			
			/*GetGame().SpawnEntityPrefabLocal(
					Resource.Load("{58D0FB3206B6F859}Prefabs/Props/Crates/Crate_01_brown.et"), 
					groupEntity.GetWorld(), 
					SpawnParams
				);*/
			
			//Add Random Movement to Locations
			GetLocationList();
			array<AIWaypoint> cycleWaypoints  = {};
			if(locationMapComponents.Count() > 5) 
			{
				for(int i = 0; i < 5; i++)
				{
					SpawnParams.Transform[3] = IEntity.Cast(locationMapComponents.GetRandomElement().Item().Entity()).GetOrigin();
					Resource moveWPPrefab = Resource.Load("{62DAF65820BF9857}Prefabs/AI/Waypoints/AIWaypoint_Move_CIV_Car.et");
					cycleWaypoints.Insert(SCR_AIWaypoint.Cast(GetGame().SpawnEntityPrefabLocal(moveWPPrefab, null, SpawnParams)));
					//cycleWaypoints.Insert(wp);
					//SCR_AIWaypoint.Cast(GetGame().SpawnEntityPrefabLocal(waypointPrefab, null, params)));
				}
			}
			
			//Add Cycle Waypoint
			SpawnParams.Transform[3] = origin; 			
			Resource cycleWPPrefab = Resource.Load("{62DAF65820BF9857}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et");
			AIWaypointCycle waypointCycle = AIWaypointCycle.Cast(GetGame().SpawnEntityPrefabLocal(cycleWPPrefab, null, SpawnParams)); 
			
			if (!waypointCycle)
				return;	
			
			waypointCycle.SetWaypoints(cycleWaypoints);
			group.AddWaypoint(waypointCycle);
		}
	}
}

