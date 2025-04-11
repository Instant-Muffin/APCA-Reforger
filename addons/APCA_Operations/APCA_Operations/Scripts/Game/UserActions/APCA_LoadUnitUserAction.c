// Based on BON Action Anim
//------------------------------------------------------------------------------------------------
class APCA_LoadUnitUserAction : ScriptedUserAction
{
	protected const float m_fVehicleSearchRadius = 10;
	protected IEntity m_NearVehicle;
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		ChimeraCharacter ownerChar = ChimeraCharacter.Cast(GetOwner());
		if (!ownerChar)
			return false;
		
		SCR_CharacterControllerComponent ownerCharCtrl = SCR_CharacterControllerComponent.Cast(ownerChar.GetCharacterController());
		if (!ownerCharCtrl || !ownerCharCtrl.IsUnconscious() || ownerCharCtrl.GetLifeState() == ECharacterLifeState.DEAD)
			return false;

		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		ChimeraCharacter userChar = ChimeraCharacter.Cast(user);
		if (!userChar)
			return false;
		
		// Check if user can take crouch stance
		SCR_CharacterControllerComponent userCharCtrl = SCR_CharacterControllerComponent.Cast(userChar.GetCharacterController());
		if (!userCharCtrl || !userCharCtrl.CanChangeStance(ECharacterStanceChange.STANCECHANGE_TOCROUCH))
		{
			SetCannotPerformReason("#AR-Keybind_StanceProne");
			return false;
		}
		
		ChimeraCharacter ownerChar = ChimeraCharacter.Cast(GetOwner());
		if (!ownerChar)
			return false;
		
		// Check if carrying is already in progress
		if (ACE_Carrying_Tools.IsCarrier(user) || ACE_Carrying_Tools.IsCarried(ownerChar))
		{
			SetCannotPerformReason("#ACE_Carrying-UserAction_Carrying");
			return false;
		}
		
		// Trying to carry while unit is ragdolling will break things
		if (ownerChar.GetAnimationComponent().IsRagdollActive())
		{
			SetCannotPerformReason("Ragdolling");
			return false;
		}
		
		m_NearVehicle = null;
		GetGame().GetWorld().QueryEntitiesBySphere(user.GetOrigin(), m_fVehicleSearchRadius, AddVehicle, FilterVehicles);

		return (m_NearVehicle);
	}
	
	//------------------------------------------------------------------------------------------------
	bool AddVehicle(IEntity ent)
	{
		m_NearVehicle = ent;
		return true;
	}

	//------------------------------------------------------------------------------------------------
	bool FilterVehicles(IEntity ent)
	{
		return (ent.IsInherited(Vehicle));
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "Load in Vehicle";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!Replication.IsServer())
			return;

		//Query again on Server
		m_NearVehicle = null;
		GetGame().GetWorld().QueryEntitiesBySphere(GetOwner().GetOrigin(), m_fVehicleSearchRadius, AddVehicle, FilterVehicles);

		SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(pOwnerEntity.FindComponent(SCR_CompartmentAccessComponent));
		compartmentAccess.MoveInVehicle(m_NearVehicle, ECompartmentType.CARGO);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Methods are executed on the local player
	override bool CanBroadcastScript() { return false; };
}
