modded class SCR_MapEntity : MapEntity
{
	override void OpenMap(MapConfiguration config)
	{
		super.OpenMap(config);

		// Quick Markers
		GetGame().GetInputManager().AddActionListener("Map1", EActionTrigger.DOWN, QuickMarkerDot);
		GetGame().GetInputManager().AddActionListener("Map2", EActionTrigger.DOWN, QuickMarkerCross);
		GetGame().GetInputManager().AddActionListener("Map3", EActionTrigger.DOWN, QuickMarkerCircle);
		
		// Pointing
		//GetGame().GetInputManager().AddActionListener("MapCtrl", EActionTrigger.DOWN, StartPointing);
		//GetGame().GetInputManager().AddActionListener("MapCtrl", EActionTrigger.UP, EndPointing);
		//GetGame().GetInputManager().AddActionListener("MapCtrl", EActionTrigger.PRESSED, UpdatePointing);
	}
	
	override void CloseMap()
	{
		super.CloseMap();

		// Quick Markers 
		GetGame().GetInputManager().RemoveActionListener("Map1", EActionTrigger.DOWN, QuickMarkerDot);
		GetGame().GetInputManager().RemoveActionListener("Map2", EActionTrigger.DOWN, QuickMarkerCross);
		GetGame().GetInputManager().RemoveActionListener("Map3", EActionTrigger.DOWN, QuickMarkerCircle);
		
		// Pointing
		//GetGame().GetInputManager().RemoveActionListener("MapCtrl", EActionTrigger.DOWN, StartPointing);
		//GetGame().GetInputManager().RemoveActionListener("MapCtrl", EActionTrigger.UP, EndPointing);
		//GetGame().GetInputManager().RemoveActionListener("MapCtrl", EActionTrigger.PRESSED, UpdatePointing);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void QuickMarkerDot()
	{
		// Gather Data
		SCR_MapMarkerManagerComponent mapMarkerMgr = SCR_MapMarkerManagerComponent.GetInstance();
		SCR_EScenarioFrameworkMarkerCustom icon = SCR_EScenarioFrameworkMarkerCustom.DOT;
		SCR_EScenarioFrameworkMarkerCustomColor color = SCR_EScenarioFrameworkMarkerCustomColor.BLUE;
		float posX = 0;
		float posY = 0;
		GetMapCursorWorldPosition(posX, posY);
		
		// Build Marker
		SCR_MapMarkerBase marker = new SCR_MapMarkerBase();
		marker.SetType(SCR_EMapMarkerType.PLACED_CUSTOM);
		marker.SetIconEntry(icon);
		marker.SetColorEntry(color);
		marker.SetCustomText("");
		marker.SetWorldPos(posX, posY);
		
		mapMarkerMgr.InsertStaticMarker(marker, false, false);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void QuickMarkerCross()
	{
		// Gather Data
		SCR_MapMarkerManagerComponent mapMarkerMgr = SCR_MapMarkerManagerComponent.GetInstance();
		SCR_EScenarioFrameworkMarkerCustom icon = SCR_EScenarioFrameworkMarkerCustom.CROSS2;
		SCR_EScenarioFrameworkMarkerCustomColor color = SCR_EScenarioFrameworkMarkerCustomColor.BLUE;
		float posX = 0;
		float posY = 0;
		GetMapCursorWorldPosition(posX, posY);
		
		// Build Marker
		SCR_MapMarkerBase marker = new SCR_MapMarkerBase();
		marker.SetType(SCR_EMapMarkerType.PLACED_CUSTOM);
		marker.SetIconEntry(icon);
		marker.SetColorEntry(color);
		marker.SetCustomText("");
		marker.SetWorldPos(posX, posY);
		
		mapMarkerMgr.InsertStaticMarker(marker, false, false);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void QuickMarkerCircle()
	{
		// Gather Data
		SCR_MapMarkerManagerComponent mapMarkerMgr = SCR_MapMarkerManagerComponent.GetInstance();
		SCR_EScenarioFrameworkMarkerCustom icon = SCR_EScenarioFrameworkMarkerCustom.CIRCLE2;
		SCR_EScenarioFrameworkMarkerCustomColor color = SCR_EScenarioFrameworkMarkerCustomColor.BLUE;
		float posX = 0;
		float posY = 0;
		GetMapCursorWorldPosition(posX, posY);
		
		// Build Marker
		SCR_MapMarkerBase marker = new SCR_MapMarkerBase();
		marker.SetType(SCR_EMapMarkerType.PLACED_CUSTOM);
		marker.SetIconEntry(icon);
		marker.SetColorEntry(color);
		marker.SetCustomText("");
		marker.SetWorldPos(posX, posY);
		
		mapMarkerMgr.InsertStaticMarker(marker, false, false);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void StartPointing()
	{
		//Print(" APCA - Creating Marker");
		PlayerManager playerManager = GetGame().GetPlayerManager();
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController)
			return;
		
		// Get Nearby Players
		array<int> allPlayers = {};
		array<int> nearbyPlayers = {};
		playerManager.GetPlayers(allPlayers);
		IEntity playerEntity = IEntity.Cast(playerController.GetControlledEntity());
		vector playerPosition = playerEntity.GetOrigin();
		//PrintFormat("APCA - nearbyPlayers: %1",nearbyPlayers);
		for(int i = 0; i < allPlayers.Count(); i++)
		{
			IEntity otherPlayerEntity = playerManager.GetPlayerControlledEntity(i);
			if(!otherPlayerEntity)
				continue;
			vector otherPlayerPosition = otherPlayerEntity.GetOrigin();
			float distance = vector.Distance(playerPosition, otherPlayerPosition);
			if(distance < 5)
				nearbyPlayers.Insert(i);
		}
		
		// Gather Data
		int playerID = playerController.GetPlayerId();
		string name = playerManager.GetPlayerName(playerID);
		SCR_MapMarkerManagerComponent mapMarkerMgr = SCR_MapMarkerManagerComponent.GetInstance();
		SCR_EScenarioFrameworkMarkerCustom icon = SCR_EScenarioFrameworkMarkerCustom.DOT2;
		SCR_EScenarioFrameworkMarkerCustomColor color = SCR_EScenarioFrameworkMarkerCustomColor.REFORGER_ORANGE;
		
		// Build Marker
		SCR_MapMarkerBase marker = new SCR_MapMarkerBase();
		marker.SetType(SCR_EMapMarkerType.PLACED_CUSTOM);
		marker.SetMarkerID(playerID + 123); // PlayerID + arbitrary number for pointing markers to deconflict?
		marker.SetMarkerOwnerID(playerID);
		marker.SetIconEntry(icon);
		marker.SetColorEntry(color);
		marker.SetCustomText(name);
		marker.SetWorldPos(0, 0);
		marker.SetCanBeRemovedByOwner(true);
		marker.SetVisible(false);
		
		mapMarkerMgr.InsertStaticMarker(marker, true, false);
		
		Rpc(SetPointingVisibility, marker, nearbyPlayers);
	}
	
	protected void UpdatePointing()
	{
		//Print(" APCA - Updating Marker");
		SCR_MapMarkerManagerComponent mapMarkerMgr = SCR_MapMarkerManagerComponent.GetInstance();
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		SCR_MapMarkerBase marker = mapMarkerMgr.GetStaticMarkerByID(playerController.GetPlayerId() + 123);
		if(marker)
		{
			float posX = 0;
			float posY = 0;
			GetMapCursorWorldPosition(posX, posY);
			marker.SetWorldPos(posX, posY);
		}
	}
	
	protected void EndPointing()
	{
		//Print(" APCA - Deleting Marker");
		
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		Rpc(RemoveMarker, playerController.GetPlayerId() + 123)
		
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RemoveMarker(int markerId)
	{
		SCR_MapMarkerManagerComponent mapMarkerMgr = SCR_MapMarkerManagerComponent.GetInstance();
		SCR_MapMarkerBase marker = mapMarkerMgr.GetStaticMarkerByID(markerId);
		if(marker)
		{
			mapMarkerMgr.RemoveStaticMarker(marker);	
		}
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void SetPointingVisibility(SCR_MapMarkerBase marker, array<int> nearbyPlayerIds)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		IEntity playerEnt = playerController.GetControlledEntity();
		int playerID = playerController.GetPlayerId();
		for(int i = 0; i < nearbyPlayerIds.Count(); i++)
		{
			if(playerID == nearbyPlayerIds[i])
				marker.SetVisible(true);
		}
	}
}