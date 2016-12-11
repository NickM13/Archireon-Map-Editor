#include "engine\game\map\Map.h"
#include "engine\gfx\font\Font.h"

#include <direct.h>
#include <fstream>

bool Map::init()
{
	for(Uint16 i = 0; i < 3; i++)
	{
		m_tileData[i] = new Uint16*[m_mapSize.x];
		for(Uint16 x = 0; x < m_mapSize.x; x++)
		{
			m_tileData[i][x] = new Uint16[m_mapSize.y];
			for(Uint16 y = 0; y < m_mapSize.y; y++)
			{
				m_tileData[i][x][y] = 0;
			}
		}
	}

	m_initialized = true;

	return true;
}

void Map::resize(Vector2<Uint16> p_zoneSize)
{
	std::cout << "Resizing map from size <" << m_mapSize.x << ", " << m_mapSize.y << "> to size <" << p_zoneSize.x << ", " << p_zoneSize.y << ">... ";
	Vector2<Uint16> _zoneSize = m_mapSize;
	Uint16** _tileData[3];
	for(Uint16 i = 0; i < 3; i++)
	{
		_tileData[i] = new Uint16*[_zoneSize.x];
		for(Uint16 x = 0; x < _zoneSize.x; x++)
		{
			_tileData[i][x] = new Uint16[_zoneSize.y];
			for(Uint16 y = 0; y < _zoneSize.y; y++)
			{
				_tileData[i][x][y] = m_tileData[i][x][y];
			}
		}
	}
	if(m_initialized)
		for(Uint16 i = 0; i < 3; i++)
		{
			for(Uint16 x = 0; x < m_mapSize.x; x++)
				delete[] m_tileData[i][x];
			delete[] m_tileData[i];
		}

	m_mapSize = p_zoneSize;
	init();
	for(Uint16 i = 0; i < 3; i++)
	{
		for(Uint16 x = 0; x < min(_zoneSize.x, m_mapSize.x); x++)
		{
			for(Uint16 y = 0; y < min(_zoneSize.y, m_mapSize.y); y++)
			{
				m_tileData[i][x][y] = _tileData[i][x][y];
			}
		}
	}
	std::cout << "Complete." << std::endl;
}

void Map::setTextureGround(Texture p_texture)
{
	m_tileSheetGround = p_texture;
}
Texture Map::getTextureGround()
{
	return m_tileSheetGround;
}

void Map::setTextureWorld(Texture p_texture)
{
	m_tileSheetWorld = p_texture;
}
Texture Map::getTextureWorld()
{
	return m_tileSheetWorld;
}

void Map::setTextureSky(Texture p_texture)
{
	m_tileSheetSky = p_texture;
}
Texture Map::getTextureSky()
{
	return m_tileSheetSky;
}

void Map::setViewSize(Rect p_viewSize)
{
	m_viewSize = p_viewSize;
}
void Map::setTileSize(Sint16 p_tileSize)
{
	m_tileSize = p_tileSize;
}
void Map::setLayerVisible(Sint16 p_layer, bool p_visible)
{
	m_layerVisible[p_layer] = p_visible;
}
void Map::setGridVisible(bool p_active)
{
	m_grid = p_active;
}

void Map::setName(std::string p_zoneName)
{
	m_mapName = p_zoneName;
}
std::string Map::getName()
{
	return m_mapName;
}
Vector2<Uint16> Map::getSize()
{
	return m_mapSize;
}

void Map::setTile(Sint8 p_layer, Sint32 x, Sint32 y, Uint16 p_tile)
{
	if(x >= 0 && x < m_mapSize.x && y >= 0 && y < m_mapSize.y)
		m_tileData[p_layer][x][y] = p_tile;
}
Uint16 Map::getTile(Sint8 p_layer, Sint32 x, Sint32 y)
{
	return m_tileData[p_layer][x][y];
}

Uint16 Map::addWorldObject(WorldObject p_object)
{
	m_worldObjects.push_back(p_object);
	return Uint16(m_worldObjects.size());
}
Map::WorldObject& Map::getWorldObject(Uint16 p_index)
{
	return m_worldObjects[p_index];
}
Uint16 Map::getWorldObjectSize()
{
	return Uint16(m_worldObjects.size());
}
void Map::removeWorldObject(Uint16 p_index)
{
	m_worldObjects.erase(m_worldObjects.begin() + p_index);
	for(Uint16 x = 0; x < m_mapSize.x; x++)
		for(Uint16 y = 0; y < m_mapSize.y; y++)
		{
			if(m_tileData[1][x][y] > p_index)
				m_tileData[1][x][y]--;
			else if(m_tileData[1][x][y] == p_index)
				m_tileData[1][x][y] = 0;
		}
}

Uint16 Map::addEntity(Entity p_entity)
{
	m_entities.push_back(p_entity);
	return Uint16(m_entities.size());
}
Map::Entity& Map::getEntity(Uint16 p_index)
{
	if(p_index < m_entities.size())
		return m_entities[p_index];
	return Entity();
}
Uint16 Map::getEntitySize()
{
	return Uint16(m_entities.size());
}
void Map::removeEntity(Uint16 p_index)
{
	m_entities.erase(m_entities.begin() + p_index);
}

void Map::render(Vector2<GLfloat> p_camPos, GLfloat p_zoom)
{
	glColor3f(1, 1, 1);

	GLfloat _tileSize = m_tileSize + p_zoom;
	Uint32 _tile;
	Vector2<Sint32> _tilesheetSize;
	_tilesheetSize = getTextureGround().getSize();
	glBindTexture(GL_TEXTURE_2D, getTextureGround().getId());

	GLfloat _texCorrection = 1.f / (_tilesheetSize.x * 2);
	p_camPos = p_camPos * _tileSize + Vector2<GLfloat>(_tileSize, _tileSize);

	glPushMatrix();
	{
		if(m_layerVisible[0]) // Ground
		{
			for(Sint32 x = Sint32(floor(m_viewSize.x / _tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / _tileSize) - 1); x++)
			{
				for(Sint32 y = Sint32(floor(m_viewSize.y / _tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / _tileSize) - 1); y++)
				{
					if(x + p_camPos.x / _tileSize >= 0 && y + p_camPos.y / _tileSize >= 0 && x + p_camPos.x / _tileSize < getSize().x && y + p_camPos.y / _tileSize < getSize().y)
					{
						_tile = getTile(0, Sint32(x + p_camPos.x / _tileSize), Sint32(y + p_camPos.y / _tileSize));
						glPushMatrix();
						{
							glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), _tileSize) + (p_camPos.x < 0 ? 0 : _tileSize)) + (p_camPos.x < 0 ? 1 : 0),
								GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), _tileSize) + (p_camPos.y < 0 ? 0 : _tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
							glBegin(GL_QUADS);
							{
								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + _texCorrection),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize))) - _texCorrection);
								glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
									GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - _texCorrection),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize))) - _texCorrection);
								glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
									GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - _texCorrection),
									GLfloat(1.f - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize)) + _texCorrection));
								glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
									GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + _texCorrection),
									GLfloat(1.f - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize)) + _texCorrection));
								glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
									GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));
							}
							glEnd();
						}
						glPopMatrix();
					}
				}
			}
		}


		_tilesheetSize = getTextureWorld().getSize();
		_texCorrection = 1.f / (_tilesheetSize.x * 2);
		glBindTexture(GL_TEXTURE_2D, getTextureWorld().getId());

		if(m_layerVisible[1]) // World
		{
			for(Sint32 x = Sint32(floor(m_viewSize.x / _tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / _tileSize) - 1); x++)
			{
				for(Sint32 y = Sint32(floor(m_viewSize.y / _tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / _tileSize) - 1); y++)
				{
					if(x + p_camPos.x / _tileSize >= 0 && y + p_camPos.y / _tileSize >= 0 && x + p_camPos.x / _tileSize < getSize().x && y + p_camPos.y / _tileSize < getSize().y)
					{
						_tile = getWorldObject(getTile(1, Sint32(x + p_camPos.x / _tileSize), Sint32(y + p_camPos.y / _tileSize))).m_tileTex;
						glPushMatrix();
						{
							glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), _tileSize) + (p_camPos.x < 0 ? 0 : _tileSize)) + (p_camPos.x < 0 ? 1 : 0),
								GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), _tileSize) + (p_camPos.y < 0 ? 0 : _tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
							glBegin(GL_QUADS);
							{
								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + _texCorrection),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize))) - _texCorrection);
								glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
									GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - _texCorrection),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize))) - _texCorrection);
								glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
									GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - _texCorrection),
									GLfloat(1.f - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize)) + _texCorrection));
								glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
									GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + _texCorrection),
									GLfloat(1.f - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize)) + _texCorrection));
								glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
									GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));
							}
							glEnd();
						}
						glPopMatrix();
					}
				}
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			Uint16 _tileType = 0;
			Uint16** _tileTypes;
			_tileTypes = new Uint16*[getSize().x];
			for(Sint32 x = 0; x < getSize().x; x++)
			{
				_tileTypes[x] = new Uint16[getSize().y];
				for(Sint32 y = 0; y < getSize().y; y++)
					_tileTypes[x][y] = getWorldObject(getTile(1, x, y)).m_interactionType;
			}
			for(Sint32 x = Sint32(floor(m_viewSize.x / _tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / _tileSize) - 1); x++)
			{
				for(Sint32 y = Sint32(floor(m_viewSize.y / _tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / _tileSize) - 1); y++)
				{
					if(x + p_camPos.x / _tileSize >= 0 && y + p_camPos.y / _tileSize >= 0 && x + p_camPos.x / _tileSize < getSize().x && y + p_camPos.y / _tileSize < getSize().y)
					{
						_tile = getWorldObject(getTile(1, Sint32(x + p_camPos.x / _tileSize), Sint32(y + p_camPos.y / _tileSize))).m_tileTex;
						_tileType = _tileTypes[Sint32(x + p_camPos.x / _tileSize)][Sint32(y + p_camPos.y / _tileSize)];
						if(_tileType != 0)
						{
							switch(_tileType)
							{
							case 1: // Solid
								glColor4f(1, 0, 0, 1);
								break;
							case 2: // Switch
								glColor4f(0, 1, 0, 1);
								break;
							case 3: // Solid Switch
								glColor4f(0, 1, 0, 1);
								break;
							case 4: // Portal
								glColor4f(1, 0, 1, 1);
								break;
							}
							glPushMatrix();
							{
								glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), _tileSize) + (p_camPos.x < 0 ? 0 : _tileSize)) + (p_camPos.x < 0 ? 1 : 0),
									GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), _tileSize) + (p_camPos.y < 0 ? 0 : _tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
								glBegin(GL_LINES);
								{
									//Left
									if(Sint32(x + p_camPos.x / _tileSize) == 0 || _tileTypes[Sint32(x + p_camPos.x / _tileSize) - 1][Sint32(y + p_camPos.y / _tileSize)] != _tileType)
									{
										glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)) + 1,
											GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));
										glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)) + 1,
											GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));
									}

									//Top
									if(Sint32(y + p_camPos.y / _tileSize) == 0 || _tileTypes[Sint32(x + p_camPos.x / _tileSize)][Sint32(y + p_camPos.y / _tileSize) - 1] != _tileType)
									{
										glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
											GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)) + 1);
										glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
											GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)) + 1);
									}

									//Right
									if(Sint32(x + p_camPos.x / _tileSize) == getSize().x - 1 || _tileTypes[Sint32(x + p_camPos.x / _tileSize) + 1][Sint32(y + p_camPos.y / _tileSize)] != _tileType)
									{
										glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
											GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));
										glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
											GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));
									}

									//Bottom
									if(Sint32(y + p_camPos.y / _tileSize) == getSize().y - 1 || _tileTypes[Sint32(x + p_camPos.x / _tileSize)][Sint32(y + p_camPos.y / _tileSize) + 1] != _tileType)
									{
										glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
											GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));
										glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
											GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));
									}
								}
								glEnd();

								switch(_tileType)
								{
								case 1: // Solid
									glColor4f(1, 0, 0, 0.05f);
									break;
								case 2: // Switch
									glColor4f(0, 1, 0, 0.05f);
									break;
								case 3: // Solid Switch
									glColor4f(0, 1, 0, 0.05f);
									break;
								case 4: // Portal
									glColor4f(1, 0, 1, 0.05f);
									break;
								}
								glBegin(GL_QUADS);
								{
									glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
										GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));

									glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
										GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));

									glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
										GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));

									glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
										GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));
								}
								glEnd();
							}
							glPopMatrix();
						}
					}
				}
			}
			for(Sint32 x = 0; x < getSize().x; x++)
				delete[] _tileTypes[x];
			delete[] _tileTypes;
		}

		Font::getInstance().setAlignment(ALIGN_CENTER);

		_texCorrection = 0;
		if(m_layerVisible[2]) // Entities
		{
			glColor3f(1, 1, 1);
			for(Uint16 i = 1; i < m_entities.size(); i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_entities[i].m_entityTex.getId());
				_tilesheetSize = m_entities[i].m_entityTex.getSize();
				if(p_zoom != 0)
					_texCorrection = 1.f / (_tilesheetSize.x * 2);
				glPushMatrix();
				{
					glTranslatef(GLfloat(m_entities[i].m_pos.x + 1) * _tileSize - p_camPos.x, GLfloat(m_entities[i].m_pos.y + 1) * _tileSize - p_camPos.y, 0);
					glBegin(GL_QUADS);
					{
						if(_tilesheetSize.x == 0 || _tilesheetSize.y == 0)
						{
							glBindTexture(GL_TEXTURE_2D, 0);
							glVertex2f(0, _tileSize);
							glVertex2f(_tileSize, _tileSize);
							glVertex2f(_tileSize, 0);
							glVertex2f(0, 0);
						}
						else
						{
							glTexCoord2f((GLfloat(m_tileSize) / _tilesheetSize.x) * (m_entities[i].m_entityTexId % Sint32(ceil(GLfloat(_tilesheetSize.x) / m_tileSize))) + _texCorrection,  -(floor(m_entities[i].m_entityTexId / (GLfloat(_tilesheetSize.x) / m_tileSize)) / (GLfloat(_tilesheetSize.y) / m_tileSize) + (GLfloat(m_tileSize) / _tilesheetSize.y)));
							glVertex2f(0, _tileSize);
							glTexCoord2f((GLfloat(m_tileSize) / _tilesheetSize.x) * (m_entities[i].m_entityTexId % Sint32(ceil(GLfloat(_tilesheetSize.x) / m_tileSize))) + GLfloat(m_tileSize) / _tilesheetSize.x - _texCorrection,  -(floor(m_entities[i].m_entityTexId / (GLfloat(_tilesheetSize.x) / m_tileSize)) / (GLfloat(_tilesheetSize.y) / m_tileSize) + (GLfloat(m_tileSize) / _tilesheetSize.y)));
							glVertex2f(_tileSize, _tileSize);
							glTexCoord2f((GLfloat(m_tileSize) / _tilesheetSize.x) * (m_entities[i].m_entityTexId % Sint32(ceil(GLfloat(_tilesheetSize.x) / m_tileSize))) + GLfloat(m_tileSize) / _tilesheetSize.x - _texCorrection,  -(floor(m_entities[i].m_entityTexId / (GLfloat(_tilesheetSize.x) / m_tileSize)) / (GLfloat(_tilesheetSize.y) / m_tileSize)));
							glVertex2f(_tileSize, 0);
							glTexCoord2f((GLfloat(m_tileSize) / _tilesheetSize.x) * (m_entities[i].m_entityTexId % Sint32(ceil(GLfloat(_tilesheetSize.x) / m_tileSize))) + _texCorrection,  -(floor(m_entities[i].m_entityTexId / (GLfloat(_tilesheetSize.x) / m_tileSize)) / (GLfloat(_tilesheetSize.y) / m_tileSize)));
							glVertex2f(0, 0);
						}
					}
					glEnd();
				}
				glPopMatrix();
			}
			for(Uint16 i = 1; i < m_entities.size(); i++)
				Font::getInstance().print(m_entities[i].m_name, (GLfloat(m_entities[i].m_pos.x + 1) * _tileSize - p_camPos.x) + _tileSize / 2, (GLfloat(m_entities[i].m_pos.y + 1) * _tileSize - p_camPos.y) - Font::getInstance().getHeight() / 2 - 1);
		}



		_tilesheetSize = getTextureSky().getSize();
		_texCorrection = 1.f / (_tilesheetSize.x * 2);
		glBindTexture(GL_TEXTURE_2D, getTextureSky().getId());

		if(m_layerVisible[3]) // Sky
		{
			for(Sint32 x = Sint32(floor(m_viewSize.x / _tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / _tileSize) - 1); x++)
			{
				for(Sint32 y = Sint32(floor(m_viewSize.y / _tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / _tileSize) - 1); y++)
				{
					if(x + p_camPos.x / _tileSize >= 0 && y + p_camPos.y / _tileSize >= 0 && x + p_camPos.x / _tileSize < getSize().x && y + p_camPos.y / _tileSize < getSize().y)
					{
						_tile = getTile(2, Sint32(x + p_camPos.x / _tileSize), Sint32(y + p_camPos.y / _tileSize));
						glPushMatrix();
						{
							glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), _tileSize) + (p_camPos.x < 0 ? 0 : _tileSize)) + (p_camPos.x < 0 ? 1 : 0),
								GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), _tileSize) + (p_camPos.y < 0 ? 0 : _tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
							glBegin(GL_QUADS);
							{
								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + _texCorrection),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize))) - _texCorrection);
								glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
									GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - _texCorrection),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize))) - _texCorrection);
								glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
									GLfloat((y * _tileSize) - (y == (floor(m_viewSize.y / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) - _tileSize : fmod(-p_camPos.y, _tileSize)) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - _texCorrection),
									GLfloat(1.f - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize)) + _texCorrection));
								glVertex2f(GLfloat((x * _tileSize + _tileSize) - (x == (ceil(m_viewSize.w / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) : fmod(-p_camPos.x, _tileSize) + _tileSize) : 0)),
									GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + _texCorrection),
									GLfloat(1.f - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize)) + _texCorrection));
								glVertex2f(GLfloat((x * _tileSize) - (x == (floor(m_viewSize.x / _tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, _tileSize) - _tileSize : fmod(-p_camPos.x, _tileSize)) : 0)),
									GLfloat((y * _tileSize + _tileSize) - (y == (ceil(m_viewSize.h / _tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, _tileSize) : fmod(-p_camPos.y, _tileSize) + _tileSize) : 0)));
							}
							glEnd();
						}
						glPopMatrix();
					}
				}
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		if(m_grid)
		{
			glColor4f(0.75f, 0.75f, 0.75f, 0.5f);

			for(Sint32 x = Sint32(floor(m_viewSize.x / _tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / _tileSize) - 1); x++)
			{
				if(x + p_camPos.x / _tileSize >= 0 && x + p_camPos.x / _tileSize < getSize().x + 1)
				{
					glPushMatrix();
					{
						glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), _tileSize) + (p_camPos.x < 0 ? 0 : _tileSize)) + (p_camPos.x < 0 ? 1 : 0), 0, 0);
						glBegin(GL_LINES);
						{
							glVertex2f(GLfloat(x * _tileSize), GLfloat(max(m_viewSize.y, -p_camPos.y + _tileSize)));
							glVertex2f(GLfloat(x * _tileSize), GLfloat(min(m_viewSize.h, -p_camPos.y + _tileSize + getSize().y * _tileSize)));
						}
						glEnd();
					}
					glPopMatrix();
				}
			}
			for(Sint32 y = Sint32(floor(m_viewSize.y / _tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / _tileSize) - 1); y++)
			{
				if(y + p_camPos.y / _tileSize >= 0 && y + p_camPos.y / _tileSize < getSize().y + 1)
				{
					glPushMatrix();
					{
						glTranslatef(0, GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), _tileSize) + (p_camPos.y < 0 ? 0 : _tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
						glBegin(GL_LINES);
						{
							glVertex2f(GLfloat(max(m_viewSize.x, -p_camPos.x + _tileSize)), GLfloat(y * _tileSize));
							glVertex2f(GLfloat(min(m_viewSize.w, -p_camPos.x + _tileSize + getSize().x * _tileSize)), GLfloat(y * _tileSize));
						}
						glEnd();
					}
					glPopMatrix();
				}
			}
		}
	}
	glPopMatrix();
}

void Map::save()
{

}
void Map::save(std::string p_src)
{

}
bool Map::load()
{
	return true;
}
bool Map::load(std::string p_src)
{
	return true;
}

void Map::createNew()
{
	std::cout << "Clearing map... ";
	init();
	std::cout << "Complete." << std::endl;
}

void Map::clear()
{
	for(Uint16 x = 0; x < m_mapSize.x; x++)
	{
		for(Uint16 y = 0; y < m_mapSize.y; y++)
		{
		}
		for(Uint16 i = 0; i < 3; i++)
			delete[] m_tileData[i][x];
	}
	for(Uint16 i = 0; i < 3; i++)
		delete[] m_tileData[i];
	m_entities.clear();
	m_worldObjects.clear();
}
