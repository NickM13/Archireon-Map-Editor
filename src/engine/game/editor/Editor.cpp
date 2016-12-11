#include "engine\game\editor\Editor.h"

void Editor::init()
{
	m_zoom = 0;

	m_cPauseScreen = -1;

	m_camPos = Vector2<GLfloat>(GLfloat(m_map->getSize().x / 2), GLfloat(m_map->getSize().y / 2));

	m_backTexId = LTexture::getInstance().getImage("Background.png");

	resize();

	m_guiAll = new Container("CONTAINER_ALL", Globals::getInstance().m_screenSize / -2, Globals::getInstance().m_screenSize, true);

	m_guiPause = new Container("CONTAINER_PAUSE", {0, 0}, Globals::getInstance().m_screenSize, false);
	m_guiTop = new Container("CONTAINER_TOP", {0, 0}, {Globals::getInstance().m_screenSize.x, 32}, true);
	m_guiLeft = new Container("CONTAINER_LEFT", {-1, 24}, {300, Globals::getInstance().m_screenSize.y - 23}, true);

	m_guiLeftLayer = new Container("CONTAINER_LEFT_LAYER", {0, 0}, {m_guiLeft->getSize().x, 94}, true);
	m_guiLeftDetail = new Container("CONTAINER_LEFT_DETAIL", {0, m_guiLeftLayer->getSize().y}, {m_guiLeft->getSize().x, m_guiLeft->getSize().y - m_guiLeftLayer->getSize().y}, true);

	m_guiAll->addComponent(m_guiPause, PANEL_ALIGN_TOP_LEFT)->setPriorityLayer(5);
	m_guiPause->addComponent(new Panel("PANEL_BG", "", {0, 0}, Globals::getInstance().m_screenSize, 4, true), PANEL_ALIGN_CENTER);
	m_guiAll->addComponent(m_guiLeft, PANEL_ALIGN_TOP_LEFT);
	m_guiLeft->addComponent(m_guiLeftLayer, PANEL_ALIGN_TOP_LEFT);
	m_guiLeft->addComponent(m_guiLeftDetail, PANEL_ALIGN_TOP_LEFT);



	m_guiAll->addComponent(m_guiTop, PANEL_ALIGN_TOP_LEFT)->setPriorityLayer(4);

	m_guiLeftLayer->addComponent(new Panel("", "", {}, m_guiLeftLayer->getSize(), 0, true, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE));


	m_selectLayer = new CButtonRadio("RADIO_LAYER", "Layer", {-1, 0}, {m_guiLeftLayer->getSize().x, 128}, 1);
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_GROUND", "Ground", {5, 4}, {m_selectLayer->getSize().x / 2 - 65, 24}, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_WORLD", "World", {m_selectLayer->getSize().x / 2 + 60, 4}, {m_selectLayer->getSize().x / 2 - 65, 24}, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_ENTITY", "Entity", {5, 34}, {m_selectLayer->getSize().x / 2 - 65, 24}, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_SKY", "Sky", {m_selectLayer->getSize().x / 2 + 60, 34}, {m_selectLayer->getSize().x / 2 - 65, 24}, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_STAMPS", "Stamps", {5, 64}, {m_selectLayer->getSize().x - 10, 24}, 1));
	m_guiLeftLayer->addComponent(m_selectLayer, PANEL_ALIGN_TOP);

	m_guiGround = new ContainerPanel("CONTAINER_LEFT_GROUND", "", {0, 0}, m_guiLeftDetail->getSize(), 0, true, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_guiWorld = new ContainerPanel("CONTAINER_LEFT_WORLD", "", {0, 0}, m_guiLeftDetail->getSize(), 0, false, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_guiEntity = new ContainerPanel("CONTAINER_LEFT_ENTITY", "", {0, 0}, m_guiLeftDetail->getSize(), 0, false, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_guiSky = new ContainerPanel("CONTAINER_LEFT_SKY", "", {0, 0}, m_guiLeftDetail->getSize(), 0, false, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_guiStamp = new ContainerPanel("CONTAINER_LEFT_STAMP", "", {0, 0}, m_guiLeftDetail->getSize(), 0, false, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);

	m_guiWorldSwitch = new Container("CONTAINER_RIGHT_WORLD_SWITCH", {0, 0}, {288, Globals::getInstance().m_screenSize.y}, false);
	m_guiWorldPortal = new Container("CONTAINER_RIGHT_WORLD_PORTAL", {0, 0}, {288, Globals::getInstance().m_screenSize.y}, false);

	//m_guiWorld->addComponent(m_guiWorldSwitch); Keeping these containers for reference, because some of their data is saved in the map file
	//m_guiWorld->addComponent(m_guiWorldPortal); Not sure if that statement is true anymore, I might have deleted stuff but too tired to check

	m_guiLeftDetail->addComponent(m_guiGround);
	m_guiLeftDetail->addComponent(m_guiWorld);
	m_guiLeftDetail->addComponent(m_guiEntity);
	m_guiLeftDetail->addComponent(m_guiSky);
	m_guiLeftDetail->addComponent(m_guiStamp);



	m_guiWorld->addComponent(new TextField("TEXTFIELD_OBJECT_NAME", "Object Name", Vector2<Sint32>(0, 170), Vector2<Sint32>(256, 1), 1), PANEL_ALIGN_TOP);

	m_guiWorldPortal->addComponent(new TextField("TEXTFIELD_PORTAL", "Destination ", {(m_guiWorldPortal->getSize().x - (256)) / 2, 416}, Vector2<Sint32>(256, 1), 1));
	m_guiWorldPortal->addComponent(new CCounter("COUNTER_PORTAL_X", "X:", {0, 440}, {0, 500}, 1, 0));
	m_guiWorldPortal->addComponent(new CCounter("COUNTER_PORTAL_Y", "Y:", {0, 460}, {0, 500}, 1, 0));

	m_guiWorld->addComponent(new CDropDown("DROPDOWN_INTERACT", "Interaction Type", {0, 296}, {264, 32}, 1), PANEL_ALIGN_TOP);
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("None");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Solid");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Switch");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Solid Switch");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->addItem("Portal");
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->setPriorityLayer(5);

	m_guiWorldSwitch->addComponent(new CCounter("COUNTER_SWITCH_FREQUENCY", "Freq.", {0, 440}, {0, 255}, 1, 0));

	m_guiWorld->addComponent(new CButton("BUTTON_DELETE", "", LTexture::getInstance().getImage("gui\\Trash.png"), {3, 646}, {24, 24}, 1), PANEL_ALIGN_TOP_RIGHT);



	m_guiEntity->addComponent(new TextField("TEXTFIELD_ENTITY_NAME", "Entity Name", Vector2<Sint32>(0, 30), Vector2<Sint32>(256, 1), 1), PANEL_ALIGN_TOP);

	m_guiEntity->addComponent(new CDropDown("DROPDOWN_TYPE", "Entity Type", {0, 430}, {264, 32}, 1), PANEL_ALIGN_TOP);
	m_guiEntity->findComponent("DROPDOWN_TYPE")->addItem("Neutral");
	m_guiEntity->findComponent("DROPDOWN_TYPE")->addItem("Opponent");
	m_guiEntity->findComponent("DROPDOWN_TYPE")->addItem("Trader");
	m_guiEntity->findComponent("DROPDOWN_TYPE")->setPriorityLayer(5);

	m_guiEntity->addComponent(new CButton("BUTTON_DELETE", "", LTexture::getInstance().getImage("gui\\Trash.png"), {3, 574}, {24, 24}, 1), PANEL_ALIGN_TOP_RIGHT);

	m_guiStamp->addComponent(new TextField("TEXTFIELD_STAMP_NAME", "Stamp Name", Vector2<Sint32>(0, 20), Vector2<Sint32>(256, 1), 1), PANEL_ALIGN_TOP);

	m_guiStamp->addComponent(new CButtonToggle("BUTTON_USE_GROUND", "Use Ground", Vector2<Sint32>(0, 360), Vector2<Sint32>(264, 32), 1), PANEL_ALIGN_TOP)->setState(1);
	m_guiStamp->addComponent(new CButtonToggle("BUTTON_USE_WORLD", "Use World", Vector2<Sint32>(0, 400), Vector2<Sint32>(264, 32), 1), PANEL_ALIGN_TOP)->setState(1);
	m_guiStamp->addComponent(new CButtonToggle("BUTTON_USE_ENTITY", "Use Entity", Vector2<Sint32>(0, 440), Vector2<Sint32>(264, 32), 1), PANEL_ALIGN_TOP)->setState(1);
	m_guiStamp->addComponent(new CButtonToggle("BUTTON_USE_SKY", "Use Sky", Vector2<Sint32>(0, 480), Vector2<Sint32>(264, 32), 1), PANEL_ALIGN_TOP)->setState(1);

	m_guiStamp->addComponent(new CButton("BUTTON_DELETE", "", LTexture::getInstance().getImage("gui\\Trash.png"), {3, 646}, {24, 24}, 1), PANEL_ALIGN_TOP_RIGHT);

	m_toolbarMenu = new CToolbar("TOOLBAR_MENU", {0, 0}, {Globals::getInstance().m_screenSize.x, 24}, 0);

	m_tabEntity = new CTabBar("TABBAR_ENTITIES", Vector2<Sint32>(0, 0), Vector2<Sint32>(m_guiEntity->getSize().x, 20), 1);
	m_tabEntity->addItem("None");
	m_map->addEntity(Map::Entity("None", Texture()));
	m_guiEntity->addComponent(m_tabEntity, PANEL_ALIGN_TOP);

	m_listStamps = new CList("LIST_STAMPS", "Stamp List", {0, 208}, {256, 4}, 32, Texture(), 1);
	m_listStamps->addItem({"None", 0});
	m_stamps.push_back(Stamp());
	m_guiStamp->addComponent(m_listStamps, PANEL_ALIGN_TOP);



	m_tileSetGround = new CTileSet("TILESET_GROUND", "Ground Tile Set", {0, 34}, {256, 256}, 32, m_map->getTextureGround(), 1);
	m_tileSetWorld = new CTileSet("TILESET_WORLD", "World Tile Set", {0, 360}, {256, 256}, 32, m_map->getTextureWorld(), 1);
	m_tileSetEntity = new CTileSet("TILESET_ENTITY", "Spritesheet", {0, 92}, {256, 256}, 32, Texture(), 1);
	m_tileSetSky = new CTileSet("TILESET_SKY", "Sky Tile Set", {0, 32}, {256, 256}, 32, m_map->getTextureSky(), 1);
	m_tileSetStamps = new CTileSet("TILESET_WORLD", "TODO: Preview", {0, 88}, {256, 256}, 32, Texture(), 1);

	m_guiGround->addComponent(m_tileSetGround, PANEL_ALIGN_TOP);
	m_guiWorld->addComponent(m_tileSetWorld, PANEL_ALIGN_TOP);
	m_guiEntity->addComponent(m_tileSetEntity, PANEL_ALIGN_TOP);
	m_guiSky->addComponent(m_tileSetSky, PANEL_ALIGN_TOP);
	m_guiStamp->addComponent(m_tileSetStamps, PANEL_ALIGN_TOP);



	m_listWorld = new CList("LIST_WORLD", "World Obj List", {0, 32}, {256, 4}, 32, m_map->getTextureWorld(), 1);
	m_listWorld->addItem({"None", 0});
	m_map->addWorldObject(Map::WorldObject("None", 0, 0));
	m_guiWorld->addComponent(m_listWorld, PANEL_ALIGN_TOP);

	m_guiLeft->addComponent(new CText("TEXT_CURSOR_POS", "LEL", {m_guiLeft->getSize().x + 2, 10}, {100, 100}, ALIGN_LEFT, Color(255, 255, 255, 255)))->setPriorityLayer(-1);

	m_selectStart = {-1, -1};
}

void Editor::resize()
{
	m_tileMapArea = Rect(-GLfloat(Globals::getInstance().m_screenSize.x / 2), -GLfloat(Globals::getInstance().m_screenSize.y / 2), GLfloat(Globals::getInstance().m_screenSize.x / 2), GLfloat(Globals::getInstance().m_screenSize.y / 2));
}

bool Editor::switchEditor()
{
	return (m_guiLeftLayer->findComponent("BUTTON_EDITOR_TYPE")->isSelected() == 1);
}

std::string Editor::getZoneName()
{
	return m_map->getName();
}

void Editor::unpause()
{
	if(m_cPauseScreen != -1)
	{
		m_guiPause->setVisible(false);
		m_pauseScreens[m_cPauseScreen]->setVisible(false);
		m_cPauseScreen = -1;
	}
}

void Editor::pause(std::string p_screen)
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

std::string Editor::getPause()
{
	if(m_cPauseScreen == -1)
		return "";
	return m_pauseScreens[m_cPauseScreen]->getName();
}

void Editor::input()
{
	Vector2<Sint32> _mousePos = Globals::getInstance().m_mousePos;
	Sint8 _rValue = 0; // input flags: 1 = mouse click, 2 = key press, 4 = mouse scroll
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
		if(m_cPauseScreen != -1)
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
	{
		m_pauseScreens[m_cPauseScreen]->findComponent("BUTTON_SET")->setState(1);
	}

	if(Globals::getInstance().m_mouseStates[0] == 1)
	{
		if(m_mouseInArea)
		{
			m_lmbDown = true;
			if(m_selectLayer->getSelectedButton() == 4)
				m_selectStart = {Sint32(floor((m_mouseBuffer.x + m_camPos.x) / TILE_SIZE) - 1), Sint32(floor((m_mouseBuffer.y + m_camPos.y) / TILE_SIZE) - 1)};
		}
	}
	if(Globals::getInstance().m_mouseStates[1] == 1)
	{
		if(m_tileMapArea.checkPoint(GLfloat(m_mouseBuffer.x), GLfloat(m_mouseBuffer.y)))
			m_rmbDown = true;
	}

	if(Globals::getInstance().m_mouseStates[0] == 0)
		m_lmbDown = false;
	if(Globals::getInstance().m_mouseStates[1] == 0)
		m_rmbDown = false;

	if((_rValue & 1) == 0 && m_lmbDown && !m_rmbDown && m_mouseInArea)
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
			m_map->getEntity(m_tabEntity->getSelectedItem()).m_pos = Vector2<Sint32>(Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom))));
			break;
		case 3:
			m_map->setTile(2, Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom))), m_tileSetSky->getSelectedTile());
			break;
		case 4:
			if(m_listStamps->getSelectedItem() == 0)
			{
				m_selectEnd = {Sint32(floor((m_mouseBuffer.x + _camPos.x) / (TILE_SIZE + m_zoom))), Sint32(floor((m_mouseBuffer.y + _camPos.y) / (TILE_SIZE + m_zoom)))};
			}
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
	else if(m_rmbDown && (_rValue & 1) == 0)
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

void Editor::update()
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
				m_map->addWorldObject({m_listWorld->getListItem(m_listWorld->getSelectedItem()).m_name, 1, m_listWorld->getListItem(m_listWorld->getSelectedItem()).m_texId});
			}
			if((m_listWorld->isUpdated() & 1) == 1) // Switch item
			{
				m_guiWorld->findComponent("TEXTFIELD_OBJECT_NAME")->setTitle(m_listWorld->getListItem(m_listWorld->getSelectedItem()).m_name);
				m_guiWorld->findComponent("DROPDOWN_INTERACT")->setSelectedItem(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_interactionType);
				m_tileSetWorld->setSelectedTile(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_tileTex);

				m_guiWorldPortal->findComponent("TEXTFIELD_PORTAL")->setTitle(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_portalDest);
				m_guiWorldPortal->findComponent("COUNTER_PORTAL_X")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destX);
				m_guiWorldPortal->findComponent("COUNTER_PORTAL_Y")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destY);
				m_guiWorldSwitch->findComponent("COUNTER_SWITCH_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
			}
		}
		else if(m_guiWorld->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_listWorld->getSelectedItem() != 0 && m_listWorld->getSelectedItem() < m_listWorld->getListSize())
		{
			m_listWorld->removeItem(m_listWorld->getSelectedItem());
			m_map->removeWorldObject(m_listWorld->getSelectedItem() + 1);

			m_guiWorld->findComponent("TEXTFIELD_OBJECT_NAME")->setTitle(m_listWorld->getListItem(m_listWorld->getSelectedItem()).m_name);
			m_guiWorld->findComponent("DROPDOWN_INTERACT")->setSelectedItem(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_interactionType);
			m_tileSetWorld->setSelectedTile(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_tileTex);

			m_guiWorldPortal->findComponent("TEXTFIELD_PORTAL")->setTitle(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_portalDest);
			m_guiWorldPortal->findComponent("COUNTER_PORTAL_X")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destX);
			m_guiWorldPortal->findComponent("COUNTER_PORTAL_Y")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destY);
			m_guiWorldSwitch->findComponent("COUNTER_SWITCH_FREQUENCY")->setValue(m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency);
		}
		if(m_listWorld->getSelectedItem() != 0)
		{
			if(m_guiWorld->findComponent("TEXTFIELD_OBJECT_NAME")->getTitle() != "")
				m_listWorld->getListItem(m_listWorld->getSelectedItem()).m_name = m_guiWorld->findComponent("TEXTFIELD_OBJECT_NAME")->getTitle();
			m_listWorld->getListItem(m_listWorld->getSelectedItem()).m_texId = m_tileSetWorld->getSelectedTile();
			m_map->getWorldObject(m_listWorld->getSelectedItem()).m_interactionType = m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem();
			m_map->getWorldObject(m_listWorld->getSelectedItem()).m_tileTex = m_tileSetWorld->getSelectedTile();
			if(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getItem(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem()) == "Portal")
			{
				m_map->getWorldObject(m_listWorld->getSelectedItem()).m_portalDest = m_guiWorldPortal->findComponent("TEXTFIELD_PORTAL")->getTitle();
				m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destX = m_guiWorldPortal->findComponent("COUNTER_PORTAL_X")->getValue();
				m_map->getWorldObject(m_listWorld->getSelectedItem()).m_destY = m_guiWorldPortal->findComponent("COUNTER_PORTAL_Y")->getValue();
			}
			else if(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getItem(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem()) == "Switch" || m_guiWorld->findComponent("DROPDOWN_INTERACT")->getItem(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem()) == "Solid Switch")
			{
				m_map->getWorldObject(m_listWorld->getSelectedItem()).m_frequency = m_guiWorldSwitch->findComponent("COUNTER_SWITCH_FREQUENCY")->getValue();
			}
		}
		if(m_guiWorld->findComponent("DROPDOWN_INTERACT")->isUpdated())
		{
			m_guiWorldPortal->setVisible(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getItem(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem()) == "Portal");
			m_guiWorldSwitch->setVisible(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getItem(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem()) == "Switch" ||
				m_guiWorld->findComponent("DROPDOWN_INTERACT")->getItem(m_guiWorld->findComponent("DROPDOWN_INTERACT")->getSelectedItem()) == "Solid Switch");
		}
		break;
	case 2: // ENTITIES
		if(m_tabEntity->isUpdated() != 0)
		{
			if((m_tabEntity->isUpdated() & 2) == 2) // New item
				m_map->addEntity(Map::Entity(m_tabEntity->getTab(m_tabEntity->getSelectedItem())));
			if((m_tabEntity->isUpdated() & 1) == 1) // Switch item
			{
				m_guiEntity->findComponent("TEXTFIELD_ENTITY_NAME")->setTitle(m_tabEntity->getTab(m_tabEntity->getSelectedItem()));
				if(m_tabEntity->getSelectedItem() != 0)
					m_tileSetEntity->setTileSheet(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityTex);
				else
					m_tileSetEntity->setTileSheet(Texture());
				m_tileSetEntity->setSelectedTile(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityTexId);
				m_guiEntity->findComponent("DROPDOWN_TYPE")->setSelectedItem(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityType);
			}
		}
		else if(m_guiEntity->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_tabEntity->getSelectedItem() != 0 && m_tabEntity->getSelectedItem() < m_tabEntity->getTabCount())
		{
			m_tabEntity->removeTab(m_tabEntity->getSelectedItem());
			m_map->removeEntity(m_tabEntity->getSelectedItem());

			m_guiEntity->findComponent("TEXTFIELD_ENTITY_NAME")->setTitle(m_tabEntity->getTab(m_tabEntity->getSelectedItem()));
			m_tileSetEntity->setTileSheet(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityTex);
			m_tileSetEntity->setSelectedTile(m_map->getEntity(m_tabEntity->getSelectedItem()).m_entityTexId);
		}
		if(m_tabEntity->getSelectedItem() > 0)
		{
			if(m_guiEntity->findComponent("TEXTFIELD_ENTITY_NAME")->getTitle() != "")
			{
				m_tabEntity->setTab(m_tabEntity->getSelectedItem(), m_guiEntity->findComponent("TEXTFIELD_ENTITY_NAME")->getTitle());
				m_map->getEntity(m_tabEntity->getSelectedItem()).m_name = m_tabEntity->getTab(m_tabEntity->getSelectedItem());
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
				m_guiStamp->findComponent("TEXTFIELD_STAMP_NAME")->setTitle(m_listStamps->getListItem(m_listStamps->getSelectedItem()).m_name);
				m_selectStart = {-1, -1};
			}
			m_guiStamp->setVisible(m_listStamps->getSelectedItem() != 0);
		}
		else if(m_guiStamp->findComponent("BUTTON_DELETE")->isSelected() == 3 && m_listStamps->getSelectedItem() != 0 && m_listStamps->getSelectedItem() < m_listStamps->getListSize())
		{
			m_stamps.erase(m_stamps.begin() + m_listStamps->getSelectedItem());
			m_listStamps->removeItem(m_listStamps->getSelectedItem());

			m_guiStamp->findComponent("TEXTFIELD_STAMP_NAME")->setTitle(m_listStamps->getListItem(m_listStamps->getSelectedItem()).m_name);
			m_selectStart = {-1, -1};

			m_guiStamp->setVisible(m_listStamps->getSelectedItem() != 0);
		}
		else if(m_listStamps->getSelectedItem() != 0)
		{
			if(m_guiStamp->findComponent("TEXTFIELD_STAMP_NAME")->getTitle() != "")
				m_listStamps->getListItem(m_listStamps->getSelectedItem()).m_name = m_guiStamp->findComponent("TEXTFIELD_STAMP_NAME")->getTitle();
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
			m_listStamps->addItem({std::string("Item ") + Util::numToString(m_listStamps->getListSize()), 0});
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

void Editor::render()
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

	glPushMatrix();
	{
		m_map->render(m_camPos, m_zoom);


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
			glTranslatef(GLfloat((floor((m_mouseBuffer.x) / (TILE_SIZE + m_zoom) + fmod(m_camPos.x, 1)) - fmod(m_camPos.x, 1)) * (TILE_SIZE + m_zoom)),
				GLfloat((floor((m_mouseBuffer.y) / (TILE_SIZE + m_zoom) + fmod(m_camPos.y, 1)) - fmod(m_camPos.y, 1)) * (TILE_SIZE + m_zoom)), 0);
			glBegin(GL_QUADS);
			{
				glVertex2f(0, 0);
				glVertex2f(GLfloat(TILE_SIZE + m_zoom), 0);
				glVertex2f(GLfloat(TILE_SIZE + m_zoom), GLfloat(TILE_SIZE + m_zoom));
				glVertex2f(0, GLfloat(TILE_SIZE + m_zoom));
			}
			glEnd();
		}
	}
	glPopMatrix();

	m_guiAll->render();
}
