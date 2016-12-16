#pragma once

#include "engine\gfx\LTexture.h"
#include "engine\utils\variable\datatype\Macros.h"

#include <vector>

class Items : public Singleton<Items>
{
public:
	struct Item
	{
		Uint16 id;
		std::string m_name;
		Texture m_texture;

		Item(Uint16 p_id = 0, std::string p_name = "", Texture p_texture = Texture()) : id(p_id), m_name(p_name), m_texture(p_texture) {}
	};

	void init();

	Uint16 addItem(Item p_item);
	Item getItem(Uint16 p_id);
	std::vector<Item> getItems() {return m_items;};
	void save();
	void load();
private:
	std::vector<Item> m_items;
};
