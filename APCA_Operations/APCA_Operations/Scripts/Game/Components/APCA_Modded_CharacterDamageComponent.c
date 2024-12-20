modded class SCR_CharacterDamageManagerComponent : ScriptedDamageManagerComponent
{
	IEntity PlayersHeldItem;

	//------------------------------------------------------------------------------------------------	
	IEntity GetDroppedItem()
	{
		return PlayersHeldItem;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
	{
		Print("APCA - OnLifeStateChanged");
		ChimeraCharacter character = ChimeraCharacter.Cast(GetOwner());
		if (!character)
			return;
		CharacterControllerComponent controller = character.GetCharacterController();
		if (!controller)
			return;
		// code provided by R34P3R
		IEntity currentWeapon;
        BaseWeaponManagerComponent wpnMan = controller.GetWeaponManagerComponent();
		
        if (wpnMan && wpnMan.GetCurrentWeapon())
            currentWeapon = wpnMan.GetCurrentWeapon().GetOwner();

        if (currentWeapon)
		{
            PlayersHeldItem = currentWeapon;
			PrintFormat("APCA - Current Weapon: %1",PlayersHeldItem);
		}
		
		super.OnLifeStateChanged(previousLifeState, newLifeState);
	}
	
	override void UpdateConsciousness()
	{
		bool unconscious = ShouldBeUnconscious();
		
		// If unconsciousness is not allowed, kill character
		// Also kill the character if the blood state is not high enough for being unconsciousness
		if (unconscious && (!GetPermitUnconsciousness() || (m_pBloodHitZone && m_pBloodHitZone.GetDamageState() == ECharacterBloodState.DESTROYED)))
		{
			Kill(GetInstigator());
			return;
		}
		
		ChimeraCharacter character = ChimeraCharacter.Cast(GetOwner());
		if (!character)
			return;
		
		CharacterControllerComponent controller = character.GetCharacterController();
		if (!controller)
			return;

		controller.SetUnconscious(unconscious);
		if (!unconscious)
			return;
		
		// code provided by R34P3R
		IEntity currentWeapon;
        BaseWeaponManagerComponent wpnMan = controller.GetWeaponManagerComponent();
		
        if (wpnMan && wpnMan.GetCurrentWeapon())
            currentWeapon = wpnMan.GetCurrentWeapon().GetOwner();

        if (currentWeapon)
            controller.TryEquipRightHandItem(null, EEquipItemType.EEquipTypeUnarmedContextual, true);
	}
}