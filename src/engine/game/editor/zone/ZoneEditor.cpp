#include "engine\game\editor\zone\ZoneEditor.h"

void ZoneEditor::init()
{
	m_map = new ZoneMap(Vector2<Uint16>(64, 64));
	m_map->load("_autosave");

	m_camPos = Vector2<GLfloat>(GLfloat(m_map->getSize().x / 2), GLfloat(m_map->getSize().y / 2));

	m_map->setTextureGround(LTexture::getInstance().getImage("TilesheetGround.png"));
	m_map->setTextureWorld(LTexture::getInstance().getImage("TilesheetWorld.png"));
	m_map->setTextureSky(LTexture::getInstance().getImage("TilesheetSky.png"));

	Editor::init();

	m_guiGround->addComponent(new CSlider("SLIDER_TIME", "Zone Time", {0, 320}, 252, 24, 0), PANEL_ALIGN_TOP)->setFunction([]()
	{
		ZoneEditor::getInstance().m_map->setTime(ZoneEditor::getInstance().m_guiGround->findComponent("SLIDER_TIME")->getValue());
	});

	m_guiLeftLayer->addComponent(new CButton("BUTTON_EDITOR_TYPE", "", LTexture::getInstance().getImage("gui\\BoardIcon.png"), {0, -15}, {24, 24}, 1), PANEL_ALIGN_CENTER)->setFunction([]() {ZoneEditor::getInstance().autosave(); })->setTooltip("Switch to Battlefield editor");

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
	m_guiEntityIdle = new Container("CONTAINER_ENTITY_IDLE_SCRIPT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityIdle->addComponent(new Panel("PANEL_BG2", "Idle Script", {0, -12}, Globals::getInstance().m_screenSize - Vector2<Sint32>(256, 128), 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityIdle->addComponent(new TextField("TEXTFIELD_SCRIPT", "Go to docs folder for interact script api", {0, 0}, Vector2<Sint32>(Globals::getInstance().m_screenSize.x - 256, Globals::getInstance().m_screenSize.y / 16 - 14), 1, false), PANEL_ALIGN_CENTER);
		m_guiEntityIdle->addComponent(new CButton("BUTTON_SET", "Save", {-130, 24}, {252, 24}, 1, []() {
			ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_idle = ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->getTitle();
			ZoneEditor::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityIdle->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}
	m_guiEntityInteract = new Container("CONTAINER_ENTITY_INTERACT_SCRIPT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityInteract->addComponent(new Panel("PANEL_BG2", "Interact Script", {0, -12}, Globals::getInstance().m_screenSize - Vector2<Sint32>(256, 128), 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityInteract->addComponent(new TextField("TEXTFIELD_SCRIPT", "Go to docs folder for interact script api", {0, 0}, Vector2<Sint32>(Globals::getInstance().m_screenSize.x - 256, Globals::getInstance().m_screenSize.y / 16 - 14), 1, false), PANEL_ALIGN_CENTER);
		m_guiEntityInteract->addComponent(new CButton("BUTTON_SET", "Save", {-130, 24}, {252, 24}, 1, []() {
			ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact = ZoneEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->getTitle();
			ZoneEditor::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityInteract->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}
	m_guiEntityInventory = new Container("CONTAINER_ENTITY_INVENTORY", {0, 0}, {384, 512}, false);
	{
		m_guiEntityInventory->addComponent(new Panel("PANEL_BG2", "Inventory", {0, -12}, {384, 512}, 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityInventory->addComponent(new CList("DROPDOWN_INVENTORY", "Items", {0, 48}, {256, 4}, 32, Texture(), 1), PANEL_ALIGN_TOP);
		m_guiEntityInventory->addComponent(new CButton("BUTTON_SET", "Save", {-96, 24}, {128, 24}, 1, []() {
			ZoneMap::Entity* _en = &ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem());
			_en->m_inventory.clear();
			for(Uint16 i = 0; i < ZoneEditor::getInstance().m_guiEntityInventory->findComponent("DROPDOWN_INVENTORY")->getItemCount(); i++)
			{
				_en->m_inventory.push_back(0);
			}
			ZoneEditor::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityInventory->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {96, 24}, {128, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}
	m_guiEntityBoard = new Container("CONTAINER_ENTITY_BOARD", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityBoard->addComponent(new Panel("PANEL_BG2", "Battle Editor", {0, -12}, {384, 512}, 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityBoard->addComponent(new TextField("TEXTFIELD_BOARD", "Loaded from map\\boards\\<input>", {0, 0}, Vector2<Sint32>(256, 1), 1, false), PANEL_ALIGN_CENTER);
		m_guiEntityBoard->addComponent(new CButton("BUTTON_SET", "Save", {-130, 24}, {128, 24}, 1, []() {
			ZoneEditor::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityBoard->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {128, 24}, 1, []() {ZoneEditor::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}

	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_TEXTURE", "Set Spritesheet", Vector2<Sint32>(0, 362), Vector2<Sint32>(264, 32), 1, []()
	{
		ZoneEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->setState(1);
		ZoneEditor::getInstance().pause("CONTAINER_ENTITY_TEXTURE");
		ZoneEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->setTitle(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex.getName());
	}), PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_IDLE_SCRIPT", "Edit Idle Script", Vector2<Sint32>(0, 466), Vector2<Sint32>(264, 32), 1, []()
	{
		ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setState(1);
		ZoneEditor::getInstance().pause("CONTAINER_ENTITY_IDLE_SCRIPT");
		ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setTitle(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_idle);
		ZoneEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setState(1);
	}), PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_INTERACT_SCRIPT", "Edit Interaction Script", Vector2<Sint32>(0, 502), Vector2<Sint32>(264, 32), 1, []()
	{
		ZoneEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->setState(1);
		ZoneEditor::getInstance().pause("CONTAINER_ENTITY_INTERACT_SCRIPT");
		ZoneEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->setTitle(ZoneEditor::getInstance().m_map->getEntity(ZoneEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact);
		ZoneEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->setState(1);
	}), PANEL_ALIGN_TOP);

	m_guiEntityNeutral = new Container("GUI_ENTITY_NEUTRAL", {0, 0}, m_guiEntity->getSize(), false);
	m_guiEntityOpponent = new Container("GUI_ENTITY_OPPONENT", {0, 0}, m_guiEntity->getSize(), false);
	m_guiEntityTrader = new Container("GUI_ENTITY_TRADER", {0, 0}, m_guiEntity->getSize(), false);
	m_guiEntityLootBag = new Container("GUI_ENTITY_LOOTBAG", {0, 0}, m_guiEntity->getSize(), false);

	m_guiEntityOpponent->addComponent(new CButton("BUTTON_ENTITY_SET_BOARD", "Set Board", Vector2<Sint32>(0, 538), Vector2<Sint32>(264, 32), 1, []()
	{
		std::cout << "Currently disabled" << std::endl;
		//ZoneEditor::getInstance().pause("CONTAINER_ENTITY_BOARD");
	}), PANEL_ALIGN_TOP);
	m_guiEntityTrader->addComponent(m_guiEntityLootBag->addComponent(new CButton("BUTTON_ENTITY_SET_INVENTORY", "Set Inventory", Vector2<Sint32>(0, 538), Vector2<Sint32>(264, 32), 1, []()
	{
		ZoneEditor::getInstance().pause("CONTAINER_ENTITY_INVENTORY");
	}), PANEL_ALIGN_TOP));

	m_guiEntity->addComponent(m_guiEntityNeutral, PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(m_guiEntityOpponent, PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(m_guiEntityTrader, PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(m_guiEntityLootBag, PANEL_ALIGN_TOP);

	m_guiEntity->findComponent("DROPDOWN_TYPE")->addItem("Neutral")->addItem("Opponent")->addItem("Trader")->addItem("Loot Bag");
	m_guiEntity->findComponent("DROPDOWN_TYPE")->setFunction([]()
	{
		Sint16 _sel = ZoneEditor::getInstance().m_guiEntity->findComponent("DROPDOWN_TYPE")->getSelectedItem();
		Sint16 _pSel = ZoneEditor::getInstance().m_guiEntity->findComponent("DROPDOWN_TYPE")->getPrevSelectedItem();
		if(_sel != _pSel)
		{
			switch(_sel)
			{
			case 0: // Neutral
				ZoneEditor::getInstance().m_guiEntityNeutral->setVisible(true);
				break;
			case 1: // Opponent
				ZoneEditor::getInstance().m_guiEntityOpponent->setVisible(true);
				break;
			case 2: // Trader
				ZoneEditor::getInstance().m_guiEntityTrader->setVisible(true);
				break;
			case 3: // Loot Bag
				ZoneEditor::getInstance().m_guiEntityLootBag->setVisible(true);
				break;
			}
			switch(_pSel)
			{
			case 0: // Neutral
				ZoneEditor::getInstance().m_guiEntityNeutral->setVisible(false);
				break;
			case 1: // Opponent
				ZoneEditor::getInstance().m_guiEntityOpponent->setVisible(false);
				break;
			case 2: // Trader
				ZoneEditor::getInstance().m_guiEntityTrader->setVisible(false);
				break;
			case 3: // Loot Bag
				ZoneEditor::getInstance().m_guiEntityLootBag->setVisible(false);
				break;
			}
		}
		ZoneEditor::getInstance().m_guiEntity->calcSize();
	});

	m_guiWorldSwitch = new Container("GUI_SWITCH", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldSolidSwitch = new Container("GUI_SOLID_SWITCH", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldPortal = new Container("GUI_PORTAL", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldDirection = new Container("GUI_DIRECTION", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldLight = new Container("GUI_DIRECTION", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);

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

	m_guiWorldLight->addComponent(new CSlider("SLIDER_LIGHT", "Light Value", {0, 0}, 252, 10, 0), PANEL_ALIGN_CENTER)->setFunction([]()
	{
		ZoneEditor::getInstance().m_map->getWorldObject(ZoneEditor::getInstance().m_listWorld->getSelectedItem()).m_lightValue = ZoneEditor::getInstance().m_guiWorldLight->findComponent("SLIDER_LIGHT")->getValue();
	})->setTooltip("How bright the light is");

	m_guiWorld->addComponent(m_guiWorldSwitch, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldSolidSwitch, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldPortal, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldDirection, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldLight, PANEL_ALIGN_CENTER);

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
			case 6: // Light
				ZoneEditor::getInstance().m_guiWorldLight->setVisible(true);
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
			case 6: // Light
				ZoneEditor::getInstance().m_guiWorldLight->setVisible(false);
				break;
			}
		}
		ZoneEditor::getInstance().m_guiWorld->calcSize();
	});
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("None")->addItem("Solid")->addItem("Switch")->addItem("Solid Switch")->addItem("Portal")->addItem("Directional")->addItem("Light");
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



	m_tileSetGround = new CTileSet("TILESET_GROUND", "Ground Tile Set", {0, 34}, {256, 256}, 32, m_map->getTextureGround(), 1);
	m_tileSetWorld = new CTileSet("TILESET_WORLD", "World Tile Set", {0, 318}, {256, 256}, 32, m_map->getTextureWorld(), 1);
	m_tileSetEntity = new CTileSet("TILESET_ENTITY", "Spritesheet", {0, 82}, {256, 256}, 32, Texture(), 1);
	m_tileSetSky = new CTileSet("TILESET_SKY", "Sky Tile Set", {0, 32}, {256, 256}, 32, m_map->getTextureSky(), 1);
	m_tileSetStamps = new CTileSet("TILESET_WORLD", "TODO: Preview", {0, 88}, {256, 256}, 32, Texture(), 1);

	m_guiGround->addComponent(m_tileSetGround, PANEL_ALIGN_TOP);
	m_guiWorld->addComponent(m_tileSetWorld, PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(m_tileSetEntity, PANEL_ALIGN_TOP);
	m_guiSky->addComponent(m_tileSetSky, PANEL_ALIGN_TOP);
	m_guiStamp->addComponent(m_tileSetStamps, PANEL_ALIGN_TOP);

	m_listWorld = new CList("LIST_WORLD", "World Object List", {0, 64}, {256, 4}, 32, m_map->getTextureWorld(), 1);
	if(m_map->getWorldObjectSize() == 0)
	{
		m_listWorld->addItem("None", 0);
		m_map->addWorldObject(Map::WorldObject("None", 0, 0));
	}
	m_guiWorld->addComponent(m_listWorld, PANEL_ALIGN_TOP);

	m_guiSaveMap = new Container("CONTAINER_SAVE_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiSaveMap->addComponent(new Panel("PANEL_BG2", "Save World Map", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
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
		m_guiLoadMap->addComponent(new Panel("PANEL_BG2", "Load World Map", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new TextField("TEXTFIELD_WORLD_NAME", "Load from maps\\zones\\<input>", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new CButton("BUTTON_SET", "Load", {-130, 24}, {252, 24}, 1, []() {
			if(ZoneEditor::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				ZoneEditor::getInstance().m_map->load(ZoneEditor::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				ZoneEditor::getInstance().refresh();
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
	m_guiPause->addComponent(m_guiEntityInventory, PANEL_ALIGN_CENTER);

	m_pauseScreens.push_back(m_guiSaveMap);
	m_pauseScreens.push_back(m_guiLoadMap);
	m_pauseScreens.push_back(m_guiClearMap);
	m_pauseScreens.push_back(m_guiResizeMap);
	m_pauseScreens.push_back(m_guiExit);
	m_pauseScreens.push_back(m_guiEntityTex);
	m_pauseScreens.push_back(m_guiEntityInteract);
	m_pauseScreens.push_back(m_guiEntityIdle);
	m_pauseScreens.push_back(m_guiEntityInventory);



	m_map->setLayerVisible(0, m_guiLeftLayer->findComponent("BUTTON_GROUND_VISIBLE")->isSelected() != 0);
	m_map->setLayerVisible(1, m_guiLeftLayer->findComponent("BUTTON_WORLD_VISIBLE")->isSelected() != 0);
	m_map->setLayerVisible(2, m_guiLeftLayer->findComponent("BUTTON_ENTITY_VISIBLE")->isSelected() != 0);
	m_map->setLayerVisible(3, m_guiLeftLayer->findComponent("BUTTON_SKY_VISIBLE")->isSelected() != 0);

	refresh();
}

std::string ZoneEditor::getZoneName()
{
	return m_map->getName();
}

void ZoneEditor::input()
{
	Vector2<Sint32> _mousePos = Globals::getInstance().m_mousePos;
	Sint8 _rValue = 0; // Input flags: 1 = mouse click, 2 = key press, 4 = mouse scroll
	Vector2<GLfloat> _camPos = m_camPos * (TILE_SIZE + m_zoom);

	m_guiAll->input(_rValue, Globals::getInstance().m_keyStates, Globals::getInstance().m_mouseStates, _mousePos);

	if(getPause() == "" && (_rValue & 2) == 0 && Globals::getInstance().m_keyStates[GLFW_KEY_G] == 1)
		m_showGrid = !m_showGrid;

	if(getPause() == "" && (_rValue & 1) == 0 && Globals::getInstance().m_mouseScroll != 0)
	{
		m_tZoom += (Globals::getInstance().m_mouseScroll) / 4.f;
		if(m_tZoom <= -TILE_SIZE + 6)
			m_tZoom = -TILE_SIZE + 6;
		if(m_tZoom > 0)
			m_tZoom = 0;
	}

	if((_rValue & 2) == 0 && Globals::getInstance().m_keyStates[GLFW_KEY_ESCAPE] == 1)
	{
		if(!m_cPauseScreen.empty())
		{
			unpause();
			if(Globals::getInstance().m_exitting == 1)
				Globals::getInstance().m_exitting = 0;
		}
		else
			Globals::getInstance().m_exitting = 1;
		_rValue += 2;
	}

	if(getPause() != "" && (_rValue & 2) == 0 && Globals::getInstance().m_keyStates[GLFW_KEY_ENTER] == 1)
		m_pauseScreens[m_cPauseScreen[m_cPauseScreen.size() - 1]]->findComponent("BUTTON_SET")->setState(1);

	if(getPause() == "" && Globals::getInstance().m_keyStates[GLFW_KEY_LEFT_CONTROL] != 0 && Globals::getInstance().m_keyStates[GLFW_KEY_Z] == 1)
		m_map->undo();
	if(getPause() == "" && Globals::getInstance().m_keyStates[GLFW_KEY_LEFT_CONTROL] != 0 && Globals::getInstance().m_keyStates[GLFW_KEY_Y] == 1)
		m_map->redo();

	if(Globals::getInstance().m_mouseStates[0] == 1 && m_mouseInArea)
	{
		if(!m_lmbDown)
		{
			if(Globals::getInstance().m_keyStates[GLFW_KEY_LEFT_SHIFT] == 0)
				m_map->startEdit();
			else
			{
				switch(m_selectLayer->getSelectedButton())
				{
				case 0:
					m_map->startFill(0, m_tileSetGround->getSelectedTile(), Vector2<Sint32>(Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)))));
					break;
				case 1:
					m_map->startFill(1, m_listWorld->getSelectedItem(), Vector2<Sint32>(Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)))));
					break;
				case 2:
					m_map->startEdit();
					break;
				case 3:
					m_map->startFill(3, m_tileSetSky->getSelectedTile(), Vector2<Sint32>(Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)))));
					break;
				}
			}
			m_lmbDown = true;
		}
		if(m_selectLayer->getSelectedButton() == 4)
			m_selectStart = {Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1), Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1)};
	}
	if(Globals::getInstance().m_mouseStates[1] == 1 && m_mouseInArea)
	{
		if(m_tileMapArea.checkPoint(GLfloat(m_mouseBuffer.x), GLfloat(m_mouseBuffer.y)))
			m_rmbDown = true;
	}

	if(m_lmbDown && Globals::getInstance().m_mouseStates[0] == 0)
	{
		m_map->stopEdit();
		m_map->stopFill();
		m_lmbDown = false;
	}
	if(m_rmbDown && Globals::getInstance().m_mouseStates[1] == 0)
		m_rmbDown = false;

	if((_rValue & 1) == 0 && m_lmbDown && !m_rmbDown && m_mouseInArea)
	{
		if(m_map->isFilling())
			m_map->moveFill(Vector2<Sint32>(Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)))));
		else
		{
			switch(m_selectLayer->getSelectedButton())
			{
			case 0:
				m_map->setTile(0, Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom))), m_tileSetGround->getSelectedTile());
				break;
			case 1:
				m_map->setTile(1, Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom))), m_listWorld->getSelectedItem());
				break;
			case 2:
				if(m_tabEntity->getSelectedItem() >= 0)
					m_map->setEntity(m_tabEntity->getSelectedItem(), Vector2<Sint32>(Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)))));
				break;
			case 3:
				m_map->setTile(2, Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom))), m_tileSetSky->getSelectedTile());
				break;
			case 4:
				if(m_listStamps->getSelectedItem() == 0)
					m_selectEnd = {Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)))};
				else
				{
					Stamp _selected = m_stamps[m_listStamps->getSelectedItem()];

					for(Sint32 x = Sint32(ceil(-GLfloat(_selected.m_size.x) / 2)); x < Sint32(ceil(GLfloat(_selected.m_size.x) / 2)); x++)
					{
						for(Sint32 y = Sint32(ceil(-GLfloat(_selected.m_size.y) / 2)); y < Sint32(ceil(GLfloat(_selected.m_size.y) / 2)); y++)
						{
							if(m_guiStamp->findComponent("BUTTON_USE_GROUND")->isSelected() != 0)
								m_map->setTile(0, Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom)) - 1) + x, Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)) - 1) + y, _selected.m_ground[x - Sint32(ceil(-GLfloat(_selected.m_size.x) / 2))][y - Sint32(ceil(-GLfloat(_selected.m_size.y) / 2))]);
							if(m_guiStamp->findComponent("BUTTON_USE_WORLD")->isSelected() != 0)
								m_map->setTile(1, Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom)) - 1) + x, Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)) - 1) + y, _selected.m_world[x - Sint32(ceil(-GLfloat(_selected.m_size.x) / 2))][y - Sint32(ceil(-GLfloat(_selected.m_size.y) / 2))]);
							if(m_guiStamp->findComponent("BUTTON_USE_ENTITY")->isSelected() != 0)
								m_map->setTile(2, Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom)) - 1) + x, Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)) - 1) + y, _selected.m_entity[x - Sint32(ceil(-GLfloat(_selected.m_size.x) / 2))][y - Sint32(ceil(-GLfloat(_selected.m_size.y) / 2))]);
							if(m_guiStamp->findComponent("BUTTON_USE_SKY")->isSelected() != 0)
								m_map->setTile(3, Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom)) - 1) + x, Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)) - 1) + y, _selected.m_sky[x - Sint32(ceil(-GLfloat(_selected.m_size.x) / 2))][y - Sint32(ceil(-GLfloat(_selected.m_size.y) / 2))]);
						}
					}
				}
				break;
			}
		}
	}
	else if(m_rmbDown)
	{
		m_camPos = m_camPos + Vector2<GLfloat>(m_mouseBuffer - _mousePos) / (TILE_SIZE + m_zoom);

		if(m_camPos.x < 0)
			m_camPos.x = 0;
		if(m_camPos.y < 0)
			m_camPos.y = 0;
		if(m_camPos.x > (m_map->getSize().x))
			m_camPos.x = GLfloat((m_map->getSize().x));
		if(m_camPos.y > (m_map->getSize().y))
			m_camPos.y = GLfloat((m_map->getSize().y));
	}

	_camPos = m_camPos;

	m_mouseBuffer = _mousePos;
	m_mouseInArea = ((_rValue & 1) == 0
		&& m_tileMapArea.checkPoint(GLfloat(m_mouseBuffer.x), GLfloat(m_mouseBuffer.y))
		&& ((m_mouseBuffer.x / (TILE_SIZE + m_zoom) + m_camPos.x) >= 0) && ((m_mouseBuffer.y / (TILE_SIZE + m_zoom) + m_camPos.y) >= 0)
		&& ((m_mouseBuffer.x / (TILE_SIZE + m_zoom) + m_camPos.x) < m_map->getSize().x) && ((m_mouseBuffer.y / (TILE_SIZE + m_zoom) + m_camPos.y) < m_map->getSize().y));
}
void ZoneEditor::update()
{
	if(Globals::getInstance().m_exitting == 1 && getPause() != "CONTAINER_EXIT")
		pause("CONTAINER_EXIT");

	m_deltaUpdate = GLfloat(glfwGetTime() - m_lastUpdate);
	m_lastUpdate = GLfloat(glfwGetTime());

	GLfloat _zoom = m_zoom;
	m_zoom -= (m_zoom - m_tZoom) * m_deltaUpdate * 10.f;
	if(abs(m_zoom - m_tZoom) < 0.01f)
		m_zoom = m_tZoom;

	m_camPos.x += (GLfloat(m_mouseBuffer.x) / (TILE_SIZE + _zoom) - GLfloat(m_mouseBuffer.x) / (TILE_SIZE + m_zoom));
	m_camPos.y += (GLfloat(m_mouseBuffer.y) / (TILE_SIZE + _zoom) - GLfloat(m_mouseBuffer.y) / (TILE_SIZE + m_zoom));

	if(m_camPos.x < 0)
		m_camPos.x = 0;
	if(m_camPos.y < 0)
		m_camPos.y = 0;
	if(m_camPos.x > (m_map->getSize().x))
		m_camPos.x = GLfloat((m_map->getSize().x));
	if(m_camPos.y > (m_map->getSize().y))
		m_camPos.y = GLfloat((m_map->getSize().y));

	m_guiAll->update(m_deltaUpdate);

	if(m_selectLayer->getPrevSelectedButton() != m_selectLayer->getSelectedButton())
	{
		switch(m_selectLayer->getSelectedButton())
		{
		case 0:
			m_guiGround->setVisible(true);
			break;
		case 1:
			m_guiWorld->setVisible(true);
			break;
		case 2:
			m_guiEntity->setVisible(true);
			break;
		case 3:
			m_guiSky->setVisible(true);
			break;
		case 4:
			m_guiStamp->setVisible(m_listStamps->getSelectedItem() != 0);
			break;
		}

		switch(m_selectLayer->getPrevSelectedButton())
		{
		case 0:
			m_guiGround->setVisible(false);
			break;
		case 1:
			m_guiWorld->setVisible(false);
			break;
		case 2:
			m_guiEntity->setVisible(false);
			break;
		case 3:
			m_guiSky->setVisible(false);
			break;
		case 4:
			m_guiStamp->setVisible(false);
			m_selectStart = {-1, -1};
			break;
		}
	}

	switch(m_selectLayer->getSelectedButton())
	{
	case 1: // WORLD OBJECTS
		if(m_listWorld->isUpdated() != 0)
		{
			m_guiWorld->setVisible(true);
			if((m_listWorld->isUpdated() & 2) == 2) // New item
			{
				m_map->addWorldObject({m_listWorld->getItem(m_listWorld->getSelectedItem()), 1, m_listWorld->getItemTexId(m_listWorld->getSelectedItem())});
			}
			if((m_listWorld->isUpdated() & 1) == 1) // Switch item
			{
				m_guiWorld->findComponent("TEXTFIELD_OBJECT_NAME")->setTitle(m_listWorld->getItem(m_listWorld->getSelectedItem()));
				m_guiWorld->findComponent("DROPDOWN_INTERACT")->setSelectedItem(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_interactionType);
				m_tileSetWorld->setSelectedTile(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_tileTex);

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
				m_guiWorldLight->findComponent("SLIDER_LIGHT")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_lightValue);
			}
		}
		else if(m_guiWorld->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_listWorld->getSelectedItem() != 0 && m_listWorld->getSelectedItem() < m_listWorld->getItemCount())
		{
			m_listWorld->removeItem(m_listWorld->getSelectedItem());
			m_map->removeWorldObject(m_listWorld->getSelectedItem() + 1);

			m_guiWorld->findComponent("TEXTFIELD_OBJECT_NAME")->setTitle(m_listWorld->getItem(m_listWorld->getSelectedItem()));
			m_guiWorld->findComponent("DROPDOWN_INTERACT")->setSelectedItem(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_interactionType);
			m_tileSetWorld->setSelectedTile(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_tileTex);

			m_guiWorldSwitch->findComponent("NUMFIELD_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			m_guiWorldSwitch->findComponent("SLIDER_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			m_guiWorldSolidSwitch->findComponent("NUMFIELD_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			m_guiWorldSolidSwitch->findComponent("SLIDER_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			m_guiWorldPortal->findComponent("TEXTFIELD_NAME")->setTitle(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_portalDest);
			m_guiWorldPortal->findComponent("NUMFIELD_X")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destX);
			m_guiWorldPortal->findComponent("NUMFIELD_Y")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destY);
			m_guiWorldDirection->findComponent("DROPDOWN_DIRECTION")->setSelectedItem(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_direction);
			m_guiWorldLight->findComponent("SLIDER_LIGHT")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_lightValue);
		}
		if(m_listWorld->getSelectedItem() != 0)
		{
			if(m_guiWorld->findComponent("TEXTFIELD_OBJECT_NAME")->getTitle() != "")
				m_listWorld->setItem(m_listWorld->getSelectedItem(), m_guiWorld->findComponent("TEXTFIELD_OBJECT_NAME")->getTitle());
			m_listWorld->setItemTexId(m_listWorld->getSelectedItem(), m_tileSetWorld->getSelectedTile());
			m_map->getWorldObject(m_listWorld->getSelectedItem()).m_interactionType = m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem();
			m_map->getWorldObject(m_listWorld->getSelectedItem()).m_tileTex = m_tileSetWorld->getSelectedTile();
		}
		break;
	case 2: // ENTITIES
		if(m_tabEntity->isUpdated() != 0)
		{
			if((m_tabEntity->isUpdated() & 2) == 2) // New item
				m_map->addEntity(ZoneMap::Entity(m_tabEntity->getItem(m_tabEntity->getSelectedItem())));
			if((m_tabEntity->isUpdated() & 1) == 1) // Switch item
			{
				m_guiEntity->findComponent("TEXTFIELD_ENTITY_NAME")->setTitle(m_tabEntity->getItem(m_tabEntity->getSelectedItem()));
				if(m_tabEntity->getSelectedItem() != 0)
					m_tileSetEntity->setTileSheet(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityTex);
				else
					m_tileSetEntity->setTileSheet(Texture());
				m_tileSetEntity->setSelectedTile(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityTexId);
				m_guiEntity->findComponent("DROPDOWN_TYPE")->setSelectedItem(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityType);
			}
		}
		else if(m_guiEntity->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_tabEntity->getItemCount() > 0 && m_tabEntity->getSelectedItem() != -1 && m_tabEntity->getSelectedItem() < m_tabEntity->getTabCount())
		{
			m_map->removeEntity(m_tabEntity->getSelectedItem());
			m_tabEntity->removeTab(m_tabEntity->getSelectedItem());

			if(m_tabEntity->getItemCount() > 0)
			{
				m_guiEntity->findComponent("TEXTFIELD_ENTITY_NAME")->setTitle(m_tabEntity->getItem(m_tabEntity->getSelectedItem()));
				m_tileSetEntity->setTileSheet(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityTex);
				m_tileSetEntity->setSelectedTile(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityTexId);
			}
			else
			{
				m_guiEntity->findComponent("TEXTFIELD_ENTITY_NAME")->setTitle("");
				m_tileSetEntity->setTileSheet(Texture());
				m_tileSetEntity->setSelectedTile(0);
			}
		}
		if(m_tabEntity->getItemCount() > 0)
		{
			if(m_guiEntity->findComponent("TEXTFIELD_ENTITY_NAME")->getTitle() != "")
			{
				m_tabEntity->setTab(m_tabEntity->getSelectedItem(), m_guiEntity->findComponent("TEXTFIELD_ENTITY_NAME")->getTitle());
				m_map->getEntity(m_tabEntity->getSelectedItem()).m_name = m_tabEntity->getItem(m_tabEntity->getSelectedItem());
			}
			m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityType = Uint8(m_guiEntity->findComponent("DROPDOWN_TYPE")->getSelectedItem());
			m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityTexId = m_tileSetEntity->getSelectedTile();
		}
		break;
	case 3:

		break;
	case 4: // STAMPS
		if(m_listStamps->isUpdated() != 0)
		{
			if((m_listStamps->isUpdated() & 2) == 2) // New item
				m_stamps.push_back(Stamp());
			if((m_listStamps->isUpdated() & 1) == 1) // Switch item
			{
				m_guiStamp->findComponent("TEXTFIELD_STAMP_NAME")->setTitle(m_listStamps->getItem(m_listStamps->getSelectedItem()));
				m_selectStart = {-1, -1};
			}
			m_guiStamp->setVisible(m_listStamps->getSelectedItem() != 0);
		}
		else if(m_guiStamp->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_listStamps->getSelectedItem() != 0 && m_listStamps->getSelectedItem() < m_listStamps->getItemCount())
		{
			m_stamps.erase(m_stamps.begin() + m_listStamps->getSelectedItem());
			m_listStamps->removeItem(m_listStamps->getSelectedItem());

			m_guiStamp->findComponent("TEXTFIELD_STAMP_NAME")->setTitle(m_listStamps->getItem(m_listStamps->getSelectedItem()));
			m_selectStart = {-1, -1};

			m_guiStamp->setVisible(m_listStamps->getSelectedItem() != 0);
		}
		else if(m_listStamps->getSelectedItem() != 0)
		{
			if(m_guiStamp->findComponent("TEXTFIELD_STAMP_NAME")->getTitle() != "")
				m_listStamps->getItem(m_listStamps->getSelectedItem()) = m_guiStamp->findComponent("TEXTFIELD_STAMP_NAME")->getTitle();
		}
		if(m_listStamps->getSelectedItem() == 0 && (Globals::getInstance().m_keyStates[GLFW_KEY_LEFT_CONTROL] || Globals::getInstance().m_keyStates[GLFW_KEY_RIGHT_CONTROL]) && Globals::getInstance().m_keyStates[GLFW_KEY_C] == 1)
		{
			m_stamps.push_back(Stamp());
			Vector2<Sint32> _topLeft = Vector2<Sint32>(min(m_selectStart.x, m_selectEnd.x), min(m_selectStart.y, m_selectEnd.y));
			Vector2<Sint32> _stampSize = Vector2<Sint32>(abs(m_selectEnd.x - m_selectStart.x), abs(m_selectEnd.y - m_selectStart.y)) + 1;
			Uint16** _groundData = new Uint16*[_stampSize.x];
			Uint16** _worldData = new Uint16*[_stampSize.x];
			Uint16** _entityData = new Uint16*[_stampSize.x];
			Uint16** _skyData = new Uint16*[_stampSize.x];
			for(Uint16 x = 0; x < _stampSize.x; x++)
			{
				_groundData[x] = new Uint16[_stampSize.y];
				_worldData[x] = new Uint16[_stampSize.y];
				_entityData[x] = new Uint16[_stampSize.y];
				_skyData[x] = new Uint16[_stampSize.y];
				for(Uint16 y = 0; y < _stampSize.y; y++)
				{
					_groundData[x][y] = m_map->getTile(0, x + _topLeft.x, y + _topLeft.y);
					_worldData[x][y] = m_map->getTile(1, x + _topLeft.x, y + _topLeft.y);
					_entityData[x][y] = m_map->getTile(2, x + _topLeft.x, y + _topLeft.y);
					_skyData[x][y] = m_map->getTile(3, x + _topLeft.x, y + _topLeft.y);
				}
			}
			m_stamps[m_stamps.size() - 1] = Stamp(_stampSize, _groundData, _worldData, _entityData, _skyData);
			m_listStamps->addItem(std::string("Item ") + Util::numToString(m_listStamps->getItemCount()), 0);
		}
		break;
	}

	if(m_mouseInArea)
		m_guiLeft->findComponent("TEXT_CURSOR_POS")->setTitle("Selected Tile\nX: " +
			Util::numToString(Sint32(floor(m_mouseBuffer.x / (TILE_SIZE + m_zoom) + m_camPos.x))) + "\nY: " +
			Util::numToString(Sint32(floor(m_mouseBuffer.y / (TILE_SIZE + m_zoom) + m_camPos.y))) + "\nCamera Pos\nX: " +
			Util::numToString((m_camPos.x), 2) + "\nY: " +
			Util::numToString((m_camPos.y), 2));
	else
		m_guiLeft->findComponent("TEXT_CURSOR_POS")->setTitle("Selected Tile\nX: NA\nY: NA\nCamera Pos\nX: " +
			Util::numToString((m_camPos.x), 2) + "\nY: " +
			Util::numToString((m_camPos.y), 2));

	m_map->setViewSize(m_tileMapArea);
	m_map->setTileSize(TILE_SIZE);
	m_map->setGridVisible(m_showGrid);
}

void ZoneEditor::render()
{
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, m_backTexId.getId());
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 1);
		glVertex2f(-GLfloat(Globals::getInstance().m_screenSize.x) / 2, -GLfloat(Globals::getInstance().m_screenSize.y) / 2);
		glTexCoord2f(1, 1);
		glVertex2f(GLfloat(Globals::getInstance().m_screenSize.x) / 2, -GLfloat(Globals::getInstance().m_screenSize.y) / 2);
		glTexCoord2f(1, 0);
		glVertex2f(GLfloat(Globals::getInstance().m_screenSize.x) / 2, GLfloat(Globals::getInstance().m_screenSize.y) / 2);
		glTexCoord2f(0, 0);
		glVertex2f(-GLfloat(Globals::getInstance().m_screenSize.x) / 2, GLfloat(Globals::getInstance().m_screenSize.y) / 2);
	}
	glEnd();

	m_map->render(m_camPos, m_zoom);

	Editor::render();
}

void ZoneEditor::autosave()
{
	m_map->save("_autosave");
}

void ZoneEditor::refresh()
{
	ZoneEditor::getInstance().m_listWorld->clear();
	for(Uint16 i = 0; i < ZoneEditor::getInstance().m_map->getWorldObjectSize(); i++)
		ZoneEditor::getInstance().m_listWorld->addItem(ZoneEditor::getInstance().m_map->getWorldObject(i).m_name, ZoneEditor::getInstance().m_map->getWorldObject(i).m_tileTex);
	ZoneEditor::getInstance().m_tabEntity->clear();
	for(Uint16 i = 0; i < ZoneEditor::getInstance().m_map->getEntitySize(); i++)
		ZoneEditor::getInstance().m_tabEntity->addItem(ZoneEditor::getInstance().m_map->getEntity(i).m_name);
}
