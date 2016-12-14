#include "engine\game\editor\zone\ZoneEditor.h"
#include "engine\game\map\zone\ZoneMap.h"

void ZoneEditor::init()
{
	m_map = new ZoneMap(Vector2<Uint16>(64, 64));

	m_map->setTextureGround(LTexture::getInstance().getImage("TilesheetGround.png"));
	m_map->setTextureWorld(LTexture::getInstance().getImage("TilesheetWorld.png"));
	m_map->setTextureSky(LTexture::getInstance().getImage("TilesheetSky.png"));

	Editor::init();

	m_guiLeftLayer->addComponent(new CButton("BUTTON_EDITOR_TYPE", "", LTexture::getInstance().getImage("gui\\BoardIcon.png"), {0, -15}, {24, 24}, 1), PANEL_ALIGN_CENTER)->setTooltip("Switch to Battlefield editor");

	m_guiWorldSwitch = new Container("GUI_SWITCH", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldSolidSwitch = new Container("GUI_SOLID_SWITCH", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldPortal = new Container("GUI_PORTAL", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldDirection = new Container("GUI_DIRECTION", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);

	m_guiWorldSwitch->addComponent(new CSlider("SLIDER_FREQUENCY", "Frequency", {0, 0}, 252, 255, 0), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_guiWorldSwitch->findComponent("NUMFIELD_FREQUENCY")->setValue(ZoneEditor::getInstance().m_guiWorldSwitch->findComponent("SLIDER_FREQUENCY")->getValue());
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_frequency = ZoneEditor::getInstance().m_guiWorldSwitch->findComponent("SLIDER_FREQUENCY")->getValue();
	})->setTooltip("Frequency of switch - which other switches\n or portals it aligns with.\nFreq. 0 means deactive.");
	m_guiWorldSwitch->addComponent(new NumberField("NUMFIELD_FREQUENCY", "", {0, 18}, {252, 1}, {0, 255}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_guiWorldSwitch->findComponent("SLIDER_FREQUENCY")->setValue(ZoneEditor::getInstance().m_guiWorldSwitch->findComponent("NUMFIELD_FREQUENCY")->getValue());
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_frequency = ZoneEditor::getInstance().m_guiWorldSwitch->findComponent("SLIDER_FREQUENCY")->getValue();
	});

	m_guiWorldSolidSwitch->addComponent(new CSlider("SLIDER_FREQUENCY", "Frequency", {0, 0}, 252, 255, 0), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_guiWorldSolidSwitch->findComponent("NUMFIELD_FREQUENCY")->setValue(ZoneEditor::getInstance().m_guiWorldSolidSwitch->findComponent("SLIDER_FREQUENCY")->getValue());
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_frequency = ZoneEditor::getInstance().m_guiWorldSolidSwitch->findComponent("SLIDER_FREQUENCY")->getValue();
	})->setTooltip("Frequency of solid - which switch needs\n to be active to pass.\nFreq. 0 means solid.");
	m_guiWorldSolidSwitch->addComponent(new NumberField("NUMFIELD_FREQUENCY", "", {0, 18}, {252, 1}, {0, 255}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_guiWorldSolidSwitch->findComponent("SLIDER_FREQUENCY")->setValue(ZoneEditor::getInstance().m_guiWorldSolidSwitch->findComponent("NUMFIELD_FREQUENCY")->getValue());
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_frequency = ZoneEditor::getInstance().m_guiWorldSolidSwitch->findComponent("SLIDER_FREQUENCY")->getValue();
	});

	m_guiWorldPortal->addComponent(new CSlider("SLIDER_FREQUENCY", "Frequency", {0, 0}, 252, 255, 0), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_guiWorldPortal->findComponent("NUMFIELD_FREQUENCY")->setValue(ZoneEditor::getInstance().m_guiWorldPortal->findComponent("SLIDER_FREQUENCY")->getValue());
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_frequency = ZoneEditor::getInstance().m_guiWorldPortal->findComponent("SLIDER_FREQUENCY")->getValue();
	})->setTooltip("Frequency of portal - which switch needs\n to be active to use.\nFreq. 0 means always active.");
	m_guiWorldPortal->addComponent(new NumberField("NUMFIELD_FREQUENCY", "", {0, 18}, {252, 1}, {0, 255}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_guiWorldPortal->findComponent("SLIDER_FREQUENCY")->setValue(ZoneEditor::getInstance().m_guiWorldPortal->findComponent("NUMFIELD_FREQUENCY")->getValue());
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_frequency = ZoneEditor::getInstance().m_guiWorldPortal->findComponent("SLIDER_FREQUENCY")->getValue();
	});

	m_guiWorldPortal->addComponent(new CText("TEXT_POS", "Destination", {0, 58}, {200, 24}, ALIGN_CENTER, Color(24, 24, 24, 255)), PANEL_ALIGN_CENTER);
	m_guiWorldPortal->addComponent(new TextField("TEXTFIELD_NAME", "Zone Name", {0, 66}, {252, 1}, 1), PANEL_ALIGN_CENTER)->setFunction([]() 
	{
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_portalDest = ZoneEditor::getInstance().m_guiWorldPortal->findComponent("TEXTFIELD_NAME")->getTitle();
	})->setTooltip("Leave blank for current zone.");
	m_guiWorldPortal->addComponent(new NumberField("NUMFIELD_X", "X:", {0, 96}, {92, 1}, {0, 10000}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_destX = ZoneEditor::getInstance().m_guiWorldPortal->findComponent("NUMFIELD_X")->getValue();
	});
	m_guiWorldPortal->addComponent(new NumberField("NUMFIELD_Y", "Y:", {0, 120}, {92, 1}, {0, 10000}, 0), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_destY = ZoneEditor::getInstance().m_guiWorldPortal->findComponent("NUMFIELD_Y")->getValue();
	});

	m_guiWorldDirection->addComponent(new CDropDown("DROPDOWN_DIRECTION", "Direction", {0, 0}, {264, 32}, 1), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_direction = ZoneEditor::getInstance().m_guiWorldDirection->findComponent("DROPDOWN_DIRECTION")->getSelectedItem();
	})->setTooltip("Which direction the unit is pushed when stepping on this tile");
	m_guiWorldDirection->findComponent("DROPDOWN_DIRECTION")->addItem("Up")->addItem("Right")->addItem("Down")->addItem("Left");
	m_guiWorldDirection->updateSize();

	m_guiWorld->addComponent(m_guiWorldSwitch, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldSolidSwitch, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldPortal, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldDirection, PANEL_ALIGN_CENTER);

	m_guiWorld->findComponent("DROPDOWN_INTERACT")->setFunction([]()
	{
		Sint16 _sel = ZoneEditor::getInstance().m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem();
		Sint16 _pSel = ZoneEditor::getInstance().m_guiWorld->findComponent("DROPDOWN_INTERACT")->getPrevSelectedItem();
		if(_sel != _pSel)
		{
			switch(_sel)
			{
			case 2: // Switch
				ZoneEditor::getInstance().m_guiWorldSwitch->setVisible(true);
				break;
			case 3: // Solid Switch
				ZoneEditor::getInstance().m_guiWorldSolidSwitch->setVisible(true);
				break;
			case 4: // Portal
				ZoneEditor::getInstance().m_guiWorldPortal->setVisible(true);
				break;
			case 5: // Directional
				ZoneEditor::getInstance().m_guiWorldDirection->setVisible(true);
				break;
			}
			switch(_pSel)
			{
			case 2: // Switch
				ZoneEditor::getInstance().m_guiWorldSwitch->setVisible(false);
				break;
			case 3: // Solid Switch
				ZoneEditor::getInstance().m_guiWorldSolidSwitch->setVisible(false);
				break;
			case 4: // Portal
				ZoneEditor::getInstance().m_guiWorldPortal->setVisible(false);
				break;
			case 5: // Directional
				ZoneEditor::getInstance().m_guiWorldDirection->setVisible(false);
				break;
			}
		}
		ZoneEditor::getInstance().m_guiWorld->calcSize();
	});
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("None")->addItem("Solid")->addItem("Switch")->addItem("Solid Switch")->addItem("Portal")->addItem("Directional");
	m_guiWorld->calcSize();

	m_toolbarMenu->addButton("", "File");
	{
		m_toolbarMenu->addButton("File", "Save", []() {
			ZoneEditor::getInstance().m_guiSaveMap->findComponent("TEXTFIELD_WORLD_NAME")->setState(1);
			ZoneEditor::getInstance().m_guiSaveMap->setVisible(true);
			ZoneEditor::getInstance().m_guiPause->setVisible(true);
			ZoneEditor::getInstance().pause("CONTAINER_SAVE_MAP");
		});
		m_toolbarMenu->addButton("File", "Load", []() {
			ZoneEditor::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->setState(1);
			ZoneEditor::getInstance().m_guiLoadMap->setVisible(true);
			ZoneEditor::getInstance().m_guiPause->setVisible(true);
			ZoneEditor::getInstance().pause("CONTAINER_LOAD_MAP");
		});
		m_toolbarMenu->addButton("File", "New", []() {
			ZoneEditor::getInstance().m_guiClearMap->setVisible(true);
			ZoneEditor::getInstance().m_guiPause->setVisible(true);
			ZoneEditor::getInstance().pause("CONTAINER_CLEAR_MAP");
		});
		m_toolbarMenu->addButton("File", "Exit", []() {
			ZoneEditor::getInstance().m_guiExit->findComponent("TEXTFIELD_WORLD_NAME")->setState(1);
			Globals::getInstance().m_exitting = 1;
			ZoneEditor::getInstance().pause("CONTAINER_EXIT");
		});
	}
	m_toolbarMenu->addButton("", "Edit");
	{
		m_toolbarMenu->addButton("Edit", "Resize", []() {
			ZoneEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_WIDTH")->setValue(ZoneEditor::getInstance().m_map->getSize().x);
			ZoneEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_HEIGHT")->setValue(ZoneEditor::getInstance().m_map->getSize().y);
			ZoneEditor::getInstance().pause("CONTAINER_RESIZE_MAP");
		});
	}
	m_toolbarMenu->addButton("", "View");
	{
		m_toolbarMenu->addButton("View", "Toggle Grid", []() {
			ZoneEditor::getInstance().m_showGrid = !ZoneEditor::getInstance().m_showGrid;
		});
		m_toolbarMenu->addButton("View", "Toggle Overlay", []() {
			ZoneEditor::getInstance().m_guiLeft->setVisible(!ZoneEditor::getInstance().m_guiLeft->isVisible());
		});
	}
	m_toolbarMenu->addButton("", "Help");
	{
		m_toolbarMenu->addButton("Help", "No help yet :(");
	}
	m_guiTop->addComponent(m_toolbarMenu);



	m_guiLeftLayer->addComponent(new CButtonToggle("BUTTON_GROUND_VISIBLE", LTexture::getInstance().getImage("gui\\Visible.png"),
		LTexture::getInstance().getImage("gui\\Invisible.png"), {-48, 4}, {24, 24}, 1, 1, []()
	{ ZoneEditor::getInstance().m_map->setLayerVisible(0, ZoneEditor::getInstance().m_guiLeftLayer->findComponent("BUTTON_GROUND_VISIBLE")->isSelected() != 0); }), PANEL_ALIGN_TOP);
	m_guiLeftLayer->addComponent(new CButtonToggle("BUTTON_WORLD_VISIBLE", LTexture::getInstance().getImage("gui\\Visible.png"),
		LTexture::getInstance().getImage("gui\\Invisible.png"), {48, 4}, {24, 24}, 1, 1, []()
	{ ZoneEditor::getInstance().m_map->setLayerVisible(1, ZoneEditor::getInstance().m_guiLeftLayer->findComponent("BUTTON_WORLD_VISIBLE")->isSelected() != 0); }), PANEL_ALIGN_TOP);
	m_guiLeftLayer->addComponent(new CButtonToggle("BUTTON_ENTITY_VISIBLE", LTexture::getInstance().getImage("gui\\Visible.png"),
		LTexture::getInstance().getImage("gui\\Invisible.png"), {-48, 34}, {24, 24}, 1, 1, []()
	{ ZoneEditor::getInstance().m_map->setLayerVisible(2, ZoneEditor::getInstance().m_guiLeftLayer->findComponent("BUTTON_ENTITY_VISIBLE")->isSelected() != 0); }), PANEL_ALIGN_TOP);
	m_guiLeftLayer->addComponent(new CButtonToggle("BUTTON_SKY_VISIBLE", LTexture::getInstance().getImage("gui\\Visible.png"),
		LTexture::getInstance().getImage("gui\\Invisible.png"), {48, 34}, {24, 24}, 1, 1, []()
	{ ZoneEditor::getInstance().m_map->setLayerVisible(3, ZoneEditor::getInstance().m_guiLeftLayer->findComponent("BUTTON_SKY_VISIBLE")->isSelected() != 0); }), PANEL_ALIGN_TOP);



	m_guiEntityTex = new Container("CONTAINER_ENTITY_TEXTURE", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityTex->addComponent(new Panel("PANEL_BG2", "Set Texture", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityTex->addComponent(new TextField("TEXTFIELD_TEXTURE", "Directory res\\texture\\", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);
		m_guiEntityTex->addComponent(new CButton("BUTTON_SET", "Set", {-130, 24}, {252, 24}, 1, []() {
			if(ZoneEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->getTitle() != "")
			{
				ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex = LTexture::getInstance().getImage(ZoneEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->getTitle());
				ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex.setName(ZoneEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->getTitle());
				ZoneEditor::getInstance().m_tileSetEntity->setTileSheet(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex);
				ZoneEditor::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiEntityTex->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiEntityInteract = new Container("CONTAINER_ENTITY_INTERACT_SCRIPT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityInteract->addComponent(new Panel("PANEL_BG2", "Interact Script", {0, -12}, Globals::getInstance().m_screenSize - Vector2<Sint32>(256, 128), 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityInteract->addComponent(new TextField("TEXTFIELD_SCRIPT", "Go to docs folder for interact script api", {0, 0}, Vector2<Sint32>(Globals::getInstance().m_screenSize.x - 256, Globals::getInstance().m_screenSize.y / 16 - 14), 1, false), PANEL_ALIGN_CENTER);
		m_guiEntityInteract->addComponent(new CButton("BUTTON_SET", "Save", {-130, 24}, {252, 24}, 1, []() {
			ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact = ZoneEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->getTitle();
			ZoneEditor::getInstance().m_tileSetEntity->setTileSheet(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex);
			ZoneEditor::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityInteract->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}
	m_guiEntityIdle = new Container("CONTAINER_ENTITY_IDLE_SCRIPT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityIdle->addComponent(new Panel("PANEL_BG2", "Idle Script", {0, -12}, Globals::getInstance().m_screenSize - Vector2<Sint32>(256, 128), 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityIdle->addComponent(new TextField("TEXTFIELD_SCRIPT", "Go to docs folder for interact script api", {0, 0}, Globals::getInstance().m_screenSize / 16 - Vector2<Sint32>(512, 21), 1, false), PANEL_ALIGN_CENTER);
		m_guiEntityIdle->addComponent(new CButton("BUTTON_SET", "Save", {-130, 24}, {252, 24}, 1, []() {
			ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact = ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->getTitle();
			ZoneEditor::getInstance().m_tileSetEntity->setTileSheet(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex);
			ZoneEditor::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityIdle->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}
	/*
	m_guiEntityBoard = new Container("CONTAINER_ENTITY_BOARD", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
	m_guiEntityBoard->addComponent(new Panel("PANEL_BG2", "Set Board", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
	m_guiEntityBoard->addComponent(new TextField("TEXTFIELD_BOARD", "Directory map\\boards\\", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);

	m_guiEntityBoard->addComponent(new CButton("BUTTON_SET2", "Set", {-130, 24}, {252, 24}, 1, []() {
	ZoneEditor::getInstance().unpause();
	}), PANEL_ALIGN_CENTER);
	m_guiEntityBoard->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	*/

	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_TEXTURE", "Set Spritesheet", Vector2<Sint32>(0, 362), Vector2<Sint32>(264, 32), 1, []()
	{
		ZoneEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->setState(1);
		ZoneEditor::getInstance().pause("CONTAINER_ENTITY_TEXTURE");
		ZoneEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->setTitle(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex.getName());
	}), PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_INTERACT_SCRIPT", "Edit Interaction Script", Vector2<Sint32>(0, 466), Vector2<Sint32>(264, 32), 1, []()
	{
		ZoneEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->setState(1);
		ZoneEditor::getInstance().pause("CONTAINER_ENTITY_INTERACT_SCRIPT");
		ZoneEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->setTitle(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact);
		ZoneEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->setState(1);
	}), PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_IDLE_SCRIPT", "Edit Idle Script", Vector2<Sint32>(0, 502), Vector2<Sint32>(264, 32), 1, []()
	{
		ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setState(1);
		ZoneEditor::getInstance().pause("CONTAINER_ENTITY_INTERACT_SCRIPT");
		ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setTitle(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact);
		ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setState(1);
	}), PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_SET_BOARD", "Set Board", Vector2<Sint32>(0, 538), Vector2<Sint32>(264, 32), 1, []()
	{
		ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setState(1);
		ZoneEditor::getInstance().pause("CONTAINER_ENTITY_INTERACT_SCRIPT");
		ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setTitle(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact);
		ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setState(1);
	}), PANEL_ALIGN_TOP);

	m_guiEntity->findComponent("DROPDOWN_TYPE")->addItem("Neutral")->addItem("Opponent")->addItem("Trader");
	


	m_guiSaveMap = new Container("CONTAINER_SAVE_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiSaveMap->addComponent(new Panel("PANEL_BG2", "Save ", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiSaveMap->addComponent(new TextField("TEXTFIELD_WORLD_NAME", "Save to maps\\zones\\<input>", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);
		m_guiSaveMap->addComponent(new CButton("BUTTON_SET", "Save", {-130, 24}, {252, 24}, 1, []() {
			if(ZoneEditor::getInstance().m_guiSaveMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				ZoneEditor::getInstance().m_map->save(ZoneEditor::getInstance().m_guiSaveMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				ZoneEditor::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiSaveMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiLoadMap = new Container("CONTAINER_LOAD_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiLoadMap->addComponent(new Panel("PANEL_BG2", "Load ", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new TextField("TEXTFIELD_WORLD_NAME", "Load from maps\\zones\\<input>", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new CButton("BUTTON_SET", "Load", {-130, 24}, {252, 24}, 1, []() {
			if(ZoneEditor::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				ZoneEditor::getInstance().m_map->load(ZoneEditor::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				ZoneEditor::getInstance().m_listWorld->clear();
				for(Uint16 i = 0; i < ZoneEditor::getInstance().m_map->getWorldObjectSize(); i++)
					ZoneEditor::getInstance().m_listWorld->addItem({ZoneEditor::getInstance().m_map->getWorldObject(i).m_name, ZoneEditor::getInstance().m_map->getWorldObject(i).m_tileTex});
				ZoneEditor::getInstance().m_tabEntity->clear();
				for(Uint16 i = 0; i < ZoneEditor::getInstance().m_map->getEntitySize(); i++)
					ZoneEditor::getInstance().m_tabEntity->addItem({ZoneEditor::getInstance().m_map->getEntity(i).m_name, 0});
				ZoneEditor::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiClearMap = new Container("CONTAINER_CLEAR_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiClearMap->addComponent(new Panel("PANEL_BG2", "Clear  (Warning: No Undo)", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);

		m_guiClearMap->addComponent(new CButton("BUTTON_SET", "Clear", {-130, 24}, {252, 24}, 1, []() {
			ZoneEditor::getInstance().m_map->createNew();
			ZoneEditor::getInstance().unpause();
		}), PANEL_ALIGN_CENTER);
		m_guiClearMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiResizeMap = new Container("CONTAINER_RESIZE_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiResizeMap->addComponent(new Panel("PANEL_BG2", "Resize ", {0, -12}, {1220, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CSlider("SLIDER_WIDTH", "Width", {0, -16}, 1024, 1024, 10, 1), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CSlider("SLIDER_HEIGHT", "Height", {0, 4}, 1024, 1024, 10, 1), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CButton("BUTTON_SET", "Resize", {-130, 24}, {252, 24}, 1, []() {
			if(ZoneEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_WIDTH")->getValue() != 0 && ZoneEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_HEIGHT")->getValue() != 0)
			{
				ZoneEditor::getInstance().m_map->resize(Vector2<Uint16>(ZoneEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_WIDTH")->getValue(), ZoneEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_HEIGHT")->getValue()));
				ZoneEditor::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiExit = new Container("CONTAINER_EXIT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiExit->addComponent(new Panel("PANEL_BG2", "Exit ZoneEditor", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);

		m_guiExit->addComponent(new TextField("TEXTFIELD_WORLD_NAME", " Name", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);
		m_guiExit->addComponent(new CButton("BUTTON_SET", "Save", {-173, 24}, {166, 24}, 1, []() {
			if(ZoneEditor::getInstance().m_guiExit->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				ZoneEditor::getInstance().m_map->save(ZoneEditor::getInstance().m_guiExit->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				Globals::getInstance().m_exitting = 2;
			}
		}), PANEL_ALIGN_CENTER);
		m_guiExit->addComponent(new CButton("BUTTON_NO_SAVE", "Don't Save", {0, 24}, {166, 24}, 1, []() {Globals::getInstance().m_exitting = 2; }), PANEL_ALIGN_CENTER);
		m_guiExit->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {173, 24}, {166, 24}, 1, []() {
			ZoneEditor::getInstance().unpause();
			Globals::getInstance().m_exitting = 0;
		}), PANEL_ALIGN_CENTER);
	}

	m_guiPause->addComponent(m_guiSaveMap, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiLoadMap, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiClearMap, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiResizeMap, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiExit, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiEntityTex, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiEntityInteract, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiEntityIdle, PANEL_ALIGN_CENTER);

	m_pauseScreens.push_back(m_guiSaveMap);
	m_pauseScreens.push_back(m_guiLoadMap);
	m_pauseScreens.push_back(m_guiClearMap);
	m_pauseScreens.push_back(m_guiResizeMap);
	m_pauseScreens.push_back(m_guiExit);
	m_pauseScreens.push_back(m_guiEntityTex);
	m_pauseScreens.push_back(m_guiEntityInteract);
	m_pauseScreens.push_back(m_guiEntityIdle);



	m_map->setLayerVisible(0, m_guiLeftLayer->findComponent("BUTTON_GROUND_VISIBLE")->isSelected() != 0);
	m_map->setLayerVisible(1, m_guiLeftLayer->findComponent("BUTTON_WORLD_VISIBLE")->isSelected() != 0);
	m_map->setLayerVisible(2, m_guiLeftLayer->findComponent("BUTTON_ENTITY_VISIBLE")->isSelected() != 0);
	m_map->setLayerVisible(3, m_guiLeftLayer->findComponent("BUTTON_SKY_VISIBLE")->isSelected() != 0);
}

void ZoneEditor::input()
{
	Editor::input();
}

void ZoneEditor::update()
{
	Editor::update();

	switch(m_selectLayer->getSelectedButton())
	{
	case 1:
		if(m_listWorld->isUpdated() != 0)
		{
			m_guiWorld->setVisible(true);
			if((m_listWorld->isUpdated() & 2) == 2) // New item
			{

			}
			if((m_listWorld->isUpdated() & 1) == 1) // Switch item
			{
				m_guiWorldSwitch->findComponent("NUMFIELD_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
				m_guiWorldSwitch->findComponent("SLIDER_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
				m_guiWorldSolidSwitch->findComponent("NUMFIELD_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
				m_guiWorldSolidSwitch->findComponent("SLIDER_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
				m_guiWorldPortal->findComponent("NUMFIELD_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
				m_guiWorldPortal->findComponent("SLIDER_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
				m_guiWorldPortal->findComponent("TEXTFIELD_NAME")->setTitle(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_portalDest);
				m_guiWorldPortal->findComponent("NUMFIELD_X")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destX);
				m_guiWorldPortal->findComponent("NUMFIELD_Y")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destY);
				m_guiWorldDirection->findComponent("DROPDOWN_DIRECTION")->setSelectedItem(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_direction);
			}
		}
		else if(m_guiWorld->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_listWorld->getSelectedItem() != 0 && m_listWorld->getSelectedItem() < m_listWorld->getListSize())
		{
			m_guiWorldSwitch->findComponent("NUMFIELD_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			m_guiWorldSwitch->findComponent("SLIDER_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			m_guiWorldSolidSwitch->findComponent("NUMFIELD_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			m_guiWorldSolidSwitch->findComponent("SLIDER_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			m_guiWorldPortal->findComponent("TEXTFIELD_NAME")->setTitle(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_portalDest);
			m_guiWorldPortal->findComponent("NUMFIELD_X")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destX);
			m_guiWorldPortal->findComponent("NUMFIELD_Y")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destY);
			m_guiWorldDirection->findComponent("DROPDOWN_DIRECTION")->setSelectedItem(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_direction);
		}
		break;
	}
}
