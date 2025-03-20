class APCA_BaseComponentClass: GameComponentClass
{
}

class APCA_BaseComponent: GameComponent
{
	[Attribute(desc: "Set the labels for the non-editable entity. NOTE! Only use this when the Prefab is non-editable otherwise it will take these labels instead of the EditableEntity Labels on the prefab. If this is required add your own Labels to a custom SCR_BaseEntityCatalogData to prevent breaking data for others", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
	protected ref array<EEditableEntityLabel> APCALabels;
	
	bool respawning = false;
	int playerID = -1;
	
	void SetRespawning(bool value)
	{
		respawning = value;
	}
	
	bool GetRespawning()
	{
		return respawning;
	}
	
	void SetPlayerID(int value)
	{
		playerID = value;
	}
	
	int GetPlayerID()
	{
		return playerID;
	}
	
	array<EEditableEntityLabel> GetAPCALabels()
	{
		return APCALabels;
	}
}
