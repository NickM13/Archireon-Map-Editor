#pragma once

#define PANEL_ALIGN_NONE				0
#define PANEL_ALIGN_CENTER				1
#define PANEL_ALIGN_LEFT				2
#define PANEL_ALIGN_RIGHT				3
#define PANEL_ALIGN_TOP					4
#define PANEL_ALIGN_BOTTOM				5
#define PANEL_ALIGN_TOP_LEFT			6
#define PANEL_ALIGN_TOP_RIGHT			7
#define PANEL_ALIGN_BOTTOM_LEFT			8
#define PANEL_ALIGN_BOTTOM_RIGHT		9

#include "..\base\Component.h"
#include "..\..\font\Font.h"

class Panel : public Component
{
private:
	struct PanelComponent
	{
		Component* m_component;
		Sint8 m_alignment;
	};
	std::vector< PanelComponent > m_componentList;
public:
	Panel() {};
	Panel(std::string p_compName, std::string p_title, Vector2< Sint32 > p_pos, Vector2< Sint32 > p_size, Sint8 p_colorTheme, bool p_visible, Sint32 p_texture = -1, Sint8 p_textureStyle = 0)
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
	}

	Sint8 input(Sint8* p_keyStates, Sint8* p_mouseStates, Vector2< Sint32 > p_mousePos)
	{
		p_mousePos = p_mousePos - m_pos;
		return Sint8((p_mousePos.x >= 0 && p_mousePos.x <= m_size.x && p_mousePos.y >= 0 && p_mousePos.y <= m_size.y) ? 1 : 0);
	}
	void update(GLfloat p_updateTime)
	{

	}
	void render()
	{
		if(m_visible)
		{
			if(m_textureStyle != COMPONENT_TEXTURE_STYLE_EMPTY)
			{
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
					Font::getInstance().setAlignment(ALIGN_CENTER);
					Font::getInstance().print(m_title, m_pos.x + m_size.x / 2, m_pos.y + 4);
				}
			}
		}
	}
};
