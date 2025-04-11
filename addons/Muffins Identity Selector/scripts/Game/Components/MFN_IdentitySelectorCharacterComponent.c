class MFN_IdentitySelectorCharacterComponentClass : ScriptComponentClass
{
}

class MFN_IdentitySelectorCharacterComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void LoadIdentity(bool forceLoad)
	{
		IEntity owner = IEntity.Cast(GetOwner());
		IEntity playerEntity = GetGame().GetPlayerController().GetControlledEntity();
		if(playerEntity != owner)
			return;
		
		Print("Loading Entity from: $profile:/IdentitySelector/Identity");
		MFN_IDentityStruct identityData = new MFN_IDentityStruct();
		identityData.LoadFromFile("$profile:/IdentitySelector/Identity.json");
		if (!identityData)
		{
			Print("Failed to load identity file", LogLevel.WARNING);
			return;
		};
		PrintFormat("Got ID Data: %1 %2", identityData.head, identityData.body);
		
		
		if(identityData.persist == "true" || forceLoad == true)
		{
			Print("Looking for ID");
			SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(playerEntity);
			
			if(!playerCharacter)
				return;
			
			SCR_Faction playerFaction = SCR_Faction.Cast(playerCharacter.GetFaction());
			
			if(!playerFaction)
				return;
			
			array<ref VisualIdentity> VisualIdentities = {};
			playerFaction.GetFactionIdentity().GetVisualIdentities(VisualIdentities);
			
			foreach(VisualIdentity ID : VisualIdentities)
			{
				if((identityData.head == ID.GetHead().GetPath()) && (identityData.body == ID.GetBody().GetPath()))
				{
					PrintFormat("Telling server to set ID: %1 %2", ID.GetHead(), ID.GetBody());
					Rpc(ServerSetIdentity, ID.GetHead(), ID.GetBody());
					return;
				}
			}
		}
	}
	
	void RemoteLoadIdentity(bool forceLoad)
	{
		IEntity owner = IEntity.Cast(GetOwner());
		IEntity playerEntity = GetGame().GetPlayerController().GetControlledEntity();
		if(playerEntity != owner)
			Rpc(LoadIdentity, forceLoad);
		else
			LoadIdentity(forceLoad);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void ServerSetIdentity(string head, string body)
	{
		PrintFormat("Server is Setting: %1 %2", head, body);
	
		SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(GetOwner());
		CharacterIdentityComponent IDComponent = CharacterIdentityComponent.Cast(playerCharacter.FindComponent(CharacterIdentityComponent));
		Identity playerID = IDComponent.GetIdentity();
		VisualIdentity newVisID = playerID.GetVisualIdentity();
		newVisID.SetHead(head);
		newVisID.SetBody(body);
		playerID.SetVisualIdentity(newVisID);
		IDComponent.SetIdentity(playerID);
	}
}