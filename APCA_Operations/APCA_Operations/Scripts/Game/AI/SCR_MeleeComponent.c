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
		IEntity Victim = m_MeleeHitData.m_Entity;
		APCA_HVTComponent hvt = APCA_HVTComponent.Cast(Victim.FindComponent(APCA_HVTComponent));
		if (hvt)
		{
			Print("HVT Melee!");
			SCR_CharacterDamageManagerComponent damageComp = SCR_CharacterDamageManagerComponent.Cast(Victim.FindComponent(SCR_CharacterDamageManagerComponent));
			if (damageComp)
			{			
				FactionManager factionManager = GetGame().GetFactionManager();
				SCR_FactionAffiliationComponent FactionAffiliation = SCR_FactionAffiliationComponent.Cast(Victim.FindComponent(SCR_FactionAffiliationComponent));	
				if (!factionManager)
					return;
		
				Faction faction = factionManager.GetFactionByKey("CIV");
				if (!faction)
					return;
				
				ChimeraAIControlComponent AIComp = ChimeraAIControlComponent.Cast(Victim.FindComponent(ChimeraAIControlComponent));
				AIAgent Victim_Agent = AIComp.GetAIAgent();
				SCR_AIGroup Group = SCR_AIGroup.Cast(Victim_Agent);
				if (!Group)
				{
					Print("There is a group");
					Group = SCR_AIGroup.Cast(Victim_Agent.GetParentGroup());
					if (Group)
					{
						Print("There is a group owner. Setting faction");
						Group.SetFaction(faction);
					}
				}
				
				
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