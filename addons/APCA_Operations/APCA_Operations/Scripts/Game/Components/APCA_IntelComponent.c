class APCA_IntelComponentClass: GameComponentClass
{
}

class APCA_IntelComponent: GameComponent
{
	[Attribute(defvalue: "")]
	protected ref SCR_HintUIInfo IntelHint;
	
	void ViewIntelHint()
	{
		if (SCR_HintManagerComponent.GetInstance().GetCurrentHint() != IntelHint)
		{
			IntelHint.SetPersistent(true);
			SCR_HintManagerComponent.ShowHint(IntelHint);
		}
	}
}
