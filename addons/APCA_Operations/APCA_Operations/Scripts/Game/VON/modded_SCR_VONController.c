//------------------------------------------------------------------------------------------------
//! Scripted VON input and control, attached to SCR_PlayerController
modded class SCR_VONController : ScriptComponent
{
	protected const string ACTION_LONG_RANGE = "VONLongRange";
	
	//------------------------------------------------------------------------------------------------
	override void ActionVONBroadcast(float value, EActionTrigger reason = EActionTrigger.UP)
	{
		if (!m_VONComp)
			return;
		
		if(m_aEntries.Count() > 0)
		{
			if(m_LongRangeEntry == m_ActiveEntry)
			{
				SetVONLongRange(false, false);
				SetEntryActive(m_SavedEntry);
			}
		}

		EVONTransmitType transmitType;

		transmitType = EVONTransmitType.CHANNEL;

		if (reason != EActionTrigger.UP && m_eVONType == transmitType)
			return;

		SetVONBroadcast(reason != EActionTrigger.UP, transmitType);

		if (reason != EActionTrigger.UP)
			m_sActiveHoldAction = ACTION_CHANNEL;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void ActionVONLRBroadcast(float value, EActionTrigger reason = EActionTrigger.UP)
	{
		if (!m_VONComp)
			return;
		
		SetVONLongRange(true, true);

		EVONTransmitType transmitType;

		transmitType = EVONTransmitType.LONG_RANGE;

		if (reason != EActionTrigger.UP && m_eVONType == transmitType)
			return;

		SetVONBroadcast(reason != EActionTrigger.UP, transmitType);

		if (reason != EActionTrigger.UP)
			m_sActiveHoldAction = ACTION_LONG_RANGE;
	}
	
	override protected void Init(IEntity owner)
	{
		super.Init(owner);
		m_InputManager = GetGame().GetInputManager();
		if (m_InputManager)
		{
			m_InputManager.AddActionListener(ACTION_LONG_RANGE, EActionTrigger.DOWN, ActionVONLRBroadcast);
			m_InputManager.AddActionListener(ACTION_LONG_RANGE, EActionTrigger.UP, ActionVONLRBroadcast);
		}
	}
	
	override protected void Cleanup()
	{
		super.Cleanup();
		
		m_InputManager = GetGame().GetInputManager();
		if (m_InputManager)
		{
			m_InputManager.RemoveActionListener(ACTION_LONG_RANGE, EActionTrigger.DOWN, ActionVONLRBroadcast);
			m_InputManager.RemoveActionListener(ACTION_LONG_RANGE, EActionTrigger.UP, ActionVONLRBroadcast);
		}
	}
};
