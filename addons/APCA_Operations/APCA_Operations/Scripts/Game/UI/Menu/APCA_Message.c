
class APCA_Message : MenuBase
{
	protected RichTextWidget Text;
	
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		// Set up Exit Button
		Text = RichTextWidget.Cast(GetRootWidget().FindAnyWidget("Text"));
		if (Text)
        {
			Text.SetText("");        
        } 
	}
	
	void SetText(string message)
	{
		Text.SetText(message);    
	}
	
	void ExitMenu()
	{
		GetManager().CloseMenu(this);
	}
};

class APCA_MessageComponent: ScriptedWidgetComponent {}

modded enum ChimeraMenuPreset
{
	APCA_Message
}