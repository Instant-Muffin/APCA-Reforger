class APCA_MarkerComponentClass : ScriptComponentClass
{
}

class APCA_MarkerComponent : ScriptComponent
{
	
	
	[Attribute(defvalue: "")]
	protected string MarkerName;
	
	[Attribute("10", desc: "Update period in seconds")]
	protected float UpdatePeriod;
	
	protected float m_fCheckDelay;
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		m_fCheckDelay -= timeSlice;
		//PrintFormat("APCA - Check Delay %1", m_fCheckDelay);
		if (m_fCheckDelay <= 0)
		{
			m_fCheckDelay = UpdatePeriod;
			
			//SCR_MapMarkerManagerComponent mapMarkerMgr = SCR_MapMarkerManagerComponent.GetInstance();
			
			//SCR_MapMarkerBase marker = mapMarkerMgr.GetStaticMarkerByID(MarkerName);
			IEntity markerEntity = GetGame().GetWorld().FindEntityByName(MarkerName);
			//PrintFormat("APCA - Looking for %1", MarkerName);
			if(!markerEntity)
				return;
			SCR_ScenarioFrameworkSlotMarker frameworkSlot = SCR_ScenarioFrameworkSlotMarker.Cast(markerEntity.FindComponent(SCR_ScenarioFrameworkSlotMarker));
			//Print("APCA - Looking for Framework");
			if(!frameworkSlot)
				return;
			
			SCR_MapMarkerBase marker = frameworkSlot.GetMapMarker();
			//Print("APCA - Looking for marker");
			if(!marker)
				return;
			//Print("APCA - Setting Marker");
			vector pos = owner.GetOrigin();
			marker.SetWorldPos(pos[0], pos[2]);
		};
	}
	
	protected override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		//Enables EOnFrame 
		SetEventMask(owner, EntityEvent.FRAME);
	}
}

