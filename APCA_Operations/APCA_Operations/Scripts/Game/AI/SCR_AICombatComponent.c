  /* ------------------------------------------------------------------------------------------------ /
 / APCA Modded Base Class										                                      /
/ ------------------------------------------------------------------------------------------------ */

//------------------------------------------------------------------------------------------------
modded class SCR_AICombatComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	override bool DismountTurretCondition(inout vector targetPos, bool targetPosProvided)
	{
		return false;
	}
};