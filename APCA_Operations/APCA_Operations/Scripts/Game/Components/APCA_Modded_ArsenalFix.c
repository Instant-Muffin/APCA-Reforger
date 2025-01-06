modded class SCR_ArsenalManagerComponent : SCR_BaseGameModeComponent
{
	override void SetPlayerArsenalLoadout(int playerId, GameEntity characterEntity, SCR_ArsenalComponent arsenalComponent, SCR_EArsenalSupplyCostType arsenalSupplyType)
	{
		//~ If Not Authority return
		if (!GetGameMode().IsMaster())
			return;
		
		string playerUID = GetGame().GetBackendApi().GetPlayerIdentityId(playerId);
		if (!characterEntity)
		{
			DoSetPlayerLoadout(playerId, string.Empty, characterEntity, arsenalSupplyType);
			return;
		}
		
		SCR_PlayerController clientPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!clientPlayerController || clientPlayerController.IsPossessing())
			return;
		
		string factionKey = SCR_PlayerArsenalLoadout.ARSENALLOADOUT_FACTIONKEY_NONE;
		FactionAffiliationComponent factionAffiliation = FactionAffiliationComponent.Cast(characterEntity.FindComponent(FactionAffiliationComponent));
		if (factionAffiliation)
			factionKey = factionAffiliation.GetAffiliatedFaction().GetFactionKey();
		
		if (!CanSaveLoadout(playerId, characterEntity, factionAffiliation, arsenalComponent, true))
			return;
		
		SCR_JsonSaveContext context = new SCR_JsonSaveContext();
		if (!context.WriteValue(SCR_PlayerArsenalLoadout.ARSENALLOADOUT_FACTION_KEY, factionKey) || !context.WriteValue(SCR_PlayerArsenalLoadout.ARSENALLOADOUT_KEY, characterEntity))
			return;
		Print("APCA Tester");
		
		DoSetPlayerLoadout(playerId, context.ExportToString(), characterEntity, arsenalSupplyType);
	}
}