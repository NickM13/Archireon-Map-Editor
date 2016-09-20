#include "engine\game\Game.h"

#include "engine\gfx\font\Font.h"

#include <functional>
#include <iostream>

bool Game::init()
{
	m_cPauseScreen = -1;

	Font::getInstance().setFont("Fontsheet.png");

	m_zoneMap = new ZoneMap(Vector2<Uint16>(64, 64));

	m_zoneMap->setTextureGround(MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("TilesheetGround.png")));
	m_zoneMap->setTextureWorld(MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("TilesheetWorld.png")));
	m_zoneMap->setTextureSky(MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("TilesheetSky.png")));

	m_zoom = 1;
	m_camPos = Vector2<Sint32>(Sint32(m_zoneMap->getSize().x / 2 * TILE_SIZE), Sint32(m_zoneMap->getSize().y / 2 * TILE_SIZE));

	m_backTexId = MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Background.png"));

	resize();

	m_guiAll = new Container("CONTAINER_ALL", Globals::getInstance().m_screenSize / -2, Globals::getInstance().m_screenSize, true);
	m_guiPause = new Container("CONTAINER_PAUSE", {0, 0}, Globals::getInstance().m_screenSize, false);
	m_guiTop = new Container("CONTAINER_TOP", {0, 0}, {Globals::getInstance().m_screenSize.x, 32}, true);
	m_guiLeft = new ContainerPanel("CONTAINER_LEFT", "Layers", {16, 64}, {266, 149}, 0, true, LTexture::getInstance().loadImage("gui\\BarFlatRound.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_guiRight = new ContainerPanel("CONTAINER_RIGHT", "Pallet", {16, 32}, {288, 640}, 0, true, LTexture::getInstance().loadImage("gui\\BarFlatRound.png"), COMPONENT_TEXTURE_STYLE_SCALE);

	m_guiAll->addComponent(m_guiPause, PANEL_ALIGN_TOP_LEFT)->setPriorityLayer(5);
	m_guiPause->addComponent(new Panel("PANEL_BG", "", {0, 0}, Globals::getInstance().m_screenSize, 4, true), PANEL_ALIGN_CENTER);
	m_guiAll->addComponent(m_guiLeft, PANEL_ALIGN_TOP_LEFT);
	m_guiAll->addComponent(m_guiRight, PANEL_ALIGN_TOP_RIGHT);
	m_guiAll->addComponent(m_guiTop, PANEL_ALIGN_TOP_LEFT)->setPriorityLayer(4);




	m_toolbarMenu = new CToolbar("TOOLBAR_MENU", {0, 0}, {Globals::getInstance().m_screenSize.x, 24}, 0);
	m_toolbarMenu->addButton("", "File");
	{
		m_toolbarMenu->addButton("File", "Save", []() {
			Game::getInstance().m_guiSaveMap->setVisible(true);
			Game::getInstance().m_guiPause->setVisible(true);
			Game::getInstance().pause("CONTAINER_SAVE_MAP");
		});
		m_toolbarMenu->addButton("File", "Load", []() {
			Game::getInstance().m_guiLoadMap->setVisible(true);
			Game::getInstance().m_guiPause->setVisible(true);
			Game::getInstance().pause("CONTAINER_LOAD_MAP");
		});
		m_toolbarMenu->addButton("File", "New", []() {
			Game::getInstance().m_guiClearMap->setVisible(true);
			Game::getInstance().m_guiPause->setVisible(true);
			Game::getInstance().pause("CONTAINER_CLEAR_MAP");
		});
		m_toolbarMenu->addButton("File", "Exit", []() {
			Globals::getInstance().m_exitting = 1;
			Game::getInstance().pause("CONTAINER_EXIT");
		});
	}
	m_toolbarMenu->addButton("", "Edit");
	{
		m_toolbarMenu->addButton("Edit", "Resize", []() {
			Game::getInstance().m_guiResizeMap->findComponent("SLIDER_WIDTH")->setValue(Game::getInstance().m_zoneMap->getSize().x);
			Game::getInstance().m_guiResizeMap->findComponent("SLIDER_HEIGHT")->setValue(Game::getInstance().m_zoneMap->getSize().y);
			Game::getInstance().pause("CONTAINER_RESIZE_MAP");
		});
	}
	m_toolbarMenu->addButton("", "View");
	{
		m_toolbarMenu->addButton("View", "Toggle Grid", []() {
			Game::getInstance().m_showGrid = !Game::getInstance().m_showGrid;
		});
		m_toolbarMenu->addButton("View", "Toggle Overlay", []() {
			Game::getInstance().m_guiLeft->setVisible(!Game::getInstance().m_guiLeft->isVisible());
			Game::getInstance().m_guiRight->setVisible(!Game::getInstance().m_guiRight->isVisible());
		});
	}
	m_toolbarMenu->addButton("", "Help");
	{
		m_toolbarMenu->addButton("Help", "Nothing yet!");
	}
	m_guiTop->addComponent(m_toolbarMenu);



	m_selectLayer = new CButtonRadio("RADIO_LAYER", "Layer", {0, 24}, {264, 200}, 1);
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_GROUND", "Ground", {0, 0}, {264, 24}, 16, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_WORLD", "World", {0, 25}, {264, 24}, 16, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_ENTITY", "Entity", {0, 50}, {264, 24}, 16, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_SKY", "Sky", {0, 75}, {264, 24}, 16, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_STAMPS", "Stamps", {0, 100}, {264, 24}, 16, 1));
	m_guiLeft->addComponent(m_selectLayer, PANEL_ALIGN_TOP);

	m_guiLeft->addComponent(new CButtonToggle("BUTTON_GROUND_VISIBLE", "", MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Visible.png")),
		MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Invisible.png")), {-120, 24}, {24, 24}, 16, 1, 1), PANEL_ALIGN_TOP);
	m_guiLeft->addComponent(new CButtonToggle("BUTTON_WORLD_VISIBLE", "", MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Visible.png")),
		MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Invisible.png")), {-120, 49}, {24, 24}, 16, 1, 1), PANEL_ALIGN_TOP);
	m_guiLeft->addComponent(new CButtonToggle("BUTTON_ENTITY_VISIBLE", "", MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Visible.png")),
		MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Invisible.png")), {-120, 74}, {24, 24}, 16, 1, 1), PANEL_ALIGN_TOP);
	m_guiLeft->addComponent(new CButtonToggle("BUTTON_SKY_VISIBLE", "", MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Visible.png")),
		MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Invisible.png")), {-120, 99}, {24, 24}, 16, 1, 1), PANEL_ALIGN_TOP);



	m_guiRightGround = new Container("CONTAINER_RIGHT_GROUND", {0, 0}, {288, 640}, true);
	m_guiRightWorld = new Container("CONTAINER_RIGHT_WORLD", {0, 0}, {288, 640}, false);
	m_guiRightEntity = new Container("CONTAINER_RIGHT_ENTITY", {0, 0}, {288, 640}, false);
	m_guiRightSky = new Container("CONTAINER_RIGHT_SKY", {0, 0}, {288, 640}, false);
	m_guiRightStamp = new Container("CONTAINER_RIGHT_STAMP", {0, 0}, {288, 640}, false);

	m_guiRight->addComponent(m_guiRightGround);
	m_guiRight->addComponent(m_guiRightWorld);
	m_guiRight->addComponent(m_guiRightEntity);
	m_guiRight->addComponent(m_guiRightSky);
	m_guiRight->addComponent(m_guiRightStamp);

	m_guiRightWorldSwitch = new Container("CONTAINER_RIGHT_WORLD_SWITCH", {0, 0}, {288, Globals::getInstance().m_screenSize.y}, false);
	m_guiRightWorldPortal = new Container("CONTAINER_RIGHT_WORLD_PORTAL", {0, 0}, {288, Globals::getInstance().m_screenSize.y}, false);

	m_guiRightWorld->addComponent(m_guiRightWorldSwitch);
	m_guiRightWorld->addComponent(m_guiRightWorldPortal);



	//m_guiRightGround->addComponent(new TextField("TEXTFIELD_STAMP_NAME", "Stamp1", Vector2<Sint32>(16, 324), Vector2<Sint32>(16, 1), 16, 1));

	m_guiRightWorld->addComponent(new TextField("TEXTFIELD_OBJECT_NAME", "Object Name", Vector2<Sint32>(16, 204), Vector2<Sint32>(16, 1), 16, 1));

	m_guiRightWorldPortal->addComponent(new TextField("TEXTFIELD_PORTAL", "Destination Zone", {8, 416}, {17, 1}, 16, 1));
	m_guiRightWorldPortal->addComponent(new CCounter("COUNTER_PORTAL_X", "X:", {0, 440}, {0, 500}, 1, 0));
	m_guiRightWorldPortal->addComponent(new CCounter("COUNTER_PORTAL_Y", "Y:", {0, 460}, {0, 500}, 1, 0));

	m_dropDownInteract = new CDropDown("DROPDOWN_INTERACT", "Interaction Type", {0, 296}, {264, 32}, 16, 1);
	m_dropDownInteract->addItem("None");
	m_dropDownInteract->addItem("Solid");
	m_dropDownInteract->addItem("Switch");
	m_dropDownInteract->addItem("Solid Switch");
	m_dropDownInteract->addItem("Portal");
	m_guiRightWorld->addComponent(m_dropDownInteract, PANEL_ALIGN_TOP);
	m_guiRightWorld->findComponent("DROPDOWN_INTERACT")->setPriorityLayer(5);

	m_guiRightWorldSwitch->addComponent(new CCounter("COUNTER_SWITCH_FREQUENCY", "Freq.", {0, 440}, {0, 255}, 1, 0));

	m_guiRightWorld->addComponent(new CButton("BUTTON_DELETE", "", MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Trash.png")), {0, 0}, {32, 32}, 16, 1), PANEL_ALIGN_BOTTOM_LEFT);



	m_guiEntityTex = new Container("CONTAINER_ENTITY_TEXTURE", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityTex->addComponent(new Panel("PANEL_BG2", "Set Texture", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityTex->addComponent(new TextField("TEXTFIELD_TEXTURE", "Texture", {0, -24}, {32, 1}, 16, 1), PANEL_ALIGN_CENTER);
		m_guiEntityTex->addComponent(new CButton("BUTTON_SET", "Set", {-130, 24}, {252, 24}, 16, 1, []() {
			if(Game::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->getTitle() != "")
			{
				Game::getInstance().m_zoneMap->getEntity(Game::getInstance().m_listEntity->getSelectedItem()).m_entityTex = MTexture::getInstance().getUnit(LTexture::getInstance().loadImage(Game::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->getTitle()));
				Game::getInstance().m_zoneMap->getEntity(Game::getInstance().m_listEntity->getSelectedItem()).m_entityTex.setName(Game::getInstance().m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->getTitle());
				Game::getInstance().m_tileSetEntity->setTileSheet(Game::getInstance().m_zoneMap->getEntity(Game::getInstance().m_listEntity->getSelectedItem()).m_entityTex);
				Game::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiEntityTex->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 16, 1, []() {Game::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiEntityScript = new Container("CONTAINER_ENTITY_SCRIPT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityScript->addComponent(new Panel("PANEL_BG2", "Entity Script", {0, -12}, Globals::getInstance().m_screenSize - Vector2<Sint32>(256, 128), 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityScript->addComponent(new TextField("TEXTFIELD_SCRIPT", "Go to docs folder for script api", {0, 0}, Globals::getInstance().m_screenSize / 16 - Vector2<Sint32>(17, 11), 16, 1, false), PANEL_ALIGN_CENTER);
		m_guiEntityScript->findComponent("TEXTFIELD_SCRIPT")->setTitle("Test\nING\nSTuff");
		m_guiEntityScript->addComponent(new CButton("BUTTON_SAVE", "Save", {-130, 24}, {252, 24}, 16, 1, []() {
			Game::getInstance().m_zoneMap->getEntity(Game::getInstance().m_listEntity->getSelectedItem()).m_script = Game::getInstance().m_guiEntityScript->findComponent("TEXTFIELD_SCRIPT")->getTitle();
			Game::getInstance().m_tileSetEntity->setTileSheet(Game::getInstance().m_zoneMap->getEntity(Game::getInstance().m_listEntity->getSelectedItem()).m_entityTex);
			Game::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityScript->addComponent(new CButton("BUTTON_CANCEL", "Don't Save", {130, 24}, {252, 24}, 16, 1, []() {Game::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}
	m_guiEntityBoard = new Container("CONTAINER_ENTITY_BOARD", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiEntityBoard->addComponent(new Panel("PANEL_BG2", "Game Board", {0, 0}, Globals::getInstance().m_screenSize - Vector2<Sint32>(256, 128), 0, true), PANEL_ALIGN_CENTER);
		m_guiEntityBoard->addComponent(new CButton("BUTTON_SAVE", "Save", {-130, 24}, {252, 24}, 16, 1, []() {
			Game::getInstance().m_zoneMap->getEntity(Game::getInstance().m_listEntity->getSelectedItem()).m_board->m_boardData = Game::getInstance().m_boardData;
			Game::getInstance().unpause();
		}), PANEL_ALIGN_BOTTOM);
		m_guiEntityBoard->addComponent(new CButton("BUTTON_CANCEL", "Don't Save", {130, 24}, {252, 24}, 16, 1, []() {Game::getInstance().unpause(); }), PANEL_ALIGN_BOTTOM);
	}

	m_guiPause->addComponent(m_guiEntityTex, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiEntityScript, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiEntityBoard, PANEL_ALIGN_CENTER);

	m_pauseScreens.push_back(m_guiEntityTex);
	m_pauseScreens.push_back(m_guiEntityScript);
	m_pauseScreens.push_back(m_guiEntityBoard);



	m_guiRightEntity->addComponent(new TextField("TEXTFIELD_ENTITY_NAME", "Entity Name", Vector2<Sint32>(16, 204), Vector2<Sint32>(16, 1), 16, 1));

	m_guiRightEntity->addComponent(new CButton("BUTTON_ENTITY_BOARD", "Edit Board", {0, 228}, {264, 32}, 16, 1), PANEL_ALIGN_TOP);
	m_guiRightEntity->addComponent(new CButton("BUTTON_ENTITY_TEXTURE", "Set Texture", {0, 264}, {264, 32}, 16, 1), PANEL_ALIGN_TOP);
	m_guiRightEntity->addComponent(new CButton("BUTTON_ENTITY_SCRIPT", "Edit Script", {0, 300}, {264, 32}, 16, 1), PANEL_ALIGN_TOP);

	m_guiRightEntity->addComponent(new CButton("BUTTON_DELETE", "", MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Trash.png")), {0, 0}, {32, 32}, 16, 1), PANEL_ALIGN_BOTTOM_LEFT);



	m_guiRightStamp->addComponent(new TextField("TEXTFIELD_STAMP_NAME", "Stamp Name", Vector2<Sint32>(8, 20), Vector2<Sint32>(17, 1), 16, 1));

	m_guiRightStamp->addComponent(new CButtonToggle("BUTTON_USE_GROUND", "Use Ground", Vector2<Sint32>(0, 360), Vector2<Sint32>(264, 32), 16, 1), PANEL_ALIGN_TOP)->setState(1);
	m_guiRightStamp->addComponent(new CButtonToggle("BUTTON_USE_WORLD", "Use World", Vector2<Sint32>(0, 400), Vector2<Sint32>(264, 32), 16, 1), PANEL_ALIGN_TOP)->setState(1);
	m_guiRightStamp->addComponent(new CButtonToggle("BUTTON_USE_ENTITY", "Use Entity", Vector2<Sint32>(0, 440), Vector2<Sint32>(264, 32), 16, 1), PANEL_ALIGN_TOP)->setState(1);
	m_guiRightStamp->addComponent(new CButtonToggle("BUTTON_USE_SKY", "Use Sky", Vector2<Sint32>(0, 480), Vector2<Sint32>(264, 32), 16, 1), PANEL_ALIGN_TOP)->setState(1);

	m_guiRightStamp->addComponent(new CButton("BUTTON_DELETE", "", MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("Trash.png")), {0, 0}, {32, 32}, 16, 1), PANEL_ALIGN_BOTTOM_LEFT);



	m_guiLeftGround = new Container("CONTAINER_LEFT_GROUND", {0, 0}, {288, Globals::getInstance().m_screenSize.y}, true);
	m_guiLeftWorld = new Container("CONTAINER_LEFT_WORLD", {0, 0}, {288, Globals::getInstance().m_screenSize.y}, false);
	m_guiLeftEntity = new Container("CONTAINER_LEFT_ENTITY", {0, 0}, {288, Globals::getInstance().m_screenSize.y}, false);
	m_guiLeftSky = new Container("CONTAINER_LEFT_SKY", {0, 0}, {288, Globals::getInstance().m_screenSize.y}, false);
	m_guiLeftStamp = new Container("CONTAINER_LEFT_STAMP", {0, 0}, {288, Globals::getInstance().m_screenSize.y}, false);

	m_guiLeft->addComponent(m_guiLeftGround);
	m_guiLeft->addComponent(m_guiLeftWorld);
	m_guiLeft->addComponent(m_guiLeftEntity);
	m_guiLeft->addComponent(m_guiLeftSky);
	m_guiLeft->addComponent(m_guiLeftStamp);



	m_guiSaveMap = new Container("CONTAINER_SAVE_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiSaveMap->addComponent(new Panel("PANEL_BG2", "Save Zone", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiSaveMap->addComponent(new TextField("TEXTFIELD_WORLD_NAME", "Zone Name", {0, -24}, {32, 1}, 16, 1), PANEL_ALIGN_CENTER);
		m_guiSaveMap->addComponent(new CButton("BUTTON_SAVE", "Save", {-130, 24}, {252, 24}, 16, 1, []() {
			if(Game::getInstance().m_guiSaveMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				Game::getInstance().m_zoneMap->save(Game::getInstance().m_guiSaveMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				Game::getInstance().unpause();
				Game::getInstance().m_guiSaveMap->findComponent("BUTTON_SAVE")->setState(0);
			}
		}), PANEL_ALIGN_CENTER);
		m_guiSaveMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 16, 1, []() {Game::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiLoadMap = new Container("CONTAINER_LOAD_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiLoadMap->addComponent(new Panel("PANEL_BG2", "Load Zone", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new TextField("TEXTFIELD_WORLD_NAME", "Zone Name", {0, -24}, {32, 1}, 16, 1), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new CButton("BUTTON_LOAD", "Load", {-130, 24}, {252, 24}, 16, 1, []() {
			if(Game::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				Game::getInstance().m_zoneMap->load(Game::getInstance().m_guiLoadMap->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				Game::getInstance().m_listWorld->clear();
				for(Uint16 i = 0; i < Game::getInstance().m_zoneMap->getWorldObjectSize(); i++)
					Game::getInstance().m_listWorld->addItem({Game::getInstance().m_zoneMap->getWorldObject(i).m_name, Game::getInstance().m_zoneMap->getWorldObject(i).m_tileTex});
				Game::getInstance().m_listEntity->clear();
				for(Uint16 i = 0; i < Game::getInstance().m_zoneMap->getEntitySize(); i++)
					Game::getInstance().m_listEntity->addItem({Game::getInstance().m_zoneMap->getEntity(i).m_name, 0});
				Game::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiLoadMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 16, 1, []() {Game::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiClearMap = new Container("CONTAINER_CLEAR_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiClearMap->addComponent(new Panel("PANEL_BG2", "Clear Zone (Warning: No Undo)", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);

		m_guiClearMap->addComponent(new CButton("BUTTON_CLEAR", "Clear", {-130, 24}, {252, 24}, 16, 1, []() {
			Game::getInstance().m_zoneMap->createNew();
			Game::getInstance().unpause();
		}), PANEL_ALIGN_CENTER);
		m_guiClearMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 16, 1, []() {Game::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiResizeMap = new Container("CONTAINER_RESIZE_MAP", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiResizeMap->addComponent(new Panel("PANEL_BG2", "Resize Zone", {0, -12}, {1220, 112}, 0, true), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CSlider("SLIDER_WIDTH", "Width", {0, -16}, 1024, 1024, 10, 1), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CSlider("SLIDER_HEIGHT", "Height", {0, 4}, 1024, 1024, 10, 1), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CButton("BUTTON_RESIZE", "Resize", {-130, 24}, {252, 24}, 16, 1, []() {
			if(Game::getInstance().m_guiResizeMap->findComponent("SLIDER_WIDTH")->getValue() != 0 && Game::getInstance().m_guiResizeMap->findComponent("SLIDER_HEIGHT")->getValue() != 0)
			{
				Game::getInstance().m_zoneMap->resize(Vector2<Uint16>(Game::getInstance().m_guiResizeMap->findComponent("SLIDER_WIDTH")->getValue(), Game::getInstance().m_guiResizeMap->findComponent("SLIDER_HEIGHT")->getValue()));
				Game::getInstance().unpause();
			}
		}), PANEL_ALIGN_CENTER);
		m_guiResizeMap->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {130, 24}, {252, 24}, 16, 1, []() {Game::getInstance().unpause(); }), PANEL_ALIGN_CENTER);
	}
	m_guiExit = new Container("CONTAINER_EXIT", {0, 0}, Globals::getInstance().m_screenSize, false);
	{
		m_guiExit->addComponent(new Panel("PANEL_BG2", "Exit Editor", {0, -12}, {534, 112}, 0, true), PANEL_ALIGN_CENTER);

		m_guiExit->addComponent(new TextField("TEXTFIELD_WORLD_NAME", "Zone Name", {0, -24}, {32, 1}, 16, 1), PANEL_ALIGN_CENTER);
		m_guiExit->addComponent(new CButton("BUTTON_SAVE", "Save", {-173, 24}, {166, 24}, 16, 1, []() {
			if(Game::getInstance().m_guiExit->findComponent("TEXTFIELD_WORLD_NAME")->getTitle() != "")
			{
				Game::getInstance().m_zoneMap->save(Game::getInstance().m_guiExit->findComponent("TEXTFIELD_WORLD_NAME")->getTitle());
				Globals::getInstance().m_exitting = 2;
			}
		}), PANEL_ALIGN_CENTER);
		m_guiExit->addComponent(new CButton("BUTTON_NO_SAVE", "Don't Save", {0, 24}, {166, 24}, 16, 1, []() {Globals::getInstance().m_exitting = 2; }), PANEL_ALIGN_CENTER);
		m_guiExit->addComponent(new CButton("BUTTON_CANCEL", "Cancel", {173, 24}, {166, 24}, 16, 1, []() {
			Game::getInstance().unpause();
			Globals::getInstance().m_exitting = 0;
		}), PANEL_ALIGN_CENTER);
	}

	m_guiPause->addComponent(m_guiSaveMap, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiLoadMap, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiClearMap, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiResizeMap, PANEL_ALIGN_CENTER);
	m_guiPause->addComponent(m_guiExit, PANEL_ALIGN_CENTER);

	m_pauseScreens.push_back(m_guiSaveMap);
	m_pauseScreens.push_back(m_guiLoadMap);
	m_pauseScreens.push_back(m_guiClearMap);
	m_pauseScreens.push_back(m_guiResizeMap);
	m_pauseScreens.push_back(m_guiExit);



	m_listEntity = new CList("LIST_ENTITY", "Entity List", {16, 60}, {256, 4}, 32, Texture(), 1);
	m_listEntity->addItem({"None", 0});
	m_zoneMap->addEntity(ZoneMap::Entity("None", Texture()));
	m_guiRightEntity->addComponent(m_listEntity);

	m_listStamps = new CList("LIST_STAMPS", "Stamp List", {16, 208}, {256, 4}, 32, Texture(), 1);
	m_listStamps->addItem({"None", 0});
	m_stamps.push_back(Stamp());
	m_guiRightStamp->addComponent(m_listStamps);

	m_tileSetGround = new CTileSet("TILESET_GROUND", "Ground Tile Set", {0, 360}, {256, 256}, 32, m_zoneMap->getTextureGround(), 1);
	m_tileSetWorld = new CTileSet("TILESET_WORLD", "World Tile Set", {0, 360}, {256, 256}, 32, m_zoneMap->getTextureWorld(), 1);
	m_tileSetEntity = new CTileSet("TILESET_ENTITY", "Sprite Sheet", {0, 360}, {256, 256}, 32, Texture(), 1);
	m_tileSetSky = new CTileSet("TILESET_SKY", "Sky Tile Set", {0, 60}, {256, 256}, 32, m_zoneMap->getTextureSky(), 1);
	m_tileSetStamps = new CTileSet("TILESET_WORLD", "TODO: Preview", {0, 88}, {256, 256}, 32, Texture(), 1);
	m_guiRightGround->addComponent(m_tileSetGround, PANEL_ALIGN_TOP);
	m_guiRightWorld->addComponent(m_tileSetWorld, PANEL_ALIGN_TOP);
	m_guiRightEntity->addComponent(m_tileSetEntity, PANEL_ALIGN_TOP);
	m_guiRightSky->addComponent(m_tileSetSky, PANEL_ALIGN_TOP);
	m_guiRightStamp->addComponent(m_tileSetStamps, PANEL_ALIGN_TOP);

	m_listWorld = new CList("LIST_WORLD", "World Obj List", {16, 60}, {256, 4}, 32, m_zoneMap->getTextureWorld(), 1);
	m_listWorld->addItem({"None", 0});
	m_zoneMap->addWorldObject(ZoneMap::WorldObject("None", 0, 0));
	m_guiRightWorld->addComponent(m_listWorld);

	//m_guiTop->addComponent(new CText("TEXT_POS", "1", {0, 0}, {100, 100}, 16, ALIGN_LEFT, Color(255, 255, 255, 255)));

	m_selectStart = {-1, -1};

	return true;
}

void Game::resize()
{
	m_tileMapArea = Rect(-GLfloat(Globals::getInstance().m_screenSize.x / 2), -GLfloat(Globals::getInstance().m_screenSize.y / 2), GLfloat(Globals::getInstance().m_screenSize.x / 2), GLfloat(Globals::getInstance().m_screenSize.y / 2));
	m_boardArea = Rect(-GLfloat(Globals::getInstance().m_screenSize.x / 2) + 136, -GLfloat(Globals::getInstance().m_screenSize.y / 2) + 96, GLfloat(Globals::getInstance().m_screenSize.x / 2) - 136, GLfloat(Globals::getInstance().m_screenSize.y / 2) - 72);
}

std::string Game::getZoneName()
{
	return m_zoneMap->getName();
}

void Game::unpause()
{
	if(m_cPauseScreen != -1)
	{
		m_guiPause->setVisible(false);
		m_pauseScreens[m_cPauseScreen]->setVisible(false);
		m_cPauseScreen = -1;
	}
}

void Game::pause(std::string p_screen)
{
	Uint16 i = 0;
	for(i = 0; i < m_pauseScreens.size(); i++)
		if(m_pauseScreens[i]->getName() == p_screen)
		{
			m_guiPause->setVisible(true);
			if(m_cPauseScreen != -1)
				m_pauseScreens[m_cPauseScreen]->setVisible(false);
			m_cPauseScreen = i;
			m_pauseScreens[m_cPauseScreen]->setVisible(true);
			return;
		}
	unpause();
}

std::string Game::getPause()
{
	if(m_cPauseScreen == -1)
		return "";
	return m_pauseScreens[m_cPauseScreen]->getName();
}

void Game::input()
{
	Vector2<Sint32> _mousePos = Globals::getInstance().m_mousePos;
	Sint8 _rValue = 0;
	m_guiAll->input(_rValue, Globals::getInstance().m_keyStates, Globals::getInstance().m_mouseStates, _mousePos);

	if((_rValue & 2) != 2 && Globals::getInstance().m_keyStates[GLFW_KEY_G] == 1)
		m_showGrid = !m_showGrid;

	if(Globals::getInstance().m_mouseStates[0] == 1)
	{
		if(getPause() == "CONTAINER_ENTITY_BOARD")
			if(m_mouseInBoardArea)
				m_blmbDown = true;
			else if(m_mouseInArea)
			{
				m_lmbDown = true;
				if(m_selectLayer->getSelectedButton() == 4)
					m_selectStart = {Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1), Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1)};
			}
	}
	if(Globals::getInstance().m_mouseStates[1] == 1)
	{
		if(getPause() == "CONTAINER_ENTITY_BOARD")
			if(m_boardArea.checkPoint(GLfloat(m_mouseBuffer.x), GLfloat(m_mouseBuffer.y)))
				m_brmbDown = true;
			else if(m_tileMapArea.checkPoint(GLfloat(m_mouseBuffer.x), GLfloat(m_mouseBuffer.y)))
			{
				m_rmbDown = true;
			}
	}
	if(Globals::getInstance().m_mouseStates[0] == 0)
	{
		m_lmbDown = false;
		m_blmbDown = false;
	}
	if(Globals::getInstance().m_mouseStates[1] == 0)
	{
		m_rmbDown = false;
		m_brmbDown = false;
	}
	if(getPause() == "CONTAINER_ENTITY_BOARD")
	{
		if(m_blmbDown && !m_brmbDown && m_mouseInBoardArea)
		{
			m_boardData[Sint32(m_mouseBuffer.x + m_boardScroll.x - m_boardArea.x) / TILE_SIZE][Sint32(m_mouseBuffer.y + m_boardScroll.y - m_boardArea.y) / TILE_SIZE] = 1;
		}
		else if(m_brmbDown)
		{
			m_boardScroll = m_boardScroll + (m_mouseBuffer - _mousePos);
			if(m_boardScroll.x < -TILE_SIZE * 4)
				m_boardScroll.x = -TILE_SIZE * 4;
			if(m_boardScroll.y < -TILE_SIZE * 4)
				m_boardScroll.y = -TILE_SIZE * 4;
		}
	}
	else if((_rValue & 1) == 0 && m_lmbDown && !m_rmbDown && m_mouseInArea)
	{
		switch(m_selectLayer->getSelectedButton())
		{
		case 0:
			m_zoneMap->setTile(0, Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1), Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1), m_tileSetGround->getSelectedTile());
			break;
		case 1:
			m_zoneMap->setTile(1, Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1), Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1), m_listWorld->getSelectedItem());
			break;
		case 2:
			m_zoneMap->setTile(2, Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1), Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1), m_listEntity->getSelectedItem());
			break;
		case 3:
			m_zoneMap->setTile(3, Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1), Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1), m_tileSetSky->getSelectedTile());
			break;
		case 4:
			if(m_listStamps->getSelectedItem() == 0)
			{
				m_selectEnd = {Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1), Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1)};
			}
			else
			{
				Stamp _selected = m_stamps[m_listStamps->getSelectedItem()];

				for(Sint32 x = Sint32(ceil(-GLfloat(_selected.m_size.x) / 2)); x < Sint32(ceil(GLfloat(_selected.m_size.x) / 2)); x++)
				{
					for(Sint32 y = Sint32(ceil(-GLfloat(_selected.m_size.y) / 2)); y < Sint32(ceil(GLfloat(_selected.m_size.y) / 2)); y++)
					{
						if(m_guiRightStamp->findComponent("BUTTON_USE_GROUND")->isSelected() != 0)
							m_zoneMap->setTile(0, Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1) + x, Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1) + y, _selected.m_ground[x - Sint32(ceil(-GLfloat(_selected.m_size.x) / 2))][y - Sint32(ceil(-GLfloat(_selected.m_size.y) / 2))]);
						if(m_guiRightStamp->findComponent("BUTTON_USE_WORLD")->isSelected() != 0)
							m_zoneMap->setTile(1, Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1) + x, Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1) + y, _selected.m_world[x - Sint32(ceil(-GLfloat(_selected.m_size.x) / 2))][y - Sint32(ceil(-GLfloat(_selected.m_size.y) / 2))]);
						if(m_guiRightStamp->findComponent("BUTTON_USE_ENTITY")->isSelected() != 0)
							m_zoneMap->setTile(2, Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1) + x, Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1) + y, _selected.m_entity[x - Sint32(ceil(-GLfloat(_selected.m_size.x) / 2))][y - Sint32(ceil(-GLfloat(_selected.m_size.y) / 2))]);
						if(m_guiRightStamp->findComponent("BUTTON_USE_SKY")->isSelected() != 0)
							m_zoneMap->setTile(3, Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1) + x, Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1) + y, _selected.m_sky[x - Sint32(ceil(-GLfloat(_selected.m_size.x) / 2))][y - Sint32(ceil(-GLfloat(_selected.m_size.y) / 2))]);
					}
				}
			}
			break;
		}
	}
	else if(m_rmbDown && (_rValue & 1) == 0)
	{
		m_camPos = m_camPos + (m_mouseBuffer - _mousePos);
		if(m_camPos.x < TILE_SIZE)
			m_camPos.x = TILE_SIZE;
		if(m_camPos.y < TILE_SIZE)
			m_camPos.y = TILE_SIZE;

		if(m_camPos.x > m_zoneMap->getSize().x * TILE_SIZE + TILE_SIZE)
			m_camPos.x = GLfloat(m_zoneMap->getSize().x * TILE_SIZE + TILE_SIZE);

		if(m_camPos.y > m_zoneMap->getSize().y * TILE_SIZE + TILE_SIZE)
			m_camPos.y = GLfloat(m_zoneMap->getSize().y * TILE_SIZE + TILE_SIZE);
	}

	m_mouseBuffer = _mousePos;
	m_mouseInArea = ((_rValue & 1) == 0
		&& m_tileMapArea.checkPoint(GLfloat(m_mouseBuffer.x), GLfloat(m_mouseBuffer.y))
		&& (m_mouseBuffer.x + m_camPos.x >= TILE_SIZE) && (m_mouseBuffer.y + m_camPos.y >= TILE_SIZE)
		&& (m_mouseBuffer.x + m_camPos.x < TILE_SIZE * (m_zoneMap->getSize().x + 1)) && (m_mouseBuffer.y + m_camPos.y < TILE_SIZE * (m_zoneMap->getSize().y + 1)));

	m_mouseInBoardArea = (m_boardArea.checkPoint(GLfloat(m_mouseBuffer.x), GLfloat(m_mouseBuffer.y))
		&& (m_mouseBuffer.x + m_boardScroll.x - m_boardArea.x >= 0) && (m_mouseBuffer.y + m_boardScroll.y - m_boardArea.y >= 0)
		&& (m_mouseBuffer.x + m_boardScroll.x - m_boardArea.x < TILE_SIZE * (m_boardSize.x)) && (m_mouseBuffer.y + m_boardScroll.y - m_boardArea.y < TILE_SIZE * (m_boardSize.y)));

	m_zoneMap->setLayerVisible(0, m_guiLeft->findComponent("BUTTON_GROUND_VISIBLE")->isSelected() != 0);
	m_zoneMap->setLayerVisible(1, m_guiLeft->findComponent("BUTTON_WORLD_VISIBLE")->isSelected() != 0);
	m_zoneMap->setLayerVisible(2, m_guiLeft->findComponent("BUTTON_ENTITY_VISIBLE")->isSelected() != 0);
	m_zoneMap->setLayerVisible(3, m_guiLeft->findComponent("BUTTON_SKY_VISIBLE")->isSelected() != 0);
}

void Game::update()
{
	if(Globals::getInstance().m_keyStates[GLFW_KEY_ESCAPE] == 3)
	{
		if(m_cPauseScreen != -1)
		{
			unpause();
			if(Globals::getInstance().m_exitting == 1)
				Globals::getInstance().m_exitting = 0;
		}
		else
		{
			Globals::getInstance().m_exitting = 1;
			pause("CONTAINER_EXIT");
		}
	}

	m_deltaUpdate = GLfloat(glfwGetTime() - m_lastUpdate);
	m_lastUpdate = GLfloat(glfwGetTime());

	m_guiAll->update(m_deltaUpdate);

	if(m_selectLayer->getPrevSelectedButton() != m_selectLayer->getSelectedButton())
	{
		switch(m_selectLayer->getSelectedButton())
		{
		case 0:
			m_guiLeftGround->setVisible(true);
			m_guiRightGround->setVisible(true);
			break;
		case 1:
			m_guiLeftWorld->setVisible(true);
			m_guiRightWorld->setVisible(true);
			break;
		case 2:
			m_guiLeftEntity->setVisible(true);
			m_guiRightEntity->setVisible(true);
			break;
		case 3:
			m_guiLeftSky->setVisible(true);
			m_guiRightSky->setVisible(true);
			break;
		case 4:
			m_guiLeftStamp->setVisible(true);
			m_guiRightStamp->setVisible(m_listStamps->getSelectedItem() != 0);
			break;
		}

		switch(m_selectLayer->getPrevSelectedButton())
		{
		case 0:
			m_guiLeftGround->setVisible(false);
			m_guiRightGround->setVisible(false);
			break;
		case 1:
			m_guiLeftWorld->setVisible(false);
			m_guiRightWorld->setVisible(false);
			break;
		case 2:
			m_guiLeftEntity->setVisible(false);
			m_guiRightEntity->setVisible(false);
			break;
		case 3:
			m_guiLeftSky->setVisible(false);
			m_guiRightSky->setVisible(false);
			break;
		case 4:
			m_guiLeftStamp->setVisible(false);
			m_guiRightStamp->setVisible(false);
			m_selectStart = {-1, -1};
			break;
		}
	}

	switch(m_selectLayer->getSelectedButton())
	{
	case 1: // WORLD OBJECTS
		if(m_listWorld->isUpdated() != 0)
		{
			m_guiRightWorld->setVisible(true);
			if((m_listWorld->isUpdated() & 2) == 2) // New item
			{
				m_zoneMap->addWorldObject({m_listWorld->getItem(m_listWorld->getSelectedItem()).m_name, 1, m_listWorld->getItem(m_listWorld->getSelectedItem()).m_texId});
			}
			if((m_listWorld->isUpdated() & 1) == 1) // Switch item
			{
				m_guiRightWorld->findComponent("TEXTFIELD_OBJECT_NAME")->setTitle(m_listWorld->getItem(m_listWorld->getSelectedItem()).m_name);
				m_dropDownInteract->setSelectedItem(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_interactionType);
				m_tileSetWorld->setSelectedTile(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_tileTex);

				m_guiRightWorldPortal->findComponent("TEXTFIELD_PORTAL")->setTitle(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_portalDest);
				m_guiRightWorldPortal->findComponent("COUNTER_PORTAL_X")->setValue(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_destX);
				m_guiRightWorldPortal->findComponent("COUNTER_PORTAL_Y")->setValue(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_destY);
				m_guiRightWorldSwitch->findComponent("COUNTER_SWITCH_FREQUENCY")->setValue(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			}
		}
		else if(m_guiRightWorld->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_listWorld->getSelectedItem() != 0 && m_listWorld->getSelectedItem() < m_listWorld->getListSize())
		{
			m_listWorld->removeItem(m_listWorld->getSelectedItem());
			m_zoneMap->removeWorldObject(m_listWorld->getSelectedItem() + 1);

			m_guiRightWorld->findComponent("TEXTFIELD_OBJECT_NAME")->setTitle(m_listWorld->getItem(m_listWorld->getSelectedItem()).m_name);
			m_dropDownInteract->setSelectedItem(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_interactionType);
			m_tileSetWorld->setSelectedTile(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_tileTex);

			m_guiRightWorldPortal->findComponent("TEXTFIELD_PORTAL")->setTitle(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_portalDest);
			m_guiRightWorldPortal->findComponent("COUNTER_PORTAL_X")->setValue(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_destX);
			m_guiRightWorldPortal->findComponent("COUNTER_PORTAL_Y")->setValue(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_destY);
			m_guiRightWorldSwitch->findComponent("COUNTER_SWITCH_FREQUENCY")->setValue(m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
		}
		if(m_listWorld->getSelectedItem() != 0)
		{
			if(m_guiRightWorld->findComponent("TEXTFIELD_OBJECT_NAME")->getTitle() != "")
				m_listWorld->getItem(m_listWorld->getSelectedItem()).m_name = m_guiRightWorld->findComponent("TEXTFIELD_OBJECT_NAME")->getTitle();
			m_listWorld->getItem(m_listWorld->getSelectedItem()).m_texId = m_tileSetWorld->getSelectedTile();
			m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_interactionType = m_dropDownInteract->getSelectedItem();
			m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_tileTex = m_tileSetWorld->getSelectedTile();
			if(m_dropDownInteract->getItem(m_dropDownInteract->getSelectedItem()) == "Portal")
			{
				m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_portalDest = m_guiRightWorldPortal->findComponent("TEXTFIELD_PORTAL")->getTitle();
				m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_destX = m_guiRightWorldPortal->findComponent("COUNTER_PORTAL_X")->getValue();
				m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_destY = m_guiRightWorldPortal->findComponent("COUNTER_PORTAL_Y")->getValue();
			}
			else if(m_dropDownInteract->getItem(m_dropDownInteract->getSelectedItem()) == "Switch" || m_dropDownInteract->getItem(m_dropDownInteract->getSelectedItem()) == "Solid Switch")
			{
				m_zoneMap->getWorldObject(m_listWorld->getSelectedItem()).m_frequency = m_guiRightWorldSwitch->findComponent("COUNTER_SWITCH_FREQUENCY")->getValue();
			}
		}
		if(m_dropDownInteract->isUpdated())
		{
			m_guiRightWorldPortal->setVisible(m_dropDownInteract->getItem(m_dropDownInteract->getSelectedItem()) == "Portal");
			m_guiRightWorldSwitch->setVisible(m_dropDownInteract->getItem(m_dropDownInteract->getSelectedItem()) == "Switch" ||
				m_dropDownInteract->getItem(m_dropDownInteract->getSelectedItem()) == "Solid Switch");
		}
		break;
	case 2: // ENTITIES
		if(m_listEntity->isUpdated() != 0)
		{
			if((m_listEntity->isUpdated() & 2) == 2) // New item
				m_zoneMap->addEntity(ZoneMap::Entity(m_listEntity->getItem(m_listEntity->getSelectedItem()).m_name));
			if((m_listEntity->isUpdated() & 1) == 1) // Switch item
			{
				m_guiRightEntity->findComponent("TEXTFIELD_ENTITY_NAME")->setTitle(m_listEntity->getItem(m_listEntity->getSelectedItem()).m_name);
				if(m_listEntity->getSelectedItem() != 0)
					m_tileSetEntity->setTileSheet(m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_entityTex);
				else
					m_tileSetEntity->setTileSheet(Texture());
				m_tileSetEntity->setSelectedTile(m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_entityTexId);
			}
		}
		else if(m_guiRightEntity->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_listEntity->getSelectedItem() != 0 && m_listEntity->getSelectedItem() < m_listEntity->getListSize())
		{
			m_listEntity->removeItem(m_listEntity->getSelectedItem());
			m_zoneMap->removeEntity(m_listEntity->getSelectedItem() + 1);

			m_guiRightEntity->findComponent("TEXTFIELD_ENTITY_NAME")->setTitle(m_listEntity->getItem(m_listEntity->getSelectedItem()).m_name);
			m_tileSetEntity->setTileSheet(m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_entityTex);
			m_tileSetEntity->setSelectedTile(m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_entityTexId);
		}
		if(m_listEntity->getSelectedItem() != 0)
		{
			if(m_guiRightEntity->findComponent("TEXTFIELD_ENTITY_NAME")->getTitle() != "")
				m_listEntity->getItem(m_listEntity->getSelectedItem()).m_name = m_guiRightEntity->findComponent("TEXTFIELD_ENTITY_NAME")->getTitle();
			m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_entityTexId = m_tileSetEntity->getSelectedTile();
			if(m_guiRightEntity->findComponent("BUTTON_ENTITY_BOARD")->isSelected() == 3)
			{
				pause("CONTAINER_ENTITY_BOARD");
				if(m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_board == 0)
					m_zoneMap->getEntity(m_listEntity->getSelectedItem()).setBoard({16, 16}, Texture());
				m_boardSize = m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_board->m_size;
				m_boardData = m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_board->m_boardData;
			}
			if(m_guiRightEntity->findComponent("BUTTON_ENTITY_TEXTURE")->isSelected() == 3)
			{
				pause("CONTAINER_ENTITY_TEXTURE");
				m_guiEntityTex->findComponent("TEXTFIELD_TEXTURE")->setTitle(m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_entityTex.getName());
			}
			if(m_guiRightEntity->findComponent("BUTTON_ENTITY_SCRIPT")->isSelected() == 3)
			{
				pause("CONTAINER_ENTITY_SCRIPT");
				m_guiEntityScript->findComponent("TEXTFIELD_SCRIPT")->setTitle(m_zoneMap->getEntity(m_listEntity->getSelectedItem()).m_script);
				m_guiEntityScript->findComponent("TEXTFIELD_SCRIPT")->setState(1);
			}
		}
		break;
	case 3:

		break;
	case 4: // STAMPS
		if(m_listStamps->isUpdated() != 0)
		{
			if((m_listStamps->isUpdated() & 2) == 2) // New item
			{
				m_stamps.push_back(Stamp());
			}
			if((m_listStamps->isUpdated() & 1) == 1) // Switch item
			{
				m_guiRightStamp->findComponent("TEXTFIELD_STAMP_NAME")->setTitle(m_listStamps->getItem(m_listStamps->getSelectedItem()).m_name);
				m_selectStart = {-1, -1};
			}
			m_guiRightStamp->setVisible(m_listStamps->getSelectedItem() != 0);
		}
		else if(m_guiRightStamp->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_listStamps->getSelectedItem() != 0 && m_listStamps->getSelectedItem() < m_listStamps->getListSize())
		{
			m_stamps.erase(m_stamps.begin() + m_listStamps->getSelectedItem());
			m_listStamps->removeItem(m_listStamps->getSelectedItem());

			m_guiRightStamp->findComponent("TEXTFIELD_STAMP_NAME")->setTitle(m_listStamps->getItem(m_listStamps->getSelectedItem()).m_name);
			m_selectStart = {-1, -1};

			m_guiRightStamp->setVisible(m_listStamps->getSelectedItem() != 0);
		}
		else if(m_listStamps->getSelectedItem() != 0)
		{
			if(m_guiRightStamp->findComponent("TEXTFIELD_STAMP_NAME")->getTitle() != "")
				m_listStamps->getItem(m_listStamps->getSelectedItem()).m_name = m_guiRightStamp->findComponent("TEXTFIELD_STAMP_NAME")->getTitle();
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
					_groundData[x][y] = m_zoneMap->getTile(0, x + _topLeft.x, y + _topLeft.y);
					_worldData[x][y] = m_zoneMap->getTile(1, x + _topLeft.x, y + _topLeft.y);
					_entityData[x][y] = m_zoneMap->getTile(2, x + _topLeft.x, y + _topLeft.y);
					_skyData[x][y] = m_zoneMap->getTile(3, x + _topLeft.x, y + _topLeft.y);
				}
			}
			m_stamps[m_stamps.size() - 1] = Stamp(_stampSize, _groundData, _worldData, _entityData, _skyData);
			m_listStamps->addItem({std::string("Item ") + Util::numToString(m_listStamps->getListSize()), 0});
		}
		break;
	}

	/*if(m_mouseInArea)
		m_guiTop->findComponent("TEXT_POS")->setTitle("Select Pos\nX:" + Util::numToString(Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1)) + "\nY:" + Util::numToString(Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1)));
	else
		m_guiTop->findComponent("TEXT_POS")->setTitle("Select Pos\nX:NA\nY:NA");
	*/

	m_zoneMap->setViewSize(m_tileMapArea);
	m_zoneMap->setTileSize(TILE_SIZE);
	m_zoneMap->setGridVisible(m_showGrid);
}

void Game::render()
{
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, m_backTexId.getId());
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 1);
		glVertex2f(m_tileMapArea.x, m_tileMapArea.y);
		glTexCoord2f(1, 1);
		glVertex2f(m_tileMapArea.w, m_tileMapArea.y);
		glTexCoord2f(1, 0);
		glVertex2f(m_tileMapArea.w, m_tileMapArea.h);
		glTexCoord2f(0, 0);
		glVertex2f(m_tileMapArea.x, m_tileMapArea.h);
	}
	glEnd();

	glPushMatrix();
	{
		m_zoneMap->render(m_camPos);


		Vector2<Sint32> _topLeft, _botRight;
		_topLeft = Vector2<Sint32>(min(m_selectStart.x * TILE_SIZE, m_selectEnd.x * TILE_SIZE), min(m_selectStart.y * TILE_SIZE, m_selectEnd.y * TILE_SIZE));
		_botRight = Vector2<Sint32>(max(m_selectStart.x * TILE_SIZE, m_selectEnd.x * TILE_SIZE), max(m_selectStart.y * TILE_SIZE, m_selectEnd.y * TILE_SIZE)) + TILE_SIZE;

		//Selection view
		if(m_selectLayer->getSelectedButton() == 4)
		{
			glPushMatrix();
			{
				if(m_listStamps->getSelectedItem() == 0 && m_selectStart.x != -1)
				{
					glTranslatef(-m_camPos.x + TILE_SIZE, -m_camPos.y + TILE_SIZE, 0);
					glColor4f(0.2f, 0.6f, 1.f, 1.f);
					glBegin(GL_LINES);
					{
						glVertex2f(GLfloat(_topLeft.x), GLfloat(_topLeft.y));
						glVertex2f(GLfloat(_botRight.x), GLfloat(_topLeft.y));

						glVertex2f(GLfloat(_botRight.x), GLfloat(_topLeft.y));
						glVertex2f(GLfloat(_botRight.x), GLfloat(_botRight.y));

						glVertex2f(GLfloat(_botRight.x), GLfloat(_botRight.y));
						glVertex2f(GLfloat(_topLeft.x), GLfloat(_botRight.y));

						glVertex2f(GLfloat(_topLeft.x), GLfloat(_botRight.y));
						glVertex2f(GLfloat(_topLeft.x), GLfloat(_topLeft.y));
					}
					glEnd();
					glColor4f(0.2f, 0.6f, 1.f, 0.2f);
					glBegin(GL_QUADS);
					{
						glVertex2f(GLfloat(_topLeft.x), GLfloat(_topLeft.y));
						glVertex2f(GLfloat(_botRight.x), GLfloat(_topLeft.y));
						glVertex2f(GLfloat(_botRight.x), GLfloat(_botRight.y));
						glVertex2f(GLfloat(_topLeft.x), GLfloat(_botRight.y));
					}
					glEnd();
				}
				else
				{
					Vector2<Sint32> _size = m_stamps[m_listStamps->getSelectedItem()].m_size * TILE_SIZE;
					glColor4f(1, 1, 1, 0.2f);
					glTranslatef(-GLfloat(floor(GLfloat(_size.x / TILE_SIZE) / 2)) * TILE_SIZE, -GLfloat(floor(GLfloat(_size.y / TILE_SIZE) / 2)) * TILE_SIZE, 0);
					glTranslatef(-GLfloat(fmod((m_mouseBuffer.x + m_camPos.x), TILE_SIZE) - ((m_mouseBuffer.x + m_camPos.x) < 0 ? TILE_SIZE : 0) - m_mouseBuffer.x), -GLfloat(fmod((m_mouseBuffer.y + m_camPos.y), TILE_SIZE) - ((m_mouseBuffer.y + m_camPos.y) < 0 ? TILE_SIZE : 0) - m_mouseBuffer.y), 0);
					glBegin(GL_QUADS);
					{
						glVertex2f(0, 0);
						glVertex2f(GLfloat(_size.x), 0);
						glVertex2f(GLfloat(_size.x), GLfloat(_size.y));
						glVertex2f(0, GLfloat(_size.y));
					}
					glEnd();
				}
			}
			glPopMatrix();
		}

		glColor4f(1, 1, 1, 0.2f);
		if(m_mouseInArea && (m_listStamps->getSelectedItem() == 0 || m_selectLayer->getSelectedButton() != 4))
		{
			glTranslatef(-GLfloat(fmod((m_mouseBuffer.x + m_camPos.x), TILE_SIZE) - ((m_mouseBuffer.x + m_camPos.x) < 0 ? TILE_SIZE : 0) - m_mouseBuffer.x), -GLfloat(fmod((m_mouseBuffer.y + m_camPos.y), TILE_SIZE) - ((m_mouseBuffer.y + m_camPos.y) < 0 ? TILE_SIZE : 0) - m_mouseBuffer.y), 0);
			glBegin(GL_QUADS);
			{
				glVertex2f(0, 0);
				glVertex2f(GLfloat(TILE_SIZE), 0);
				glVertex2f(GLfloat(TILE_SIZE), GLfloat(TILE_SIZE));
				glVertex2f(0, GLfloat(TILE_SIZE));
			}
			glEnd();
		}
	}
	glPopMatrix();

	m_guiAll->render();

	if(getPause() == "CONTAINER_ENTITY_BOARD")
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_SCISSOR_TEST);
		glScissor(GLint(m_boardArea.x + Globals::getInstance().m_screenSize.x / 2), GLint(-m_boardArea.h + Globals::getInstance().m_screenSize.y / 2), GLsizei(m_boardArea.w - m_boardArea.x), GLsizei(m_boardArea.h - m_boardArea.y));
		glPushMatrix();
		{
			glTranslatef(m_boardArea.x, m_boardArea.y, 0);
			glTranslatef(-GLfloat(m_boardScroll.x % TILE_SIZE), -GLfloat(m_boardScroll.y % TILE_SIZE), 0);
			glBegin(GL_QUADS);
			{
				for(Sint16 x = -1; x < ceil((m_boardArea.w - m_boardArea.x) / TILE_SIZE) + 1; x++)
				{
					for(Sint16 y = -1; y < ceil((m_boardArea.h - m_boardArea.y) / TILE_SIZE) + 1; y++)
					{
						if((x + ceil(m_boardScroll.x / TILE_SIZE)) > 0 && (x + ceil(m_boardScroll.x / TILE_SIZE)) < m_boardSize.x &&
							(y + ceil(m_boardScroll.y / TILE_SIZE)) > 0 && (y + ceil(m_boardScroll.y / TILE_SIZE)) < m_boardSize.y &&
							m_boardData[Sint32(x + ceil(m_boardScroll.x / TILE_SIZE))][Sint32(y + ceil(m_boardScroll.y / TILE_SIZE))] == 0)
							glColor4f(0.02f, 0.08f, 0.2f, 0.6f);
						else
							glColor4f(0.02f, 0.08f, 0.2f, 0.1f);
						glVertex2f(GLfloat(x * TILE_SIZE), GLfloat(y * TILE_SIZE));
						glVertex2f((x + 1.0f) * TILE_SIZE, GLfloat(y * TILE_SIZE));
						glVertex2f((x + 1.0f) * TILE_SIZE, (y + 1.0f) * TILE_SIZE);
						glVertex2f(GLfloat(x * TILE_SIZE), (y + 1.0f) * TILE_SIZE);
					}
				}
			}
			glEnd();
			if(m_showGrid)
			{
				glBegin(GL_LINES);
				{
					for(Sint16 x = -1; x < ceil((m_boardArea.w - m_boardArea.x) / TILE_SIZE) + 1; x++)
					{
						if((x + ceil(m_boardScroll.x / TILE_SIZE)) > 0 && (x + ceil(m_boardScroll.x / TILE_SIZE)) <= m_boardSize.x)
						{
							glVertex2f(GLfloat(x * TILE_SIZE), -m_boardScroll.y + (m_boardScroll.y % TILE_SIZE) + TILE_SIZE);
							glVertex2f(GLfloat(x * TILE_SIZE), -m_boardScroll.y + (m_boardScroll.y % TILE_SIZE) + m_boardSize.y * TILE_SIZE);
						}
					}
					for(Sint16 y = -1; y < ceil((m_boardArea.h - m_boardArea.y) / TILE_SIZE) + 1; y++)
					{
						if((y + ceil(m_boardScroll.y / TILE_SIZE)) > 0 && (y + ceil(m_boardScroll.y / TILE_SIZE)) <= m_boardSize.y)
						{
							glVertex2f(-m_boardScroll.x + (m_boardScroll.x % TILE_SIZE) + TILE_SIZE, GLfloat(y * TILE_SIZE));
							glVertex2f(-m_boardScroll.x + (m_boardScroll.x % TILE_SIZE) + m_boardSize.x * TILE_SIZE, GLfloat(y * TILE_SIZE));
						}
					}
				}
				glEnd();
			}
		}
		glPopMatrix();
		glDisable(GL_SCISSOR_TEST);
	}
}
