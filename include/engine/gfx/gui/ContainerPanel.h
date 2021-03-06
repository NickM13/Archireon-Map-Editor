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
	bool m_scrollX, m_scrollY;
public:
	ContainerPanel() {};
	ContainerPanel(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_colorTheme, bool p_visible, Sint32 p_texture = -1, Sint8 p_textureStyle = 0);

	Component* addComponent(Component* p_component, Sint8 p_alignment = PANEL_ALIGN_NONE);

	void calcSize(Vector2<Sint32> p_offset = {});
	//Set whether scroll bar across bottom exists
	void setScrollX(bool p_state);
	//Set whether scroll bar across right exists
	void setScrollY(bool p_state);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_updateTime);
	void render();
};
