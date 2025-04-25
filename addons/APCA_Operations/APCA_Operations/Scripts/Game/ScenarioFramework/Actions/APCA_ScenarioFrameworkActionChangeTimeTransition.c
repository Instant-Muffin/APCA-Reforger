[BaseContainerProps(), SCR_ContainerActionTitle()]
class APCA_ScenarioFrameworkActionChangeTimeTransition : SCR_ScenarioFrameworkActionBase
{
	[Attribute("8", UIWidgets.Slider, "Time of day (hours)", "0 23 1")]
	int m_iHours;

	[Attribute("0", UIWidgets.Slider, "Time of day (minutes)", "0 59 1")]
	int m_iMinutes;
	
	TimeAndWeatherManagerEntity manager;
	APCA_Message messageMenu;
	string message;
	
	//------------------------------------------------------------------------------------------------
	override void OnActivate(IEntity object)
	{
		if (!CanActivate())
			return;
		
		ChimeraWorld world = GetGame().GetWorld();
		if (!world)
			return;
		
		manager = world.GetTimeAndWeatherManager();
		if (!manager)
			return;
		
		// Start players fade
		APCA_GamemodeComponent gamemode = APCA_GamemodeComponent.Cast(GetGame().GetGameMode().FindComponent(APCA_GamemodeComponent));
		gamemode.Fade("");
		
		// Set time in 3 seconds
		GetGame().GetCallqueue().CallLater(SetTime, 3000, false);
		
		// Create message
		messageMenu = APCA_Message.Cast(GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.APCA_Message)); 
		TimeContainer currentTime = manager.GetTime();
		if(currentTime.m_iHours > m_iHours) 
		{
			message = string.Format("%1 Hours Later...", currentTime.m_iHours - m_iHours);
		}
		else
		{
			message = string.Format("%1 Hours Later...", m_iHours - currentTime.m_iHours);
		}
		GetGame().GetCallqueue().CallLater(ShowMessageText, 2000);
	}
	
	void SetTime()
	{
		if (!manager)
			return;
		
		manager.SetHoursMinutesSeconds(m_iHours, m_iMinutes, 0);
	}
	
	void ShowMessageText()
	{
		if(!messageMenu)
			return;
		messageMenu.SetText(message);
		GetGame().GetCallqueue().CallLater(CloseMessageText, 3000);
	}
	
	void CloseMessageText()
	{
		messageMenu.ExitMenu();
	}
}