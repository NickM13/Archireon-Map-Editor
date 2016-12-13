#pragma once

#include "engine\utils\Globals.h"
#include "engine\utils\Utilities.h"
#include "_Component.h"
#include "..\font\Font.h"

class CList : public Component
{
private:
	Sint16 m_selectedItem;
	Uint16 m_itemHeight;

	Sint16 m_scroll, m_maxScroll;

	Vector2<Uint16> m_mouseBuffer;

	bool m_hover, m_dragging;
	Uint8 m_update;

	Texture m_tileSheet;

	struct ListItem
	{
		std::string m_name;
		Uint16 m_texId;

		ListItem(std::string p_name, Uint16 p_texId)
		{
			m_name = p_name;
			m_texId = p_texId;
		}
	};
	std::vector<ListItem> m_itemList;
public:
	CList(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight, Texture p_tileSheet, Sint8 p_colorTheme = 0);

	void addItem(ListItem p_item);
	void removeItem(Uint16 p_index);
	ListItem& getListItem(Uint16 p_index);
	Uint16 getListSize();

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();

	Sint16 getSelectedItem();
	Uint8 isUpdated();

	Sint8 isSelected();

	void clear();
};
