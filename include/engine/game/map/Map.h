#pragma once

#pragma warning( disable : 6031 )

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Rectangle.h"
#include "engine\utils\variable\datatype\Vector2.h"

#include "engine\gfx\LTexture.h"

#include <string>

class Map
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
	struct Entity
	{
		std::string m_name;
		Vector2<Uint16> m_pos;
		Texture m_entityTex;
		Uint16 m_entityTexId;
		Uint8 m_entityType; // 0=Neutral, 1=Opponent, 2=Trader
		std::string m_interact, m_idle;

		Entity(std::string p_name = "", Texture p_entityTex = Texture())
		{
			m_name = p_name;
			m_entityTex = p_entityTex;
			m_entityTexId = 0;
			m_interact = m_idle = "";
			m_pos = {};
			m_entityType = 0;
		}
	};
public:
	Map() {};
	Map(Vector2<Uint16> p_mapSize) {};
	~Map() {};

	bool init(); //
	void resize(Vector2<Uint16> p_zoneSize); //

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
	void setEntity(Uint16 p_index, Vector2<Sint32> p_pos);

	void startEdit();
	void stopEdit();
	void undo();
	void redo();

	void render(Vector2<GLfloat> p_camPos, GLfloat p_zoom);

	virtual void save();
	virtual void save(std::string p_zoneName);

	virtual bool load();
	virtual bool load(std::string p_zoneName);

	virtual void createNew();
	virtual void clear();
protected:
	std::string m_mapName;
	Vector2<Uint16> m_mapSize;
	Rect m_viewSize;
	Sint16 m_tileSize;
	bool m_grid;
	std::vector<WorldObject> m_worldObjects;
	std::vector<Entity> m_entities;

	bool m_layerVisible[4];
	Uint16** m_tileData[3]; // Is called tile data but also accounts for entity id

	Texture m_tileSheetGround, m_tileSheetWorld, m_tileSheetSky;

	bool m_initialized;


	struct Edit
	{
		struct Tile
		{
			Tile() {}
			Tile(Uint16 p_layer, Uint16 p_x, Uint16 p_y, Uint16 p_id) : layer(p_layer), x(p_x), y(p_y), id(p_id)
			{}
			Sint32 layer, x, y, id;
		};
		struct Entity
		{
			Entity() {}
			Entity(Uint16 p_id, Vector2<Sint32> p_pos) : id(p_id), pos(p_pos)
			{}
			Uint16 id = 0;
			Vector2<Sint32> pos;
		};
		std::vector<Tile> m_tile;
		Entity m_entity;
	};
	Edit m_currentUndoEdit;
	Edit m_currentRedoEdit;
	std::vector<Edit> m_undoEdits; // For undoing
	std::vector<Edit> m_redoEdits; // For redoing
	Sint32 m_cEdit; // Current edit counter, for redoing
	bool m_editting;
};
