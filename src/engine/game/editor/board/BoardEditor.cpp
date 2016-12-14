#include "engine\game\editor\board\BoardEditor.h"
#include "engine\game\map\board\BoardMap.h"

void BoardEditor::init()
{
	m_map = new BoardMap(Vector2<Uint16>(24, 16));

	m_map->setTextureGround(LTexture::getInstance().getImage("TilesheetGround1.png"));
	m_map->setTextureWorld(LTexture::getInstance().getImage("TilesheetWorld.png"));
	m_map->setTextureSky(LTexture::getInstance().getImage("TilesheetSky.png"));

	Editor::init();

	m_guiLeftLayer->addComponent(new CButton("BUTTON_EDITOR_TYPE", "", LTexture::getInstance().getImage("gui\\ZoneIcon.png"), {0, -15}, {24, 24}, 1), PANEL_ALIGN_CENTER)->setTooltip("Switch to World editor");

	m_guiWorldSlow = new Container("GUI_SLOW", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldTrap = new Container("GUI_TRAP", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldSwitch = new Container("GUI_SWITCH", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldSolidSwitch = new Container("GUI_SOLID_SWITCH", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldPortal = new Container("GUI_PORTAL", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);
	m_guiWorldDirection = new Container("GUI_DIRECTION", {0, 300}, {m_guiLeftDetail->getSize().x, 32}, false);

	m_guiWorldSlow->addComponent(new CSlider("SLIDER_SLOW", "Slow", {0, 0}, 252, 10, 2, 1), PANEL_ALIGN_CENTER)->setTooltip("How many movements it takes to walk through.");

	m_guiWorldTrap->addComponent(new CSlider("SLIDER_TURNS", "Turns", {0, 0}, 252, 10, 2, 1), PANEL_ALIGN_CENTER)->setTooltip("How many turns this trap will hold a unit.");

	m_guiWorldSwitch->addComponent(new CSlider("SLIDER_FREQUENCY", "Frequency", {0, 0}, 252, 255, 0, 1), PANEL_ALIGN_CENTER)->setTooltip("Frequency of switch - which other switches\n or portals it aligns with.\nFreq. 0 means deactive.");

	m_guiWorldSolidSwitch->addComponent(new CSlider("SLIDER_FREQUENCY", "Frequency", {0, 0}, 252, 255, 0, 1), PANEL_ALIGN_CENTER)->setTooltip("Frequency of solid - which switch needs\n to be active to pass.\nFreq. 0 means solid.");

	m_guiWorldPortal->addComponent(new CSlider("SLIDER_FREQUENCY", "Frequency", {0, 0}, 252, 255, 0, 1), PANEL_ALIGN_CENTER)->setTooltip("Frequency of portal - which switch needs\n to be active to use.\nFreq. 0 means always active.");

	m_guiWorldDirection->addComponent(new CDropDown("DROPDOWN_DIRECTION", "Direction", {0, 0}, {264, 32}, 1), PANEL_ALIGN_CENTER)->setTooltip("Which direction the unit is pushed when stepping on this tile");
	m_guiWorldDirection->findComponent("DROPDOWN_DIRECTION")->addItem("Up");
	m_guiWorldDirection->findComponent("DROPDOWN_DIRECTION")->addItem("Right");
	m_guiWorldDirection->findComponent("DROPDOWN_DIRECTION")->addItem("Down");
	m_guiWorldDirection->findComponent("DROPDOWN_DIRECTION")->addItem("Left");
	m_guiWorldDirection->updateSize();

	m_guiWorld->addComponent(m_guiWorldSlow, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldTrap, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldSwitch, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldSolidSwitch, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldPortal, PANEL_ALIGN_CENTER);
	m_guiWorld->addComponent(m_guiWorldDirection, PANEL_ALIGN_CENTER);

	m_guiWorld->findComponent("DROPDOWN_INTERACT")->setFunction([]()
	{
		Sint8 _sel = BoardEditor::getInstance().m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem();
		Sint8 _pSel = BoardEditor::getInstance().m_guiWorld->findComponent("DROPDOWN_INTERACT")->getPrevSelectedItem();
		if(_sel != _pSel)
		{
			switch(_sel)
			{
			case 2: // Slow
				BoardEditor::getInstance().m_guiWorldSlow->setVisible(true);
				break;
			case 3: // Trap
				BoardEditor::getInstance().m_guiWorldTrap->setVisible(true);
				break;
			case 4: // Switch
				BoardEditor::getInstance().m_guiWorldSwitch->setVisible(true);
				break;
			case 5: // Solid Switch
				BoardEditor::getInstance().m_guiWorldSolidSwitch->setVisible(true);
				break;
			case 6: // Portal
				BoardEditor::getInstance().m_guiWorldPortal->setVisible(true);
				break;
			case 7: // Directional
				BoardEditor::getInstance().m_guiWorldDirection->setVisible(true);
				break;
			}
			switch(_pSel)
			{
			case 2: // Slow
				BoardEditor::getInstance().m_guiWorldSlow->setVisible(false);
				break;
			case 3: // Trap
				BoardEditor::getInstance().m_guiWorldTrap->setVisible(false);
				break;
			case 4: // Switch
				BoardEditor::getInstance().m_guiWorldSwitch->setVisible(false);
				break;
			case 5: // Solid Switch
				BoardEditor::getInstance().m_guiWorldSolidSwitch->setVisible(false);
				break;
			case 6: // Portal
				BoardEditor::getInstance().m_guiWorldPortal->setVisible(false);
				break;
			case 7: // Directional
				BoardEditor::getInstance().m_guiWorldDirection->setVisible(false);
				break;
			}
			BoardEditor::getInstance().m_guiWorld->calcSize();
		}
	});
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("None");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Solid");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Slowing");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Trap");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Switch");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Solid Switch");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Portal");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Directional");

	m_toolbarMenu->addButton("", "File");
	{
		m_toolbarMenu->addButton("File", "Save", []() {
			BoardEditor::getInstance().m_guiSaveMap->findComponent("TEXTFIELD_WORLD_NAME")->setState(1);
			BoardEditor::getInstance().m_guiSaveMap->setVisible(true);
			BoardEditor::getInstance().m_guiPause->setVisible(true);
			BoardEditor::getInstance().pause("CONTAINER_SAVE_MAP");
		});
		m_toolbarMenu->addButton("File", "Load", []() {
			BoardEditor::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->setState(1);
			BoardEditor::getInstance().m_guiLoadMap->setVisible(true);
			BoardEditor::getInstance().m_guiPause->setVisible(true);
			BoardEditor::getInstance().pause("CONTAINER_LOAD_MAP");
		});
		m_toolbarMenu->addButton("File", "New", []() {
			BoardEditor::getInstance().m_guiClearMap->setVisible(true);
			BoardEditor::getInstance().m_guiPause->setVisible(true);
			BoardEditor::getInstance().pause("CONTAINER_CLEAR_MAP");
		});
		m_toolbarMenu->addButton("File", "Exit", []() {
			BoardEditor::getInstance().m_guiExit->findComponent("TEXTFIELD_WORLD_NAME")->setState(1);
			Globals::getInstance().m_exitting = 1;
			BoardEditor::getInstance().pause("CONTAINER_EXIT");
		});
	}
	m_toolbarMenu->addButton("", "Edit");
	{
		m_toolbarMenu->addButton("Edit", "Resize", []() {
			BoardEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_WIDTH")->setValue(BoardEditor::getInstance().m_map->getSize().x);
			BoardEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_HEIGHT")->setValue(BoardEditor::getInstance().m_map->getSize().y);
			BoardEditor::getInstance().pause("CONTAINER_RESIZE_MAP");
		});
	}
	m_toolbarMenu->addButton("", "View");
	{
		m_toolbarMenu->addButton("View", "Toggle Grid", []() {
			BoardEditor::getInstance().m_showGrid = !BoardEditor::getInstance().m_showGrid;
		});
		m_toolbarMenu->addButton("View", "Toggle Overlay", []() {
			BoardEditor::getInstance().m_guiLeft->setVisible(!BoardEditor::getInstance().m_guiLeft->isVisible());
		});
	}
	m_toolbarMenu->addButton("", "Help");
	{
		m_toolbarMenu->addButton("Help", "No help yet :(");
	}
	m_guiTop->addComponent(m_toolbarMenu);



	m_guiLeftLayer->addComponent(new CButtonToggle("BUTTON_GROUND_VISIBLE", LTexture::getInstance().getImage("gui\\Visible.png"),
		LTexture::getInstance().getImage("gui\\Invisible.png"), {-48, 4}, {24, 24}, 1, 1, []()
	{ BoardEditor::getInstance().m_map->setLayerVisible(0, BoardEditor::getInstance().m_guiLeftLayer->findComponent("BUTTON_GROUND_VISIBLE")->isSelected() != 0); }), PANEL_ALIGN_TOP);
	m_guiLeftLayer->addComponent(new CButtonToggle("BUTTON_WORLD_VISIBLE", LTexture::getInstance().getImage("gui\\Visible.png"),
		LTexture::getInstance().getImage("gui\\Invisible.png"), {48, 4}, {24, 24}, 1, 1, []()
	{ BoardEditor::getInstance().m_map->setLayerVisible(1, BoardEditor::getInstance().m_guiLeftLayer->findComponent("BUTTON_WORLD_VISIBLE")->isSelected() != 0); }), PANEL_ALIGN_TOP);
	m_guiLeftLayer->addComponent(new CButtonToggle("BUTTON_ENTITY_VISIBLE", LTexture::getInstance().getImage("gui\\Visible.png"),
		LTexture::getInstance().getImage("gui\\Invisible.png"), {-48, 34}, {24, 24}, 1, 1, []()
	{ BoardEditor::getInstance().m_map->setLayerVisible(2, BoardEditor::getInstance().m_guiLeftLayer->findComponent("BUTTON_ENTITY_VISIBLE")->isSelected() != 0); }), PANEL_ALIGN_TOP);
	m_guiLeftLayer->addComponent(new CButtonToggle("BUTTON_SKY_VISIBLE", LTexture::getInstance().getImage("gui\\Visible.png"),
		LTexture::getInstance().getImage("gui\\Invisible.png"), {48, 34}, {24, 24}, 1, 1, []()
	{ BoardEditor::getInstance().m_map->setLayerVisible(3, BoardEditor::getInstance().m_guiLeftLayer->findComponent("BUTTON_SKY_VISIBLE")->isSelected() != 0); }), PANEL_ALIGN_TOP);



	m_guiEntityTex = new Container("CONTAINER_ENTITY_TEXTURE", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityTex->addComponent(new Panel("PANEL_BG2", "Set Texture", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityTex->addComponent(new TextField("TEXTFIELD_TEXTURE", "Directory res\\texture\\", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);
		m_guiEntityTex->addComponent(new CButton("BUTTON_SET", "Set", {-130, 24}, {252, 24}, 1, []() {
			if(BoardEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->getTitle() != "")
			{
				BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex = LTexture::getInstance().getImage(BoardEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->getTitle());
				BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex.setName(BoardEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->getTitle());
				BoardEditor::getInstance().m_tileSetEntity->setTileSheet(BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex);
				BoardEditor::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiEntityTex->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {BoardEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiEntityInteract = new Container("CONTAINER_ENTITY_INTERACT_SCRIPT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityInteract->addComponent(new Panel("PANEL_BG2", "Interact Script", {0, -12}, Globals::getInstance().m_screenSize - Vector2<Sint32>(256, 128), 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityInteract->addComponent(new TextField("TEXTFIELD_SCRIPT", "Go to docs folder for interact script api", {0, 0}, Vector2<Sint32>(Globals::getInstance().m_screenSize.x - 256, Globals::getInstance().m_screenSize.y / 16 - 14), 1, false), PANEL_ALIGN_CENTER);
		m_guiEntityInteract->addComponent(new CButton("BUTTON_SET", "Save", {-130, 24}, {252, 24}, 1, []() {
			BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact = BoardEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->getTitle();
			BoardEditor::getInstance().m_tileSetEntity->setTileSheet(BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex);
			BoardEditor::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityInteract->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {BoardEditor::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}
	m_guiEntityIdle = new Container("CONTAINER_ENTITY_IDLE_SCRIPT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityIdle->addComponent(new Panel("PANEL_BG2", "Idle Script", {0, -12}, Globals::getInstance().m_screenSize - Vector2<Sint32>(256, 128), 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityIdle->addComponent(new TextField("TEXTFIELD_SCRIPT", "Go to docs folder for interact script api", {0, 0}, Globals::getInstance().m_screenSize / 16 - Vector2<Sint32>(512, 21), 1, false), PANEL_ALIGN_CENTER);
		m_guiEntityIdle->addComponent(new CButton("BUTTON_SET", "Save", {-130, 24}, {252, 24}, 1, []() {
			BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact = BoardEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->getTitle();
			BoardEditor::getInstance().m_tileSetEntity->setTileSheet(BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex);
			BoardEditor::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityIdle->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {BoardEditor::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}

	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_TEXTURE", "Set Spritesheet", Vector2<Sint32>(0, 362), Vector2<Sint32>(264, 32), 1, []()
	{
		BoardEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->setState(1);
		BoardEditor::getInstance().pause("CONTAINER_ENTITY_TEXTURE");
		BoardEditor::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->setTitle(BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_entityTex.getName());
	}), PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_INTERACT_SCRIPT", "Edit Interaction Script", Vector2<Sint32>(0, 466), Vector2<Sint32>(264, 32), 1, []()
	{
		BoardEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->setState(1);
		BoardEditor::getInstance().pause("CONTAINER_ENTITY_INTERACT_SCRIPT");
		BoardEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->setTitle(BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact);
		BoardEditor::getInstance().m_guiEntityInteract->findComponent("TEXTFIELD_SCRIPT")->setState(1);
	}), PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(new CButton("BUTTON_ENTITY_IDLE_SCRIPT", "Edit Idle Script", Vector2<Sint32>(0, 502), Vector2<Sint32>(264, 32), 1, []()
	{
		BoardEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setState(1);
		BoardEditor::getInstance().pause("CONTAINER_ENTITY_INTERACT_SCRIPT");
		BoardEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setTitle(BoardEditor::getInstance().m_map->getEntity(BoardEditor::getInstance().m_tabEntity->getSelectedItem()).m_interact);
		BoardEditor::getInstance().m_guiEntityIdle->findComponent("TEXTFIELD_SCRIPT")->setState(1);
	}), PANEL_ALIGN_TOP);

	m_guiEntity->findComponent("DROPDOWN_TYPE")->addItem("Neutral")->addItem("Opponent")->addItem("Trader");


	m_guiSaveMap = new Container("CONTAINER_SAVE_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiSaveMap->addComponent(new Panel("PANEL_BG2", "Save ", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiSaveMap->addComponent(new TextField("TEXTFIELD_WORLD_NAME", "Save to maps\\zones\\<input>", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);
		m_guiSaveMap->addComponent(new CButton("BUTTON_SET", "Save", {-130, 24}, {252, 24}, 1, []() {
			if(BoardEditor::getInstance().m_guiSaveMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				BoardEditor::getInstance().m_map->save(BoardEditor::getInstance().m_guiSaveMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				BoardEditor::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiSaveMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {BoardEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiLoadMap = new Container("CONTAINER_LOAD_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiLoadMap->addComponent(new Panel("PANEL_BG2", "Load ", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new TextField("TEXTFIELD_WORLD_NAME", "Load from maps\\zones\\<input>", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new CButton("BUTTON_SET", "Load", {-130, 24}, {252, 24}, 1, []() {
			if(BoardEditor::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				BoardEditor::getInstance().m_map->load(BoardEditor::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				BoardEditor::getInstance().m_listWorld->clear();
				for(Uint16 i = 0; i < BoardEditor::getInstance().m_map->getWorldObjectSize(); i++)
					BoardEditor::getInstance().m_listWorld->addItem({BoardEditor::getInstance().m_map->getWorldObject(i).m_name, BoardEditor::getInstance().m_map->getWorldObject(i).m_tileTex});
				BoardEditor::getInstance().m_tabEntity->clear();
				for(Uint16 i = 0; i < BoardEditor::getInstance().m_map->getEntitySize(); i++)
					BoardEditor::getInstance().m_tabEntity->addItem({BoardEditor::getInstance().m_map->getEntity(i).m_name, 0});
				BoardEditor::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {BoardEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiClearMap = new Container("CONTAINER_CLEAR_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiClearMap->addComponent(new Panel("PANEL_BG2", "Clear  (Warning: No Undo)", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);

		m_guiClearMap->addComponent(new CButton("BUTTON_SET", "Clear", {-130, 24}, {252, 24}, 1, []() {
			BoardEditor::getInstance().m_map->createNew();
			BoardEditor::getInstance().unpause();
		}), PANEL_ALIGN_CENTER);
		m_guiClearMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {BoardEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiResizeMap = new Container("CONTAINER_RESIZE_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiResizeMap->addComponent(new Panel("PANEL_BG2", "Resize ", {0, -12}, {1220, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CSlider("SLIDER_WIDTH", "Width", {0, -16}, 1024, 1024, 10, 1), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CSlider("SLIDER_HEIGHT", "Height", {0, 4}, 1024, 1024, 10, 1), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CButton("BUTTON_SET", "Resize", {-130, 24}, {252, 24}, 1, []() {
			if(BoardEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_WIDTH")->getValue() != 0 && BoardEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_HEIGHT")->getValue() != 0)
			{
				BoardEditor::getInstance().m_map->resize(Vector2<Uint16>(BoardEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_WIDTH")->getValue(), BoardEditor::getInstance().m_guiResizeMap->findComponent("SLIDER_HEIGHT")->getValue()));
				BoardEditor::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 1, []() {BoardEditor::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiExit = new Container("CONTAINER_EXIT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiExit->addComponent(new Panel("PANEL_BG2", "Exit BoardEditor", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);

		m_guiExit->addComponent(new TextField("TEXTFIELD_WORLD_NAME", " Name", {0, -24}, {512, 1}, 1), PANEL_ALIGN_CENTER);
		m_guiExit->addComponent(new CButton("BUTTON_SET", "Save", {-173, 24}, {166, 24}, 1, []() {
			if(BoardEditor::getInstance().m_guiExit->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				BoardEditor::getInstance().m_map->save(BoardEditor::getInstance().m_guiExit->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				Globals::getInstance().m_exitting = 2;
			}
		}), PANEL_ALIGN_CENTER);
		m_guiExit->addComponent(new CButton("BUTTON_NO_SAVE", "Don't Save", {0, 24}, {166, 24}, 1, []() {Globals::getInstance().m_exitting = 2; }), PANEL_ALIGN_CENTER);
		m_guiExit->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {173, 24}, {166, 24}, 1, []() {
			BoardEditor::getInstance().unpause();
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

void BoardEditor::input()
{
	Editor::input();
}

void BoardEditor::update()
{
	Editor::update();
}
