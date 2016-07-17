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

#include "engine\utils\Globals.h"

class Container : public Component
{
private:
	struct PanelComponent
	{
		Component* m_component;
		Sint8 m_alignment;
	};
	std::vector< PanelComponent > m_componentList;
public:
	Container(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_visible)
	{
		m_compName = p_compName;
		m_pos = p_pos;
		m_size = p_size;
		m_visible = p_visible;
	}

	Component* addComponent(Component* p_component, Sint8 p_alignment = PANEL_ALIGN_NONE)
	{
		m_componentList.push_back({p_component, p_alignment});
		return p_component;
	}
	Component* findComponent(std::string p_compName)
	{
		for(Uint16 i = 0; i < m_componentList.size(); i++)
			if(m_componentList[i].m_component->getName() == p_compName)
				return m_componentList[i].m_component;
		return 0;
	}

	void setVisible(bool p_visible)
	{
		m_visible = p_visible;
		if(p_visible == false)
			for(Uint16 i = 0; i < m_componentList.size(); i++)
				m_componentList[i].m_component->update(0);
	}

	Sint8 input(Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
	{
		Sint8 _rValue = 0;
		Sint8 _interacted = 0;
		Vector2<Sint32> _mousePos;
		if(m_visible)
		{
			for(Sint32 i = m_componentList.size() - 1; i >= 0; i--)
			{
				if(!m_componentList[i].m_component->isVisible())
					continue;
				_mousePos = p_mousePos - m_pos;
				switch(m_componentList[i].m_alignment)
				{
				case PANEL_ALIGN_NONE:

					break;
				case PANEL_ALIGN_CENTER:
					_mousePos = _mousePos - Vector2<Sint32>(Sint32(m_size.x - m_componentList[i].m_component->getSize().x) / 2, Sint32(m_size.y - m_componentList[i].m_component->getSize().y) / 2);
					break;
				case PANEL_ALIGN_LEFT:
					_mousePos = _mousePos - Vector2<Sint32>(0, Sint32(m_size.y - m_componentList[i].m_component->getSize().y) / 2);
					break;
				case PANEL_ALIGN_RIGHT:
					_mousePos = _mousePos - Vector2<Sint32>(Sint32(m_size.x - m_componentList[i].m_component->getSize().x), Sint32(m_size.y - m_componentList[i].m_component->getSize().y) / 2);
					_mousePos = _mousePos - Vector2<Sint32>(Sint32(m_componentList[i].m_component->getPosition().x * -2), 0);
					break;
				case PANEL_ALIGN_TOP:
					_mousePos = _mousePos - Vector2<Sint32>(Sint32(m_size.x - m_componentList[i].m_component->getSize().x) / 2, 0);
					break;
				case PANEL_ALIGN_BOTTOM:
					_mousePos = _mousePos - Vector2<Sint32>(Sint32(m_size.x - m_componentList[i].m_component->getSize().x) / 2, Sint32(m_size.y - m_componentList[i].m_component->getSize().y));
					_mousePos = _mousePos - Vector2<Sint32>(0, Sint32(m_componentList[i].m_component->getPosition().y * -2));
					break;
				case PANEL_ALIGN_TOP_LEFT:

					break;
				case PANEL_ALIGN_TOP_RIGHT:
					_mousePos = _mousePos - Vector2<Sint32>(Sint32(m_size.x - m_componentList[i].m_component->getSize().x), 0);
					_mousePos = _mousePos - Vector2<Sint32>(Sint32(m_componentList[i].m_component->getPosition().x * -2), 0);
					break;
				case PANEL_ALIGN_BOTTOM_LEFT:
					_mousePos = _mousePos - Vector2<Sint32>(0, Sint32(m_size.y - m_componentList[i].m_component->getSize().y));
					_mousePos = _mousePos - Vector2<Sint32>(0, Sint32(m_componentList[i].m_component->getPosition().y * -2));
					break;
				case PANEL_ALIGN_BOTTOM_RIGHT:
					_mousePos = _mousePos - Vector2<Sint32>(Sint32(m_size.x - m_componentList[i].m_component->getSize().x), Sint32(m_size.y - m_componentList[i].m_component->getSize().y));
					_mousePos = _mousePos - Vector2<Sint32>(Sint32(m_componentList[i].m_component->getPosition().x * -2), Sint32(m_componentList[i].m_component->getPosition().y * -2));
					break;
				}
				if((_interacted & 1) == 1)
					_rValue = m_componentList[i].m_component->input(p_keyStates, p_mouseStates, {-9001, -9001});
				else
					_rValue = m_componentList[i].m_component->input(p_keyStates, p_mouseStates, _mousePos);
				if((_rValue & 1) == 1 && (_interacted & 1) != 1)
				{
					_interacted += 1;
				}
				if((_rValue & 2) == 2 && (_interacted & 2) != 2)
					_interacted += 2;
				if(_interacted == 3)
					return 3;
			}
		}
		return _interacted;
	}
	void update(GLfloat p_updateTime)
	{
		if(m_visible)
			for(Uint16 i = 0; i < m_componentList.size(); i++)
				if(m_componentList[i].m_component->isVisible())
					m_componentList[i].m_component->update(p_updateTime);
	}
	void render()
	{
		if(m_visible)
		{
			glPushMatrix();
			{
				glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
				for(Uint16 i = 0; i < m_componentList.size(); i++)
				{
					if(!m_componentList[i].m_component->isVisible())
						continue;
					glPushMatrix();
					{
						switch(m_componentList[i].m_alignment)
						{
						case PANEL_ALIGN_NONE:

							break;
						case PANEL_ALIGN_CENTER:
							glTranslatef(GLfloat(m_size.x - m_componentList[i].m_component->getSize().x) / 2, GLfloat(m_size.y - m_componentList[i].m_component->getSize().y) / 2, 0);
							break;
						case PANEL_ALIGN_LEFT:
							glTranslatef(0, GLfloat(m_size.y - m_componentList[i].m_component->getSize().y) / 2, 0);
							break;
						case PANEL_ALIGN_RIGHT:
							glTranslatef(GLfloat(m_size.x - m_componentList[i].m_component->getSize().x), GLfloat(m_size.y - m_componentList[i].m_component->getSize().y) / 2, 0);
							glTranslatef(GLfloat(m_componentList[i].m_component->getPosition().x * -2), 0, 0);
							break;
						case PANEL_ALIGN_TOP:
							glTranslatef(GLfloat(m_size.x - m_componentList[i].m_component->getSize().x) / 2, 0, 0);
							break;
						case PANEL_ALIGN_BOTTOM:
							glTranslatef(GLfloat(m_size.x - m_componentList[i].m_component->getSize().x) / 2, GLfloat(m_size.y - m_componentList[i].m_component->getSize().y), 0);
							glTranslatef(0, GLfloat(m_componentList[i].m_component->getPosition().y * -2), 0);
							break;
						case PANEL_ALIGN_TOP_LEFT:

							break;
						case PANEL_ALIGN_TOP_RIGHT:
							glTranslatef(GLfloat(m_size.x - m_componentList[i].m_component->getSize().x), 0, 0);
							glTranslatef(GLfloat(m_componentList[i].m_component->getPosition().x * -2), 0, 0);
							break;
						case PANEL_ALIGN_BOTTOM_LEFT:
							glTranslatef(0, GLfloat(m_size.y - m_componentList[i].m_component->getSize().y), 0);
							glTranslatef(0, GLfloat(m_componentList[i].m_component->getPosition().y * -2), 0);
							break;
						case PANEL_ALIGN_BOTTOM_RIGHT:
							glTranslatef(GLfloat(m_size.x - m_componentList[i].m_component->getSize().x), GLfloat(m_size.y - m_componentList[i].m_component->getSize().y), 0);
							glTranslatef(GLfloat(m_componentList[i].m_component->getPosition().x * -2), GLfloat(m_componentList[i].m_component->getPosition().y * -2), 0);
							break;
						}
						m_componentList[i].m_component->render();
					}
					glPopMatrix();
				}
			}
			glPopMatrix();
		}
	}
};
