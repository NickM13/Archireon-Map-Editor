#pragma once

#pragma warning( disable : 6031 )

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Rectangle.h"
#include "engine\utils\variable\datatype\Vector2.h"

#include "engine\gfx\LTexture.h"

#include <string>

class ZoneMap
{
public:
	struct WorldObject
	{
		std::string m_name;
		Uint16 m_interactionType;
		Uint16 m_tileTex;
		Uint16 m_frequency;
		std::string m_portalDest;
		Uint16 m_destX, m_destY;

		WorldObject(std::string p_name, Uint16 p_interactionType, Uint16 p_tileTex, Uint16 p_frequency = 0, std::string p_portalDest = "", Uint16 p_destX = 0, Uint16 p_destY = 0)
		{
			m_name = p_name;
			m_interactionType = p_interactionType;
			m_tileTex = p_tileTex;
			m_frequency = p_frequency;
			m_portalDest = p_portalDest;
			m_destX = p_destX;
			m_destY = p_destY;
		}
	};

	struct Board
	{
		Vector2<Uint16> m_size;
		Uint8** m_boardData;
		Texture m_background;

		Board(Vector2<Uint16> p_size, Texture p_background)
		{
			m_size = p_size;
			m_background = p_background;
			m_boardData = new Uint8*[m_size.x];
			for(Uint16 x = 0; x < m_size.x; x++)
			{
				m_boardData[x] = new Uint8[m_size.y];
				for(Uint16 y = 0; y < m_size.y; y++)
				{
					m_boardData[x][y] = 0;
				}
			}
		}
	};

	struct Entity
	{
		std::string m_name;
		Texture m_entityTex;
		Uint16 m_entityTexId;
		std::string m_script;

		Board* m_board;

		Entity(std::string p_name = "", Texture p_entityTex = Texture())
		{
			m_name = p_name;
			m_entityTex = p_entityTex;
			m_entityTexId = 0;
			m_script = "";

			m_board = 0;
		}

		void setBoard(Vector2<Uint16> p_size, Texture p_background)
		{
			m_board = new Board(p_size, p_background);
		}
	};

private:
	std::string m_zoneName;
	Vector2<Uint16> m_zoneSize;
	Rect m_viewSize;
	Sint16 m_tileSize;
	bool m_grid;
	std::vector<WorldObject> m_worldObjects;
	std::vector<Entity> m_entities;

	bool m_layerVisible[4];
	Uint16** m_tileData[4]; // Is called tile data but also accounts for entity id

	Texture m_tileSheetGround, m_tileSheetWorld, m_tileSheetSky;

	bool m_initialized;

public:
	ZoneMap();
	ZoneMap(Vector2<Uint16> p_zoneSize);
	~ZoneMap();

	bool init();
	void resize(Vector2<Uint16> p_zoneSize);

	void setTextureGround(Texture p_texture);
	Texture getTextureGround();
	void setTextureWorld(Texture p_texture);
	Texture getTextureWorld();
	void setTextureSky(Texture p_texture);
	Texture getTextureSky();

	void setViewSize(Rect p_viewSize);
	void setTileSize(Sint16 p_tileSize);
	void setLayerVisible(Sint16 p_layer, bool p_visible);
	void setGridVisible(bool p_visible);

	void setName(std::string p_zoneName);
	std::string getName();
	Vector2<Uint16> getSize();

	void setTile(Sint8 p_layer, Sint32 x, Sint32 y, Uint16 p_tile);
	Uint16 getTile(Sint8 p_layer, Sint32 x, Sint32 y);

	Uint16 addWorldObject(WorldObject p_object);
	WorldObject& getWorldObject(Uint16 p_index);
	Uint16 getWorldObjectSize();
	void removeWorldObject(Uint16 p_index);

	Uint16 addEntity(Entity p_entity);
	Entity& getEntity(Uint16 p_index);
	Uint16 getEntitySize();
	void removeEntity(Uint16 p_index);

	void render(Vector2<Sint32> p_camPos);

	void save();
	void save(std::string p_zoneName);

	bool load();
	bool load(std::string p_zoneName);

	void createNew();
	
	void clear();
};
