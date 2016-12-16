#pragma once

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
		Uint8 m_frequency;
		std::string m_portalDest;
		Sint16 m_destX, m_destY;
		Sint8 m_slowCount;
		Sint8 m_direction; // Up=0, Right, Down, Left
		Uint8 m_lightValue;

		WorldObject(std::string p_name, Uint16 p_interactionType, Uint16 p_tileTex)
		{
			m_name = p_name;
			m_interactionType = p_interactionType;
			m_tileTex = p_tileTex;
			m_frequency = 0;
			m_portalDest = "";
			m_destX = 0;
			m_destY = 0;
			m_direction = 0;
			m_lightValue = 0;
		}
	};
	struct LightNode
	{
		Uint16 x, y;
		GLfloat brightness;
		LightNode(Uint16 p_x = 0, Uint16 p_y = 0, GLfloat p_brightness = 0) : x(p_x), y(p_y), brightness(p_brightness) {}
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

	void setTime(GLfloat p_time) {m_time = p_time;};
	void setName(std::string p_zoneName);
	std::string getName();
	Vector2<Uint16> getSize();

	void setTile(Sint8 p_layer, Sint32 x, Sint32 y, Uint16 p_tile);
	Uint16 getTile(Sint8 p_layer, Sint32 x, Sint32 y);

	Uint16 addWorldObject(WorldObject p_object);
	WorldObject& getWorldObject(Uint16 p_index);
	Uint16 getWorldObjectSize();
	void removeWorldObject(Uint16 p_index);

	void startEdit();
	void stopEdit();
	virtual void undo();
	virtual void redo();

	bool isFilling() {return m_filling;};
	void startFill(Sint8 p_layer, Uint16 p_id, Vector2<Sint32> p_pos);
	void moveFill(Vector2<Sint32> p_pos);
	void stopFill();

	void addLight(LightNode p_node);
	void updateLight();
	void removeLight(Vector2<Uint16> p_pos);

	virtual void render(Vector2<GLfloat> p_camPos, GLfloat p_zoom);
	void renderGround(Vector2<GLfloat> p_camPos, GLfloat p_tileSize);
	void renderWorld(Vector2<GLfloat> p_camPos, GLfloat p_zoom);
	virtual void renderEntity(Vector2<GLfloat> p_camPos, GLfloat p_zoom);
	void renderSky(Vector2<GLfloat> p_camPos, GLfloat p_zoom);
	virtual void renderLighting(Vector2<GLfloat> p_camPos, GLfloat p_zoom);

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

	bool m_layerVisible[4];
	Uint16** m_tileData[3];
	bool m_lightEnabled;
	GLfloat** m_lightData;
	GLfloat** m_lightDataCorners;
	std::vector<LightNode> m_lightNodes;
	bool m_lightNeedsUpdate;
	bool m_dayCycle;
	GLfloat m_time;
	GLfloat m_dayLength;

	Texture m_tileSheetGround, m_tileSheetWorld, m_tileSheetSky;

	bool m_initialized;


	bool m_filling;
	Sint8 m_fillLayer;
	Uint16 m_fillTileId;
	Vector2<Sint32> m_fillStart, m_fillEnd;


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
			Entity(Sint16 p_id, Vector2<Sint32> p_pos) : id(p_id), pos(p_pos)
			{}
			Sint16 id = -1;
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

	std::vector<Color> m_worldColors;
};
