class APCA_IEDEntityClass: ScriptedGameTriggerEntityClass 
{
};

class APCA_IEDEntity: ScriptedGameTriggerEntity
{	
	[Attribute("1", desc: "detonated by vehicles")]
	protected bool m_bDetonatedByVehicles;
	
	[Attribute("1", desc: "detonated by characters")]
	protected bool m_bDetonatedByCharacters;
	
	[Attribute("1", desc: "characters must not be prone")]
	protected bool m_bMustNotBeProne;
	
	[Attribute("5", desc: "seconds to delay explosion")]
	protected int m_DetonationDelay;
	
	bool m_hasDetonated = false;
	float m_deleteAfterSeconds = 2;
	
	SCR_MineDamageManager m_damageManagerComponent;
	BaseTriggerComponent m_triggerComponent;
	IEntity m_owner;
	RplComponent m_rplComp;
	
	void APCA_IEDEntity(IEntitySource src, IEntity parent) {
		SetEventMask(EntityEvent.INIT);
	};
	
	override void EOnInit(IEntity owner)
	{
		m_owner = owner;
		m_triggerComponent = BaseTriggerComponent.Cast(owner.FindComponent(BaseTriggerComponent));
		m_damageManagerComponent = SCR_MineDamageManager.Cast(owner.FindComponent(SCR_MineDamageManager));
		m_rplComp = RplComponent.Cast(owner.FindComponent(RplComponent));
		
		SetFlags(EntityFlags.ACTIVE, true);
	};

	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		// Only check if we haven't detonated already
		if (m_hasDetonated)
			return false;
		
		// Only check on the server
		if (!Replication.IsServer())
			return false;
		
		// Detonate it if the IED is shot or blown up
		if (m_damageManagerComponent.IsDestroyed())
		{
			TriggerIED(null);
			return false;
		};
		
		// Is the entity null?
		if (!ent)
			return false;
		
		// Detonate if the entity is a vehicle and not null if we allow vehicles?
		Vehicle veh = Vehicle.Cast(ent);
		if (m_bDetonatedByVehicles && veh)
		{
			TriggerIED(veh);
			return false;
		};
		
		// Detonate if the entity is a character and not null if we allow characters?
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(ent);
		if (m_bDetonatedByCharacters && char)
		{
			if(m_bMustNotBeProne)
			{
				CharacterAnimationComponent animComponent = CharacterAnimationComponent.Cast(char.FindComponent(CharacterAnimationComponent));
				CharacterMovementState movementState = new CharacterMovementState();
				animComponent.GetMovementState(movementState);
				if(movementState.m_iStanceIdx != ECharacterStance.PRONE)
				{
					TriggerIED(ent);
					return false;
				};
			};
		};
		return false;
	};
	
	void TriggerIED(IEntity entity)
	{
		// If we've already exploded then don't do anything
		if (m_hasDetonated)
			return;
		
		SetDetonated(true);
		
		// Play a sound here

		// Explode after detonation delay
		GetGame().GetCallqueue().CallLater(Detonate, m_DetonationDelay, false, entity);
	};
	
	void Detonate(IEntity entity) 
	{
		Rpc(RpcDetonate);
		
		if (m_triggerComponent)
			m_triggerComponent.OnUserTrigger(m_owner);
		
		ClearFlags(EntityFlags.VISIBLE, false);
		SetEventMask(EntityEvent.FRAME);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]	
	void RpcDetonate()
	{
		if (!m_owner)
			return;
	
		if (m_hasDetonated)
			return; 
		
		SetDetonated(true);
		
		ClearFlags(EntityFlags.VISIBLE, false);
		
		if (m_triggerComponent)
			m_triggerComponent.OnUserTrigger(m_owner);
	}
		
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		m_deleteAfterSeconds = m_deleteAfterSeconds - timeSlice;
		if (m_deleteAfterSeconds < 0)
		{
			ClearEventMask(EntityEvent.FRAME);
			
			if (Replication.IsServer())
				SCR_EntityHelper.DeleteEntityAndChildren(owner);
		};
	};
	
	bool GetDetonated()
	{
		return m_hasDetonated;
	};
	
	void SetDetonated(bool value)
	{
		m_hasDetonated = value;
	};
};