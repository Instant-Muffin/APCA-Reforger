
class MFN_IdentitySelector : MenuBase
{
	ref SharedItemRef m_World;
	protected RenderTargetWidget PreviewWindow;
	protected OverlayWidget IdentityListOverlay;
	protected SCR_ListBoxComponent IdentityList;
    protected SCR_InputButtonComponent ExitBtn;
    protected SCR_InputButtonComponent ApplyBtn;
	protected CheckBoxWidget PersistenceEnable;
	protected ref array<ref VisualIdentity> VisualIdentities;
	IEntity PreviewBody;
	IEntity PreviewHead;
	
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		PlayerController controller = PlayerController.Cast(GetGame().GetPlayerController());
		if(!controller)
			return;
		SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(controller.GetControlledEntity());
		if(!playerCharacter)
			return;
		
		CharacterIdentityComponent IDComponent = CharacterIdentityComponent.Cast(playerCharacter.FindComponent(CharacterIdentityComponent));
		Identity ID = IDComponent.GetIdentity();
		
		// Set up Apply Button
		ApplyBtn = SCR_InputButtonComponent.GetInputButtonComponent("ApplyButton", GetRootWidget());
		if (ApplyBtn)
        {
            GetGame().GetWorkspace().SetFocusedWidget(ApplyBtn.GetRootWidget());            
            ApplyBtn.m_OnActivated.Insert(ApplySelectedIdentity);
        } 
		
		
		// Set up Exit Button
		ExitBtn = SCR_InputButtonComponent.GetInputButtonComponent("ExitButton", GetRootWidget());
		if (ExitBtn)
        {
            GetGame().GetWorkspace().SetFocusedWidget(ExitBtn.GetRootWidget());      
			ExitBtn.m_OnActivated.Insert(ExitMenu);      
            
        } 
		
		
		// Set up Persistence switch
		PersistenceEnable = CheckBoxWidget.Cast(GetRootWidget().FindAnyWidget("PersistenceEnable"));
		
		// Load current settings
		LoadIdentityData();
		
		
		// Set up RT
		StartPreview();
		
		
		// Set up List
		IdentityListOverlay = OverlayWidget.Cast(GetRootWidget().FindAnyWidget("IdentityListBox"));
		IdentityList = SCR_ListBoxComponent.Cast(IdentityListOverlay.FindHandler(SCR_ListBoxComponent));
		if(IdentityList)
		{
			if(!VisualIdentities)
			{
				SCR_Faction playerFaction = SCR_Faction.Cast(playerCharacter.GetFaction());
				VisualIdentities = {};
				playerFaction.GetFactionIdentity().GetVisualIdentities(VisualIdentities);
			}
			
			for(int i = 0; i < VisualIdentities.Count(); i++)
			{
				bool isCurrentID = false;
				string fullName = VisualIdentities[i].GetHead().GetPath();
				string newString = "";
				if(fullName == ID.GetVisualIdentity().GetHead().GetPath())
				{
					isCurrentID = true;
					newString = " * ";
				}
				array<string> pairs = new array<string>;
				fullName.Split("/", pairs, true);
				pairs[pairs.Count() - 1].Split(".", pairs, true);
				newString = newString + pairs[0];
				IdentityList.AddItem(newString); 
				
				if(isCurrentID)
				{
					IdentityList.SetItemSelected(i, true);		
					UpdatePreview();		
				};
			}
			
			IdentityList.m_OnChanged.Insert(UpdatePreview);
		}
	};
	
	void ExitMenu()
	{
		GetManager().CloseMenu(this);
	}
	
	void ApplySelectedIdentity()
	{
		if(!IdentityList || (VisualIdentities.Count() == 0))
			return;
		
		int selectedIndex = IdentityList.GetSelectedItem();
		SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity());
		CharacterIdentityComponent IDComponent = CharacterIdentityComponent.Cast(playerCharacter.FindComponent(CharacterIdentityComponent));
		Identity ID = IDComponent.GetIdentity();
		ID.SetVisualIdentity(VisualIdentities[selectedIndex]);
		
		SaveIdentity(ID, PersistenceEnable.IsChecked());
		
		MFN_IdentitySelectorCharacterComponent IDComp = MFN_IdentitySelectorCharacterComponent.Cast(playerCharacter.FindComponent(MFN_IdentitySelectorCharacterComponent));
		if(IDComp)
		{
			Print("Loading Identity");
			IDComp.LoadIdentity(true);
		}
		
		ExitMenu();
	};
	
	void StartPreview()
	{
		PreviewWindow = RenderTargetWidget.Cast(GetRootWidget().FindAnyWidget("PreviewWindow"));
		m_World = BaseWorld.CreateWorld("Preview", "Preview");
		BaseWorld world = m_World.GetRef();
		PreviewWindow.Update();
		PreviewWindow.SetWorld(world, 0);
		
		// create generic world
		Resource rsc = Resource.Load("{E91350F79536248F}Prefabs/PreviewWorld.et");	
		if (rsc.IsValid())
			GetGame().SpawnEntityPrefab(rsc, world);
		
		// setup cam
		world.SetCamera(0, "0 1.75 -0.58", "0 -8 0");
		world.SetCameraType(0, CameraType.ORTHOGRAPHIC);
		world.SetCameraNearPlane(0, 0.1);
		world.SetCameraFarPlane(0, 4000);
		world.SetCameraVerticalFOV(0, 30);
	}
	
	void UpdatePreview()
	{
		if (PreviewHead)
			delete PreviewHead;
		
		if (PreviewBody)
			delete PreviewBody;
		
		BaseWorld world = m_World.GetRef();	
		int selectedIndex = IdentityList.GetSelectedItem();
		Resource rsc = Resource.Load(VisualIdentities[selectedIndex].GetHead());		
		if (rsc.IsValid())
		{
			PreviewHead = GetGame().SpawnEntityPrefab(rsc, world);
			vector angles = PreviewHead.GetAngles();
			angles[1] = -160;
			PreviewHead.SetAngles(angles);
			PreviewHead.SetOrigin("0 0 0");
			
			rsc = Resource.Load(VisualIdentities[selectedIndex].GetBody());	
			PreviewBody = GetGame().SpawnEntityPrefab(rsc, world);
			PreviewBody.SetAngles(angles);
			PreviewBody.SetOrigin("0 0 0");
		}
	}
	
	void SaveIdentity(Identity data, bool persist)
	{
		Print("Saving Identity");
		/*private string path = "$profile:/IdentitySelector/";
		FileIO.MakeDirectory(path);
		FileHandle handle = FileIO.OpenFile(path + "Identity", FileMode.WRITE);
		if (!handle)
		{
			Print("Failed to save identity file", LogLevel.ERROR);
			return;
		};
		
		handle.WriteLine(data.GetVisualIdentity().GetHead().GetPath());
		handle.WriteLine(data.GetVisualIdentity().GetBody().GetPath());
		handle.WriteLine(persist.ToString());
		handle.Close();*/
		
		MFN_IDentityStruct identityData = new MFN_IDentityStruct();
		private string path = "$profile:/IdentitySelector/";
		FileIO.MakeDirectory(path);
		
		identityData.head = data.GetVisualIdentity().GetHead().GetPath();
		identityData.body = data.GetVisualIdentity().GetBody().GetPath();
		identityData.persist = persist.ToString();
		identityData.Pack();
		identityData.SaveToFile(path+ "Identity.json");
	}
	
	void LoadIdentityData()
	{
		MFN_IDentityStruct identityData = new MFN_IDentityStruct();
		private string path = "$profile:/IdentitySelector/";
		identityData.LoadFromFile(path + "Identity.json");
		if (!identityData)
		{
			Print("Failed to load identity file", LogLevel.WARNING);
			return;
		};
		
		SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity());
		SCR_Faction playerFaction = SCR_Faction.Cast(playerCharacter.GetFaction());
		VisualIdentities = {};
		playerFaction.GetFactionIdentity().GetVisualIdentities(VisualIdentities);

		if(identityData.persist == "true")
			PersistenceEnable.SetChecked(true);
		else
			PersistenceEnable.SetChecked(false);
	};
	
};

class MFN_IdentitySelectorComponent: ScriptedWidgetComponent {}

modded enum ChimeraMenuPreset
{
	MFN_IdentitySelector
}