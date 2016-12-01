#pragma once

#include "Container.h"

class ContainerPanel : public Container
{
private:
	Sint8 m_lHeld, m_rHeld;
	Vector2<Sint32> m_mousePos;
	Vector2<Sint32> m_scroll;
	Vector2<Sint32> m_minScroll;
	Vector2<Sint32> m_maxScroll;
	bool m_draggable;
public:
	ContainerPanel() {};
	ContainerPanel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_colorTheme, bool p_visible, Sint32 p_texture = -1, Sint8 p_textureStyle = 0)
	{
		m_compName = p_compName;
		m_title = p_title;
		m_pos = p_pos;
		m_size = p_size;
		m_colorTheme = m_colorThemes[p_colorTheme];
		m_texture = p_texture;
		m_textureStyle = p_textureStyle;
		m_visible = p_visible;

		m_borderThickness = 1;

		m_scroll = {0, 0};
		m_minScroll = m_maxScroll = {0, 0};

		m_draggable = false; //currently disabled
	}

	Component* addComponent(Component* p_component, Sint8 p_alignment = PANEL_ALIGN_NONE)
	{
		Component* _comp = Container::addComponent(p_component, p_alignment);

		m_minScroll.x = min(m_minScroll.x, m_contentArea.x);
		m_minScroll.y = min(m_minScroll.y, m_contentArea.y - (m_title != "" ? 20 : 0));

		m_maxScroll.x = max(m_maxScroll.x, m_contentArea.z - m_size.x);
		m_maxScroll.y = max(m_maxScroll.y, m_contentArea.w - m_size.y + 2);

		m_scroll = m_minScroll;

		return _comp;
	}

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
	{
		Container::input(p_interactFlags, p_keyStates, p_mouseStates, p_mousePos + m_scroll);

		m_moveToFront = (m_lHeld != 0);

		// Scroll window
		if((p_interactFlags & 1) == 0)
		{
			if(m_rHeld != 0)
			{
				if(m_minScroll.x != 0 || m_maxScroll.x != 0)
					m_scroll.x -= p_mousePos.x - m_mousePos.x;
				if(m_minScroll.y != 0 || m_maxScroll.y != 0)
					m_scroll.y -= p_mousePos.y - m_mousePos.y;
				
				if(m_scroll.x < m_minScroll.x)
					m_scroll.x = m_minScroll.x;
				if(m_scroll.y < m_minScroll.y)
					m_scroll.y = m_minScroll.y;
				
				if(m_scroll.x > m_maxScroll.x)
					m_scroll.x = m_maxScroll.x;
				if(m_scroll.y > m_maxScroll.y)
					m_scroll.y = m_maxScroll.y;
			}
		}

		// Drag window -- disabled
		if(((p_interactFlags & 1) == 0 || m_lHeld) && m_draggable)
		{
			if(m_lHeld != 0)
				m_pos = m_pos + (p_mousePos - m_mousePos);
			if(p_mousePos.x - m_pos.x >= 0 && p_mousePos.x - m_pos.x <= m_size.x && 
				p_mousePos.y - m_pos.y >= 0 && p_mousePos.y - m_pos.y <= m_size.y)
			{
				if(p_mouseStates[0] == 1)
					m_lHeld = 2;
				else if(p_mouseStates[0] == 0 || p_mouseStates[0] == 3)
				{
					m_lHeld = 0;
					if(m_pos.x < 0)
						m_pos.x = 0;
					else if(m_pos.x > Sint32(Globals::getInstance().m_screenSize.x - m_size.x))
						m_pos.x = Sint32(Globals::getInstance().m_screenSize.x - m_size.x);
					if(m_pos.y > 0)
						m_pos.y = 0;
					else if(m_pos.y < -Sint32(Globals::getInstance().m_screenSize.y - m_size.y) + 24)
						m_pos.y = -Sint32(Globals::getInstance().m_screenSize.y - m_size.y) + 24;
				}

				if(p_mouseStates[0] != 0 && (p_interactFlags & 1) == 0)
					p_interactFlags += 1;
			}
			else
			{
				if(p_mouseStates[0] == 0 || p_mouseStates[0] == 3)
				{
					m_lHeld = 0;
					if(m_pos.x < 0)
						m_pos.x = 0;
					else if(m_pos.x > Sint32(Globals::getInstance().m_screenSize.x - m_size.x))
						m_pos.x = Sint32(Globals::getInstance().m_screenSize.x - m_size.x);
					if(m_pos.y > 0)
						m_pos.y = 0;
					else if(m_pos.y < -Sint32(Globals::getInstance().m_screenSize.x - m_size.x) + 24)
						m_pos.y = -Sint32(Globals::getInstance().m_screenSize.x - m_size.x) + 24;
				}
			}
		}
		if(p_mousePos.x - m_pos.x >= 0 && p_mousePos.x - m_pos.x <= m_size.x && 
			p_mousePos.y - m_pos.y >= 0 && p_mousePos.y - m_pos.y <= m_size.y)
		{
			if(p_mouseStates[1] == 1)
				m_rHeld = 2;
			else if(p_mouseStates[1] == 0 || p_mouseStates[1] == 3)
				m_rHeld = 0;

			if((p_interactFlags & 1) == 0)
				p_interactFlags += 1;
		}
		else
		{
			if(p_mouseStates[1] == 0 || p_mouseStates[1] == 3)
				m_rHeld = 0;
			else
				if(m_rHeld && (p_interactFlags & 1) == 0)
					p_interactFlags += 1;
		}
		m_mousePos = p_mousePos;
	}
	void update(GLfloat p_updateTime)
	{
		Container::update(p_updateTime);
	}
	void render()
	{
		if(m_visible)
		{
			Math::pushScissor(Rect(GLfloat(m_pos.x), GLfloat(m_pos.y), GLfloat(m_size.x), GLfloat(m_size.y)));
			if(m_texture != 0)
				Component::renderFill();
			else
				Component::render();
			if(m_title != "")
			{
				m_colorTheme.m_back.useColor();
				glBegin(GL_LINES);
				{
					glVertex2f(GLfloat(m_pos.x), GLfloat(m_pos.y + 24));
					glVertex2f(GLfloat(m_pos.x + m_size.x), GLfloat(m_pos.y + 24));
				}
				glEnd();

				m_colorTheme.m_text.useColor();
				Font::getInstance().setFontSize(16);
				Font::getInstance().setAlignment(ALIGN_CENTER);
				Font::getInstance().print(m_title, m_pos.x + m_size.x / 2, m_pos.y + 4);
			}
			Math::popScissor();
			Math::pushScissor(Rect(GLfloat(m_pos.x + 1), GLfloat(m_pos.y + (m_title != "" ? 20 : 0)), GLfloat(m_size.x - 2), GLfloat(m_size.y - 1 - (m_title != "" ? 20 : 0))));
			glPushMatrix();
			{
				glTranslatef(-GLfloat(m_scroll.x), -GLfloat(m_scroll.y), 0);
				Container::render();
			}
			glPopMatrix();
			Math::popScissor();
		}
	}
};
