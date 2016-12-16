#include "engine\game\editor\Editor.h"

void Editor::init()
{
	m_zoom = 0;

	m_cPauseScreen.clear();

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
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_GROUND", "Ground", {5, 4}, {m_selectLayer->getSize().x / 2 - 40, 24}, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_WORLD", "World", {m_selectLayer->getSize().x / 2 + 35, 4}, {m_selectLayer->getSize().x / 2 - 40, 24}, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_ENTITY", "Entity", {5, 34}, {m_selectLayer->getSize().x / 2 - 40, 24}, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_SKY", "Sky", {m_selectLayer->getSize().x / 2 + 35, 34}, {m_selectLayer->getSize().x / 2 - 40, 24}, 1));
	m_selectLayer->addButton(new CButtonToggle("RBUTTON_STAMPS", "Stamps", {5, 64}, {m_selectLayer->getSize().x - 10, 24}, 1));
	m_guiLeftLayer->addComponent(m_selectLayer, PANEL_ALIGN_TOP);

	m_guiGround = new ContainerPanel("CONTAINER_LEFT_GROUND", "", {0, 0}, m_guiLeftDetail->getSize(), 0, true, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_guiWorld = new ContainerPanel("CONTAINER_LEFT_WORLD", "", {0, 0}, m_guiLeftDetail->getSize(), 0, false, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_guiEntity = new ContainerPanel("CONTAINER_LEFT_ENTITY", "", {0, 0}, m_guiLeftDetail->getSize(), 0, false, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_guiSky = new ContainerPanel("CONTAINER_LEFT_SKY", "", {0, 0}, m_guiLeftDetail->getSize(), 0, false, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_guiStamp = new ContainerPanel("CONTAINER_LEFT_STAMP", "", {0, 0}, m_guiLeftDetail->getSize(), 0, false, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);

	m_guiLeftDetail->addComponent(m_guiGround);
	m_guiLeftDetail->addComponent(m_guiWorld);
	m_guiLeftDetail->addComponent(m_guiEntity);
	m_guiLeftDetail->addComponent(m_guiSky);
	m_guiLeftDetail->addComponent(m_guiStamp);



	m_guiWorld->addComponent(new TextField("TEXTFIELD_OBJECT_NAME", "Object Name", Vector2<Sint32>(0, 204), Vector2<Sint32>(256, 1), 1), PANEL_ALIGN_TOP);

	m_guiWorld->addComponent(new CDropDown("DROPDOWN_INTERACT", "Interaction Type", {0, 250}, {264, 32}, 1), PANEL_ALIGN_TOP);
	m_guiWorld->findComponent("DROPDOWN_INTERACT")->setPriorityLayer(5);

	m_guiWorld->addComponent(new CButton("BUTTON_DELETE", "", LTexture::getInstance().getImage("gui\\Trash.png"), {6, 6}, {24, 24}, 1), PANEL_ALIGN_TOP_RIGHT)->setTooltip("Deletes Object\n*Can't be undone!*");



	m_guiEntity->addComponent(new TextField("TEXTFIELD_ENTITY_NAME", "Entity Name", Vector2<Sint32>(36, 30), Vector2<Sint32>(256, 1), 1), PANEL_ALIGN_TOP_RIGHT);
	m_guiEntity->addComponent(new CDropDown("DROPDOWN_TYPE", "Entity Type", {0, 430}, {264, 32}, 1), PANEL_ALIGN_TOP)->setPriorityLayer(5);
	m_guiEntity->addComponent(new CButton("BUTTON_DELETE", "", LTexture::getInstance().getImage("gui\\Trash.png"), {3, 30}, {24, 24}, 1), PANEL_ALIGN_TOP_RIGHT)->setTooltip("Deletes Entity\n*Can't be undone!*");

	m_guiStamp->addComponent(new TextField("TEXTFIELD_STAMP_NAME", "Stamp Name", Vector2<Sint32>(0, 20), Vector2<Sint32>(256, 1), 1), PANEL_ALIGN_TOP);

	m_guiStamp->addComponent(new CButtonToggle("BUTTON_USE_GROUND", "Use Ground", Vector2<Sint32>(0, 360), Vector2<Sint32>(264, 32), 1), PANEL_ALIGN_TOP)->setState(1);
	m_guiStamp->addComponent(new CButtonToggle("BUTTON_USE_WORLD", "Use World", Vector2<Sint32>(0, 400), Vector2<Sint32>(264, 32), 1), PANEL_ALIGN_TOP)->setState(1);
	m_guiStamp->addComponent(new CButtonToggle("BUTTON_USE_ENTITY", "Use Entity", Vector2<Sint32>(0, 440), Vector2<Sint32>(264, 32), 1), PANEL_ALIGN_TOP)->setState(1);
	m_guiStamp->addComponent(new CButtonToggle("BUTTON_USE_SKY", "Use Sky", Vector2<Sint32>(0, 480), Vector2<Sint32>(264, 32), 1), PANEL_ALIGN_TOP)->setState(1);

	m_guiStamp->addComponent(new CButton("BUTTON_DELETE", "", LTexture::getInstance().getImage("gui\\Trash.png"), {3, 646}, {24, 24}, 1), PANEL_ALIGN_TOP_RIGHT);

	m_toolbarMenu = new CToolbar("TOOLBAR_MENU", {0, 0}, {Globals::getInstance().m_screenSize.x, 24}, 0);

	m_tabEntity = new CTabBar("TABBAR_ENTITIES", Vector2<Sint32>(0, 0), Vector2<Sint32>(m_guiEntity->getSize().x, 20), 1);
	m_guiEntity->addComponent(m_tabEntity, PANEL_ALIGN_TOP);

	m_listStamps = new CList("LIST_STAMPS", "Stamp List", {0, 208}, {256, 4}, 32, Texture(), 1);
	m_listStamps->addItem("None", 0);
	m_stamps.push_back(Stamp());
	m_guiStamp->addComponent(m_listStamps, PANEL_ALIGN_TOP);

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
	return "";
}

void Editor::unpause()
{
	if(!m_cPauseScreen.empty())
	{
		m_pauseScreens[m_cPauseScreen[m_cPauseScreen.size() - 1]]->setVisible(false);
		m_cPauseScreen.pop_back();
		if(!m_cPauseScreen.empty())
			m_pauseScreens[m_cPauseScreen[m_cPauseScreen.size() - 1]]->setVisible(true);
		else
			m_guiPause->setVisible(false);
	}
}

void Editor::pause(std::string p_screen)
{
	Uint16 i = 0;
	for(i = 0; i < m_pauseScreens.size(); i++)
		if(m_pauseScreens[i]->getName() == p_screen)
		{
			m_guiPause->setVisible(true);
			if(!m_cPauseScreen.empty())
				m_pauseScreens[m_cPauseScreen[m_cPauseScreen.size() - 1]]->setVisible(false);
			m_cPauseScreen.push_back(i);
			m_pauseScreens[i]->setVisible(true);
			return;
		}
	unpause();
}

std::string Editor::getPause()
{
	if(m_cPauseScreen.empty())
		return "";
	return m_pauseScreens[m_cPauseScreen[m_cPauseScreen.size() - 1]]->getName();
}

void Editor::input()
{

}

void Editor::update()
{

}

void Editor::render()
{
	glPushMatrix();
	{
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

		glBindTexture(GL_TEXTURE_2D, 0);
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

	std::string _tooltip = Globals::getInstance().m_tooltip;

	if(_tooltip != "")
	{
		Vector2<Sint32> _size = Vector2<Sint32>(Font::getInstance().getMessageWidth(_tooltip).x + 12, Font::getInstance().getSpacingHeight() * Font::getInstance().getMessageWidth(_tooltip).y + 6);

		glPushMatrix();
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glTranslatef(m_mouseBuffer.x, m_mouseBuffer.y, 0);
			glTranslatef((m_mouseBuffer.x - _size.x > -(Globals::getInstance().m_screenSize.x / 2) ? -_size.x : 0), (m_mouseBuffer.y - _size.y < -(Globals::getInstance().m_screenSize.y / 2) ? 0 : -_size.y), 0);
			glBegin(GL_QUADS);
			{
				glColor4f(0, 0, 0, 0.5f);
				glVertex2f(-1, -1);
				glVertex2f(_size.x + 1, -1);
				glVertex2f(_size.x + 1, _size.y + 1);
				glVertex2f(-1, _size.y + 1);

				glColor4f(1, 1, 1, 0.05f);
				glVertex2f(0, 0);
				glVertex2f(_size.x, 0);
				glVertex2f(_size.x, _size.y);
				glVertex2f(0, _size.y);
			}
			glEnd();
			glColor3f(1, 1, 1);
			Font::getInstance().setAlignment(ALIGN_CENTER);
			Font::getInstance().print(_tooltip, _size.x / 2, Font::getInstance().getHeight() + 2);
		}
		glPopMatrix();
		Globals::getInstance().m_tooltip = "";
	}
}

void Editor::autosave()
{

}

void Editor::refresh()
{

}
