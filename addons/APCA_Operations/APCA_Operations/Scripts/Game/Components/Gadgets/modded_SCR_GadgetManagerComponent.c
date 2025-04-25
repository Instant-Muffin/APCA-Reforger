modded class SCR_GadgetManagerComponent : ScriptGameComponent
{
	//------------------------------------------------------------------------------------------------
	//! Register radio gadgets into the VON system as entries
	override void RegisterVONEntries()
	{
		if (!GetGame().GetPlayerController())
			return;

		m_VONController = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		if (!m_VONController)
			return;

		array<SCR_GadgetComponent> radiosArray = {};
		radiosArray.Copy(GetGadgetsByType(EGadgetType.RADIO)); 					// squad radios
		radiosArray.InsertAll(GetGadgetsByType(EGadgetType.RADIO_BACKPACK)); 	// backpack radio

		SCR_RadioComponent radioGadget;
		BaseRadioComponent radioComp;
		foreach (SCR_GadgetComponent radio : radiosArray)
		{
			radioGadget = SCR_RadioComponent.Cast(radio);
			if (!radioGadget)
				continue;

			radioComp = radioGadget.GetRadioComponent();
			if (!radioComp)
				continue;

			int count = 1; // Only one tranceiver per radio
			for (int i = 0; i < count; ++i)	// Get all individual transceivers (AKA channels) from the radio
			{
				SCR_VONEntryRadio radioEntry = new SCR_VONEntryRadio();
				radioEntry.SetRadioEntry(radioComp.GetTransceiver(i), i + 1, radio);

				m_VONController.AddEntry(radioEntry);
			}
		}
	}
}
