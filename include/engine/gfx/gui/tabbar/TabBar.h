#pragma once

#include "engine\utils\Utilities.h"
#include "..\base\Component.h"
#include "..\..\font\Font.h"
#include <vector>

struct TabBar : public Component
{
private:
	Vector2<Sint32> m_pos;
	Uint16 m_width;

	Uint16 m_numOfTabs;
	Uint16 m_selected;

	Sint32 m_scroll;
public:
	TabBar() {}

	void setPosition(Vector2<Sint32> p_pos);
	void setWidth(Sint32 p_width);

	void setTabCount(Uint16 p_num);
	Uint16 getTabCount();
	void setSelected(Uint16 p_selected);
	Uint16 getSelected();

	void checkPoint(Vector2<Sint32> p_mousePos);

	void render();
};
