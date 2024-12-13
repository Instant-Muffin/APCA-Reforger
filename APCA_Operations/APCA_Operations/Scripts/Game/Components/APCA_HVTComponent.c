[ComponentEditorProps(category: "APCA/HVT", description: "TODO")]
class APCA_HVTComponentClass : ScriptComponentClass
{
}

class APCA_HVTComponent : ScriptComponent
{
	[Attribute(defvalue: "true", desc: "Allow Following", category: "HVT")]
	protected bool m_willFollow;
	protected float m_fCheckDelay;
	protected static const float CHECK_PERIOD = 10;
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		m_fCheckDelay -= timeSlice;
		if (m_fCheckDelay <= 0)
		{
			m_fCheckDelay = CHECK_PERIOD;
			// Do stuff here
			
		};
	}
	
	protected override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		//Enables EOnFrame 
		SetEventMask(owner, EntityEvent.FRAME);
	}
}

