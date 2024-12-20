class APCA_BaseComponentClass: GameComponentClass
{
}

class APCA_BaseComponent: GameComponent
{
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
}
