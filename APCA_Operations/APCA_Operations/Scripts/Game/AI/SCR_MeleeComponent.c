  /* ------------------------------------------------------------------------------------------------ /
 / APCA Modded Base Class										                                      /
/ ------------------------------------------------------------------------------------------------ */

//------------------------------------------------------------------------------------------------
modded class SCR_MeleeComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	//!
	void RefreshVictimResilience()
	{	
		SCR_CharacterDamageManagerComponent damageComp = SCR_CharacterDamageManagerComponent.Cast(m_MeleeHitData.m_Entity.FindComponent(SCR_CharacterDamageManagerComponent));
		if (!damageComp)
			return;
		
		damageComp.GetResilienceHitZone().SetHealth(0);
	}
	
	//------------------------------------------------------------------------------------------------
	//!
	void RefreshVictimBloodLevel()
	{
		SCR_CharacterDamageManagerComponent damageComp = SCR_CharacterDamageManagerComponent.Cast(m_MeleeHitData.m_Entity.FindComponent(SCR_CharacterDamageManagerComponent));
		if (!damageComp)
			return;
		
		damageComp.GetBloodHitZone().SetHealth(2000);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void HandleDamageDelayed()
	{
		//-----------------------------------------
		// Check if entity is an APCA_HVT
		APCA_HVTComponent hvt = APCA_HVTComponent.Cast(m_MeleeHitData.m_Entity.FindComponent(APCA_HVTComponent));
		if (hvt)
		{
			Print("HVT Melee!");
			SCR_CharacterDamageManagerComponent damageComp = SCR_CharacterDamageManagerComponent.Cast(m_MeleeHitData.m_Entity.FindComponent(SCR_CharacterDamageManagerComponent));
			if (damageComp)
			{
				damageComp.ForceUnconsciousness();
				damageComp.GetResilienceHitZone().SetHealth(0);
				damageComp.GetBloodHitZone().SetHealth(2000);
				GetGame().GetCallqueue().CallLater(RefreshVictimBloodLevel, 1000, true);
				GetGame().GetCallqueue().CallLater(RefreshVictimResilience, 1000, true);
				return;
			}	
		}
		//-----------------------------------------
		
		return super.HandleDamageDelayed();
	}
};