
modded class SCR_PressureTriggerComponent : SCR_BaseTriggerComponentClass
{	
	[Attribute("0", "Must not be prone?")]
	protected bool m_bProneSafe;
	
	[Attribute("1", desc: "Time to delay explosion (ms)")]
	protected int m_DetonationDelay;
	
	//------------------------------------------------------------------------------------------------
	override void EOnContact(IEntity owner, IEntity other, Contact contact)
	{
		Print("APCA - Contact");
		if (GetGame().GetWorld().GetWorldTime() - m_fLastTryTime < MIN_DELAY)
			return;
		
		Physics otherPhysics = other.GetPhysics();
		if (!otherPhysics)
			return;
		
	 	float otherMass = otherPhysics.GetMass();

		VehicleWheeledSimulation vehicleSimulation = VehicleWheeledSimulation.Cast(other.FindComponent(VehicleWheeledSimulation));
		if (vehicleSimulation)
			otherMass /= vehicleSimulation.WheelCount(); // assume it's a vehicle and assume min. weight it lays on the trigger is weight / wheels count
		
		m_fLastTryTime = GetGame().GetWorld().GetWorldTime();
		
		if (otherMass < m_fMinWeight)
			return; // Too light, won't set the trigger off
		
		// Don't detonate if we don't explode on prone players (so they can reliably prone to disarm it)
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(other);
		if (char && m_bProneSafe)
		{
			CharacterAnimationComponent animComponent = CharacterAnimationComponent.Cast(char.FindComponent(CharacterAnimationComponent));
			CharacterMovementState movementState = new CharacterMovementState();
			animComponent.GetMovementState(movementState);
			if(movementState.m_iStanceIdx == ECharacterStance.PRONE)
			{
				return;
			};
		};
		
		BaseTriggerComponent baseTriggerComponent = BaseTriggerComponent.Cast(GetOwner().FindComponent(BaseTriggerComponent));
		if (!baseTriggerComponent)
			return;
		
		// Okay we're exploding
		SoundComponent soundComponent = SoundComponent.Cast(owner.FindComponent(SoundComponent));
		if(soundComponent)
			soundComponent.SoundEvent("SOUND_DRY");
		
		GetGame().GetCallqueue().CallLater(Detonate, m_DetonationDelay);
	}
	
	void Detonate()
	{
		// Delay it to next frame, cannot delete entity at the same time as rpc
		GetGame().GetCallqueue().CallLater(RPC_DoTrigger);
		Rpc(RPC_DoTrigger);
	}
}