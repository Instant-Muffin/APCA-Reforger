//------------------------------------------------------------------------------------------------
class MFN_OpenIdentitySelector_UserAction : ScriptedUserAction
{
	protected override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.MFN_IdentitySelector); 
	}	
	
	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return true;
	}
};
