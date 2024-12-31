[ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Plays sound event", color: "0 0 255 255")]
class APCA_SoundFirefightComponentClass: ScriptComponentClass
{
}

class APCA_SoundFirefightComponent: ScriptComponent
{
	[Attribute(desc: "Sound events")]
	ref array<string> soundNames;
	
	[Attribute(desc: "Minimum Delay")]
	float minDelay;
	
	[Attribute(desc: "Maximum Delay")]
	float maxDelay;
	
	[Attribute(desc: "Shooting profiles")]
	ref array<int> profiles;
	
	SoundComponent soundComponent;
	ref array<float> timingList;
	ref array<string> soundList;
	float delay;
	float checkRate;
	float checkDelay;
	
	
	override void OnPostInit(IEntity owner) 
	{
		owner.SetFlags(EntityFlags.ACTIVE, false);
		SetEventMask(owner, EntityEvent.INIT  | EntityEvent.FRAME);
	};

	override void EOnInit(IEntity owner) 
	{
		if (SCR_Global.IsEditMode(owner))
			return;
        
		timingList = {};
		soundList = {};
		delay = 0;
		checkDelay = 0;
		checkRate = 0.12;
		soundComponent = SoundComponent.Cast(owner.FindComponent(SoundComponent));
		
	};
	
	override void EOnFrame(IEntity owner, float timeSlice)
    {
        super.EOnFrame(owner, timeSlice);
		checkDelay = checkDelay + timeSlice;
		delay = delay + timeSlice;
		if(checkDelay > checkRate) {
			UpdateSounds();
			checkDelay = 0;
		}
	}
	
	void ScheduleSound(string sound, int time)
	{
		soundList.Insert(sound);
		timingList.Insert(time);
	}
	
	void UpdateSounds()
	{	
		if (soundComponent) 
		{
			
			if(delay > minDelay)
			{
				// Add a new shooting profile
				int profile = profiles.GetRandomElement();
				switch(profile)
				{
					case 0: // Single
					{
						string sound = soundNames.GetRandomElement();
						float time = GetGame().GetWorld().GetWorldTime() + Math.RandomFloat(0.01, maxDelay) - minDelay;
						ScheduleSound(sound, time);
						break;
					}
					case 1: // Short Slow Burst
					{
						int quantity = Math.RandomInt(3,6);
						string sound = soundNames.GetRandomElement();
						float time = GetGame().GetWorld().GetWorldTime() + Math.RandomFloat(0.01, maxDelay) - minDelay;
						for(int i = 0; i < quantity; i++)
						{
							ScheduleSound(sound, time);
							time = time + 300;
						}
						break;
					}
					case 2: // Short Fast Burst
					{
						int quantity = Math.RandomInt(3,5);
						string sound = soundNames.GetRandomElement();
						float time = GetGame().GetWorld().GetWorldTime() + Math.RandomFloat(0.01, maxDelay) - minDelay;
						for(int i = 0; i < quantity; i++)
						{
							ScheduleSound(sound, time);
							time = time + 120;
						}
						break;
					}
					case 3: // Long Burst
					{
						int quantity = Math.RandomInt(5,8);
						string sound = soundNames.GetRandomElement();
						float time = GetGame().GetWorld().GetWorldTime() + Math.RandomFloat(0.01, maxDelay) - minDelay;
						for(int i = 0; i < quantity; i++)
						{
							ScheduleSound(sound, time);
							time = time + 120;
						}
						break;
					}
					case 4: // Machine Gun
					{
						int quantity = Math.RandomInt(10,20);
						string sound = soundNames.GetRandomElement();
						float time = GetGame().GetWorld().GetWorldTime() + Math.RandomFloat(0.01, maxDelay) - minDelay;
						for(int i = 0; i < quantity; i++)
						{
							ScheduleSound(sound, time);
							time = time + 120;
						}
						break;
					}
				}
				
				delay = 0;
			}
			
			for(int i = 0; i < timingList.Count(); i++)
			{
				float time = GetGame().GetWorld().GetWorldTime();
				if(timingList[i] < time)
				{
					soundComponent.SoundEvent( soundList[i] );
					timingList.Remove(i);
					soundList.Remove(i);
				}
			}
			/*
			ShootingProfile
			0 -	SINGLE,
			1 -	SHORT_BURST,
			2 -	LONG_BURST,
			3 -	MACHINE_GUN
			*/
		}
	}

}