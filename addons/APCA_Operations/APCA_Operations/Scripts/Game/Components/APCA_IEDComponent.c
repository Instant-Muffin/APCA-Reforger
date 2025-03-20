class APCA_IEDComponentClass: ScriptComponentClass
{
}

class APCA_IEDComponent: ScriptComponent
{
	[Attribute("Investigate Hint")]
	protected ref SCR_HintUIInfo InvestigateHint;
	int DefusalState; 
	
	//------------------------------------------------------------------------------------------------
	protected override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		DefusalState = 0;
	}
	
	int GetDefusalState()
	{
		return DefusalState;
	};
	
	void SetDefusalState(int value)
	{
		DefusalState = value;
	};
	
	void ViewIEDHint()
	{
		if (SCR_HintManagerComponent.GetInstance().GetCurrentHint() != InvestigateHint)
		{
			InvestigateHint.SetPersistent(true);
			SCR_HintManagerComponent.ShowHint(InvestigateHint);
		}
	}
}
