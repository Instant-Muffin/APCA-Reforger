class APCA_CheckIntelUserAction : ScriptedUserAction
{
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		APCA_IntelComponent IntelComp = APCA_IntelComponent.Cast(pOwnerEntity.FindComponent(APCA_IntelComponent));
		if(IntelComp)
		{
			IntelComp.ViewIntelHint();
		}
	}
};
