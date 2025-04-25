[ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "")]
class APCA_GamemodeComponentClass : SCR_BaseGameModeComponentClass
{
	
}

class APCA_GamemodeComponent : SCR_BaseGameModeComponent
{
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void Fade(string message)
	{
		FadeOut(1000);
		
		GetGame().GetCallqueue().CallLater(FadeIn, 6000, false, 3000);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FadeOut(int duration)
	{
		SCR_ScreenEffectsManager manager = SCR_ScreenEffectsManager.GetScreenEffectsDisplay();
		if (manager)
		{
			SCR_FadeInOutEffect fade = SCR_FadeInOutEffect.Cast(manager.GetEffect(SCR_FadeInOutEffect));
			if (fade)
				fade.FadeOutEffect(true, duration);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FadeIn(int duration)
	{
		SCR_ScreenEffectsManager manager = SCR_ScreenEffectsManager.GetScreenEffectsDisplay();
		if (manager)
		{
			SCR_FadeInOutEffect fade = SCR_FadeInOutEffect.Cast(manager.GetEffect(SCR_FadeInOutEffect));
			if (fade)
				fade.FadeOutEffect(false, duration);
		}
	}
}