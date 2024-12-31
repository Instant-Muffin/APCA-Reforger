[ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Plays sound event", color: "0 0 255 255")]
class APCA_SoundComponentClass: ScriptComponentClass
{
}

class APCA_SoundComponent: ScriptComponent
{
	[Attribute(desc: "Select sound event")]
	string soundName;
	
	override void OnPostInit(IEntity owner) 
	{
		owner.SetFlags(EntityFlags.ACTIVE, false);
		SetEventMask(owner, EntityEvent.INIT);
	};

	override void EOnInit(IEntity owner) 
	{
		if (SCR_Global.IsEditMode(owner))
			return;
		
		SoundComponent soundComponent = SoundComponent.Cast(owner.FindComponent(SoundComponent));
        if (soundComponent) {
            soundComponent.SoundEvent( soundName );
		};
	};
}