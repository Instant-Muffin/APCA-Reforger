class APCA_IEDInteractUserAction : ScriptedUserAction
{
	
	[Attribute("IED Action Type")]
	protected int ActionType;
	
	[Attribute("Action Sound")]
	protected string ActionSound;
	
	protected IEntity m_pUser;
	
	APCA_IEDComponent IEDComp;
	SCR_PressureTriggerComponent TriggerComp;
	
	/*
	//------------------------------------------------------------------------------------------------
	//! Start gadget animation
	override void OnActionStart(IEntity pUserEntity)
	{
		super.OnActionStart(pUserEntity);
		
		ChimeraCharacter character = ChimeraCharacter.Cast(pUserEntity);
		if (!character)
			return;
		
		CharacterControllerComponent charController = character.GetCharacterController();
		if (charController)
		{
			CharacterAnimationComponent pAnimationComponent = character.GetAnimationComponent();
			int itemActionId = pAnimationComponent.BindCommand(ANIMATION_BIND_COMMAND);
	
			ItemUseParameters animParams = new ItemUseParameters();
			animParams.SetEntity(GetOwner());
			animParams.SetAllowMovementDuringAction(false);
			animParams.SetKeepInHandAfterSuccess(true);
			animParams.SetCommandID(itemActionId);
			animParams.SetCommandIntArg(1);
	
			charController.TryUseItemOverrideParams(animParams);
		}
		
		m_pUser = pUserEntity;
	}
	
	//------------------------------------------------------------------------------------------------
	//! End gadget animation
	override void OnActionCanceled(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.OnActionCanceled(pOwnerEntity, pUserEntity);
		
		ChimeraCharacter character = ChimeraCharacter.Cast(pUserEntity);
		if (!character)
			return;
		
		CharacterControllerComponent charController = character.GetCharacterController();
		if (charController)
		{
			CharacterAnimationComponent pAnimationComponent = charController.GetAnimationComponent();
			int itemActionId = pAnimationComponent.BindCommand("CMD_Item_Action");
			CharacterCommandHandlerComponent cmdHandler = CharacterCommandHandlerComponent.Cast(pAnimationComponent.GetCommandHandler());
			if (cmdHandler)
				cmdHandler.FinishItemUse();
		}
	}*/
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		Print("APCA - TEST");
		IEDComp = APCA_IEDComponent.Cast(pOwnerEntity.FindComponent(APCA_IEDComponent));
		TriggerComp = SCR_PressureTriggerComponent.Cast(pOwnerEntity.FindComponent(SCR_PressureTriggerComponent));
		
		// 0 - Investigate
		// 1 - Remove Casing
		// 2 - Expose Wiring
		// 3 - Bypass Power 
		// 4 - Dismantle Detonator
		
		if(IEDComp && TriggerComp)
		{
			if(ActionType == 0)
			{
				IEDComp.ViewIEDHint();
				if(IEDComp.GetDefusalState() == 0)
				{
					IEDComp.SetDefusalState(1);
				};
				return;
			};

			if(IEDComp.GetDefusalState() == ActionType)
			{
				IEDComp.SetDefusalState(ActionType + 1);
			}
			else
			{
				TriggerComp.Detonate();
			};
			
			switch (ActionType)
			{
				case 0:
					AudioSystem.PlaySound("{7839D4FC9BC94358}Sounds/Items/Equipment/Shovel/Samples/Items_Shovel_Debris_Generic_02.wav");
				break;
				
				case 1:
					AudioSystem.PlaySound("{40C0FAB385DD030A}Sounds/Items/Equipment/Radios/Samples/Items_Radio_Inspect_Foley_02.wav");
				break;
				
				case 2:
					AudioSystem.PlaySound("{383507350AA38821}Sounds/Items/MineFlags/Samples/Items_MineFlag_Place_02.wav");
				break;
				
				case 3:
					AudioSystem.PlaySound("{383507350AA38821}Sounds/Items/MineFlags/Samples/Items_MineFlag_Place_02.wav");
				break;
				
				case 4:
					AudioSystem.PlaySound("{2B2D5AFBD9BCAF30}Sounds/Items/SuppliesSupport/RearmKit/Samples/Items_RearmKit_Prepare_03.wav");
					GetGame().GetCallqueue().CallLater(SCR_EntityHelper.DeleteEntityAndChildren, 1, false, pOwnerEntity);
				break;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		IEntity gadget = GetHeldGadget(user);
		if (!gadget)
            return false;
		
		string gadgetName = gadget.GetPrefabData().GetPrefabName();
        if (!gadgetName.Contains("ETool"))
    		return false;
		
		IEDComp = APCA_IEDComponent.Cast(GetOwner().FindComponent(APCA_IEDComponent));
		int DefusalState = IEDComp.GetDefusalState();
		if(IEDComp)
		{
			if((ActionType != 0) && (DefusalState == 0))
				return false;
			
			if((ActionType != 0) && (ActionType < DefusalState))
				return false;
		};
		
		return super.CanBeShownScript(user);
	}
	
	void PlaySound(string sound)
	{
		SoundComponent soundComponent = SoundComponent.Cast(GetOwner().FindComponent(SoundComponent));
		soundComponent.SoundEvent(sound);
	}
	
	//! Get gadget entity
	IEntity GetHeldGadget(notnull IEntity ent)
	{
		SCR_GadgetManagerComponent gadgetManager = SCR_GadgetManagerComponent.GetGadgetManager(ent);
		if (!gadgetManager)
			return null;
		
		return gadgetManager.GetHeldGadget();
	}
	

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return true;
	}
};
