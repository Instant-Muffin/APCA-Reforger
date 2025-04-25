modded class SCR_PlayerController : PlayerController
{
	
	//------------------------------------------------------------------------------------------------
	override void SetPossessedEntity(IEntity entity)
	{
		super.SetPossessedEntity(entity);
		if(!entity)
			return;
		
		//Print("Possessing Entity");
		
		MFN_IdentitySelectorCharacterComponent IDComp = MFN_IdentitySelectorCharacterComponent.Cast(entity.FindComponent(MFN_IdentitySelectorCharacterComponent));
		if(IDComp)
			IDComp.RemoteLoadIdentity(false);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		super.OnControlledEntityChanged(from, to);
		if(!to)
			return;
		
		MFN_IdentitySelectorCharacterComponent IDComp = MFN_IdentitySelectorCharacterComponent.Cast(to.FindComponent(MFN_IdentitySelectorCharacterComponent));
		if(IDComp)
			IDComp.RemoteLoadIdentity(false);
	}
}