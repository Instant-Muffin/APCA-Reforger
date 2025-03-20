modded class SCR_ScenarioFrameworkArea : SCR_ScenarioFrameworkLayerBase
{	
	[Attribute(desc: "Should the dynamic Spawn/Despawn based on alititude of observer cameras be enabled?", category: "Activation")]
	bool m_bDynamicDespawnExcludeAir;
	
	//------------------------------------------------------------------------------------------------
	//! \return Represents the trigger entity associated with this area.
	bool GetExcludeAir()
	{
		return m_bDynamicDespawnExcludeAir;
	}
}
