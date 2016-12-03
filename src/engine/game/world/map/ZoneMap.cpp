#include "engine\game\world\map\ZoneMap.h"

#include <direct.h>
#include <fstream>

ZoneMap::ZoneMap()
{
	m_initialized = false;
}

ZoneMap::ZoneMap(Vector2<Uint16> p_zoneSize)
{
	m_initialized = false;

	m_zoneSize = p_zoneSize;
	init();
}

ZoneMap::~ZoneMap()
{
	clear();
}

bool ZoneMap::init()
{
	for(Uint16 i = 0; i < 4; i++)
	{
		m_tileData[i] = new Uint16*[m_zoneSize.x];
		for(Uint16 x = 0; x < m_zoneSize.x; x++)
		{
			m_tileData[i][x] = new Uint16[m_zoneSize.y];
			for(Uint16 y = 0; y < m_zoneSize.y; y++)
			{
				m_tileData[i][x][y] = 0;
			}
		}
	}

	m_initialized = true;

	return true;
}

void ZoneMap::resize(Vector2<Uint16> p_zoneSize)
{
	std::cout << "Resizing map from size <" << m_zoneSize.x << ", " << m_zoneSize.y << "> to size <" << p_zoneSize.x << ", " << p_zoneSize.y << ">... ";
	Vector2<Uint16> _zoneSize = m_zoneSize;
	Uint16** _tileData[4];
	for(Uint16 i = 0; i < 4; i++)
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
	clear();
	m_zoneSize = p_zoneSize;
	init();
	for(Uint16 i = 0; i < 4; i++)
	{
		for(Uint16 x = 0; x < min(_zoneSize.x, m_zoneSize.x); x++)
		{
			for(Uint16 y = 0; y < min(_zoneSize.y, m_zoneSize.y); y++)
			{
				m_tileData[i][x][y] = _tileData[i][x][y];
			}
		}
	}
	std::cout << "Complete." << std::endl;
}

void ZoneMap::setTextureGround(Texture p_texture)
{
	m_tileSheetGround = p_texture;
}
Texture ZoneMap::getTextureGround()
{
	return m_tileSheetGround;
}

void ZoneMap::setTextureWorld(Texture p_texture)
{
	m_tileSheetWorld = p_texture;
}
Texture ZoneMap::getTextureWorld()
{
	return m_tileSheetWorld;
}

void ZoneMap::setTextureSky(Texture p_texture)
{
	m_tileSheetSky = p_texture;
}
Texture ZoneMap::getTextureSky()
{
	return m_tileSheetSky;
}

void ZoneMap::setViewSize(Rect p_viewSize)
{
	m_viewSize = p_viewSize;
}
void ZoneMap::setTileSize(Sint16 p_tileSize)
{
	m_tileSize = p_tileSize;
}
void ZoneMap::setLayerVisible(Sint16 p_layer, bool p_visible)
{
	m_layerVisible[p_layer] = p_visible;
}
void ZoneMap::setGridVisible(bool p_active)
{
	m_grid = p_active;
}

void ZoneMap::setName(std::string p_zoneName)
{
	m_zoneName = p_zoneName;
}
std::string ZoneMap::getName()
{
	return m_zoneName;
}
Vector2<Uint16> ZoneMap::getSize()
{
	return m_zoneSize;
}

void ZoneMap::setTile(Sint8 p_layer, Sint32 x, Sint32 y, Uint16 p_tile)
{
	if(x >= 0 && x < m_zoneSize.x && y >= 0 && y < m_zoneSize.y)
	{
		if(p_layer != 2)
		{
			m_tileData[p_layer][x][y] = p_tile;
		}
		else
		{

		}
	}
}
Uint16 ZoneMap::getTile(Sint8 p_layer, Sint32 x, Sint32 y)
{
	return m_tileData[p_layer][x][y];
}

Uint16 ZoneMap::addWorldObject(WorldObject p_object)
{
	m_worldObjects.push_back(p_object);
	return Uint16(m_worldObjects.size());
}
ZoneMap::WorldObject& ZoneMap::getWorldObject(Uint16 p_index)
{
	return m_worldObjects[p_index];
}
Uint16 ZoneMap::getWorldObjectSize()
{
	return Uint16(m_worldObjects.size());
}
void ZoneMap::removeWorldObject(Uint16 p_index)
{
	m_worldObjects.erase(m_worldObjects.begin() + p_index);
	for(Uint16 x = 0; x < m_zoneSize.x; x++)
		for(Uint16 y = 0; y < m_zoneSize.y; y++)
		{
			if(m_tileData[1][x][y] > p_index)
				m_tileData[1][x][y]--;
			else if(m_tileData[1][x][y] == p_index)
				m_tileData[1][x][y] = 0;
		}
}

Uint16 ZoneMap::addEntity(Entity p_entity)
{
	m_entities.push_back(p_entity);
	return Uint16(m_entities.size());
}
ZoneMap::Entity& ZoneMap::getEntity(Uint16 p_index)
{
	return m_entities[p_index];
}
Uint16 ZoneMap::getEntitySize()
{
	return Uint16(m_entities.size());
}
void ZoneMap::removeEntity(Uint16 p_index)
{
	m_entities.erase(m_entities.begin() + p_index);
	for(Uint16 x = 0; x < m_zoneSize.x; x++)
		for(Uint16 y = 0; y < m_zoneSize.y; y++)
		{
			if(m_tileData[2][x][y] > p_index)
				m_tileData[2][x][y]--;
			else if(m_tileData[2][x][y] == p_index)
				m_tileData[2][x][y] = 0;
		}
}

void ZoneMap::render(Vector2<Sint32> p_camPos)
{
	glColor3f(1, 1, 1);

	Uint32 _tile;
	Vector2<Sint32> _tilesheetSize;
	_tilesheetSize = getTextureGround().getSize();
	glBindTexture(GL_TEXTURE_2D, getTextureGround().getId());

	if(m_layerVisible[0])
	{
		for(Sint32 x = Sint32(floor(m_viewSize.x / m_tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / m_tileSize) - 1); x++)
		{
			for(Sint32 y = Sint32(floor(m_viewSize.y / m_tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / m_tileSize) - 1); y++)
			{
				if(x + p_camPos.x / m_tileSize >= 0 && y + p_camPos.y / m_tileSize >= 0 && x + p_camPos.x / m_tileSize < getSize().x && y + p_camPos.y / m_tileSize < getSize().y)
				{
					_tile = getTile(0, Sint32(x + p_camPos.x / m_tileSize), Sint32(y + p_camPos.y / m_tileSize));
					glPushMatrix();
					{
						glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), m_tileSize) + (p_camPos.x < 0 ? 0 : m_tileSize)) + (p_camPos.x < 0 ? 1 : 0),
							GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), m_tileSize) + (p_camPos.y < 0 ? 0 : m_tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
						glBegin(GL_QUADS);
						{
							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0)),
								GLfloat((y * m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0)));

							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize + m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0)),
								GLfloat((y * m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0)));

							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize + m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0)),
								GLfloat((y * m_tileSize + m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0)));

							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0)),
								GLfloat((y * m_tileSize + m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0)));
						}
						glEnd();
					}
					glPopMatrix();
				}
			}
		}
	}


	_tilesheetSize = getTextureWorld().getSize();
	glBindTexture(GL_TEXTURE_2D, getTextureWorld().getId());

	if(m_layerVisible[1])
	{
		for(Sint32 x = Sint32(floor(m_viewSize.x / m_tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / m_tileSize) - 1); x++)
		{
			for(Sint32 y = Sint32(floor(m_viewSize.y / m_tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / m_tileSize) - 1); y++)
			{
				if(x + p_camPos.x / m_tileSize >= 0 && y + p_camPos.y / m_tileSize >= 0 && x + p_camPos.x / m_tileSize < getSize().x && y + p_camPos.y / m_tileSize < getSize().y)
				{
					_tile = getWorldObject(getTile(1, Sint32(x + p_camPos.x / m_tileSize), Sint32(y + p_camPos.y / m_tileSize))).m_tileTex;
					glPushMatrix();
					{
						glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), m_tileSize) + (p_camPos.x < 0 ? 0 : m_tileSize)) + (p_camPos.x < 0 ? 1 : 0),
							GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), m_tileSize) + (p_camPos.y < 0 ? 0 : m_tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
						glBegin(GL_QUADS);
						{
							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0)),
								GLfloat((y * m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0)));

							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize + m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0)),
								GLfloat((y * m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0)));

							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize + m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0)),
								GLfloat((y * m_tileSize + m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0)));

							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0)),
								GLfloat((y * m_tileSize + m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0)));
						}
						glEnd();
					}
					glPopMatrix();
				}
			}
		}
	}



	if(m_layerVisible[2])
	{
		for(Sint32 x = Sint32(floor(m_viewSize.x / m_tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / m_tileSize) - 1); x++)
		{
			for(Sint32 y = Sint32(floor(m_viewSize.y / m_tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / m_tileSize) - 1); y++)
			{
				if(x + p_camPos.x / m_tileSize >= 0 && y + p_camPos.y / m_tileSize >= 0 && x + p_camPos.x / m_tileSize < getSize().x && y + p_camPos.y / m_tileSize < getSize().y)
				{
					_tile = getTile(2, Sint32(x + p_camPos.x / m_tileSize), Sint32(y + p_camPos.y / m_tileSize));
					if(_tile != 0 && getEntity(_tile).m_entityTex.getId() > 0)
					{
						glBindTexture(GL_TEXTURE_2D, getEntity(_tile).m_entityTex.getId());
						_tilesheetSize = getEntity(_tile).m_entityTex.getSize();
						_tile = getEntity(_tile).m_entityTexId;
						glPushMatrix();
						{
							glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), m_tileSize) + (p_camPos.x < 0 ? 0 : m_tileSize)) + (p_camPos.x < 0 ? 1 : 0),
								GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), m_tileSize) + (p_camPos.y < 0 ? 0 : m_tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
							glBegin(GL_QUADS);
							{
								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0) / _tilesheetSize.x),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0) / _tilesheetSize.y)));
								glVertex2f(GLfloat((x * m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0)),
									GLfloat((y * m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.x),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0) / _tilesheetSize.y)));
								glVertex2f(GLfloat((x * m_tileSize + m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0)),
									GLfloat((y * m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.x),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.y)));
								glVertex2f(GLfloat((x * m_tileSize + m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0)),
									GLfloat((y * m_tileSize + m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0)));

								glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0) / _tilesheetSize.x),
									GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.y)));
								glVertex2f(GLfloat((x * m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0)),
									GLfloat((y * m_tileSize + m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0)));
							}
							glEnd();
						}
						glPopMatrix();
					}
				}
			}
		}
	}


	_tilesheetSize = getTextureSky().getSize();
	glBindTexture(GL_TEXTURE_2D, getTextureSky().getId());

	if(m_layerVisible[3])
	{
		for(Sint32 x = Sint32(floor(m_viewSize.x / m_tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / m_tileSize) - 1); x++)
		{
			for(Sint32 y = Sint32(floor(m_viewSize.y / m_tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / m_tileSize) - 1); y++)
			{
				if(x + p_camPos.x / m_tileSize >= 0 && y + p_camPos.y / m_tileSize >= 0 && x + p_camPos.x / m_tileSize < getSize().x && y + p_camPos.y / m_tileSize < getSize().y)
				{
					_tile = getTile(3, Sint32(x + p_camPos.x / m_tileSize), Sint32(y + p_camPos.y / m_tileSize));
					glPushMatrix();
					{
						glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), m_tileSize) + (p_camPos.x < 0 ? 0 : m_tileSize)) + (p_camPos.x < 0 ? 1 : 0),
							GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), m_tileSize) + (p_camPos.y < 0 ? 0 : m_tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
						glBegin(GL_QUADS);
						{
							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0)),
								GLfloat((y * m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0)));

							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize + m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0)),
								GLfloat((y * m_tileSize) - (y == (floor(m_viewSize.y / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.y, m_tileSize)) : 0)));

							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) + 1.f / (_tilesheetSize.x / m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize + m_tileSize) - (x == (ceil(m_viewSize.w / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) : fmod(-p_camPos.x, m_tileSize) + m_tileSize) : 0)),
								GLfloat((y * m_tileSize + m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0)));

							glTexCoord2f(GLfloat(GLfloat(_tile % (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.x / m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0) / _tilesheetSize.x),
								GLfloat(1 - (floor(_tile / (_tilesheetSize.x / m_tileSize)) / (_tilesheetSize.y / m_tileSize) + 1.f / (_tilesheetSize.y / m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0) / _tilesheetSize.y)));
							glVertex2f(GLfloat((x * m_tileSize) - (x == (floor(m_viewSize.x / m_tileSize) - 1) ? (p_camPos.x < 0 ? fmod(-p_camPos.x - 1, m_tileSize) - m_tileSize : fmod(-p_camPos.x, m_tileSize)) : 0)),
								GLfloat((y * m_tileSize + m_tileSize) - (y == (ceil(m_viewSize.h / m_tileSize) - 1) ? (p_camPos.y < 0 ? fmod(-p_camPos.y - 1, m_tileSize) : fmod(-p_camPos.y, m_tileSize) + m_tileSize) : 0)));
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

		for(Sint32 x = Sint32(floor(m_viewSize.x / m_tileSize)) - 1; x <= Sint32(ceil(m_viewSize.w / m_tileSize) - 1); x++)
		{
			if(x + p_camPos.x / m_tileSize >= 0 && x + p_camPos.x / m_tileSize < getSize().x + 1)
			{
				glPushMatrix();
				{
					glTranslatef(GLfloat(fmod(-p_camPos.x + (p_camPos.x < 0 ? -1 : 0), m_tileSize) + (p_camPos.x < 0 ? 0 : m_tileSize)) + (p_camPos.x < 0 ? 1 : 0), 0, 0);
					glBegin(GL_LINES);
					{
						glVertex2f(GLfloat(x * m_tileSize), GLfloat(max(m_viewSize.y, -p_camPos.y + m_tileSize)));
						glVertex2f(GLfloat(x * m_tileSize), GLfloat(min(m_viewSize.h, -p_camPos.y + m_tileSize + getSize().y * m_tileSize)));
					}
					glEnd();
				}
				glPopMatrix();
			}
		}
		for(Sint32 y = Sint32(floor(m_viewSize.y / m_tileSize)) - 1; y <= Sint32(ceil(m_viewSize.h / m_tileSize) - 1); y++)
		{
			if(y + p_camPos.y / m_tileSize >= 0 && y + p_camPos.y / m_tileSize < getSize().y + 1)
			{
				glPushMatrix();
				{
					glTranslatef(0, GLfloat(fmod(-p_camPos.y + (p_camPos.y < 0 ? -1 : 0), m_tileSize) + (p_camPos.y < 0 ? 0 : m_tileSize)) + (p_camPos.y < 0 ? 1 : 0), 0);
					glBegin(GL_LINES);
					{
						glVertex2f(GLfloat(max(m_viewSize.x, -p_camPos.x + m_tileSize)), GLfloat(y * m_tileSize));
						glVertex2f(GLfloat(min(m_viewSize.w, -p_camPos.x + m_tileSize + getSize().x * m_tileSize)), GLfloat(y * m_tileSize));
					}
					glEnd();
				}
				glPopMatrix();
			}
		}
	}
}

void ZoneMap::save()
{
	save(m_zoneName);
}
void ZoneMap::save(std::string p_zoneName)
{
	std::cout << "Saving map zones\\" << p_zoneName << "... ";

	m_zoneName = p_zoneName;
	std::ofstream _file;
	_mkdir(std::string("zones\\").c_str());
	_mkdir(std::string("zones\\" + m_zoneName).c_str());
	_mkdir(std::string("zones\\" + m_zoneName + "\\entities").c_str());
	_file.open(std::string("zones\\" + m_zoneName + "\\Info.cfg"), std::ios::binary);
	{
		FileExt::writeShort(_file, m_zoneSize.x);
		FileExt::writeShort(_file, m_zoneSize.y);
	}
	_file.close();

	_file.open(std::string("zones\\" + m_zoneName + "\\Ground.tmf"));
	{
		Uint16 _tCount = 0;
		Uint16 _tile = m_tileData[0][0][0];
		for(Uint16 x = 0; x < m_zoneSize.x; x++)
		{
			for(Uint16 y = 0; y < m_zoneSize.y; y++)
			{
				if(_tile == m_tileData[0][x][y] && _tCount < 255)
					_tCount++;
				else
				{
					FileExt::writeShort(_file, _tCount);
					FileExt::writeShort(_file, _tile);

					_tCount = 1;
					_tile = m_tileData[0][x][y];
				}
			}
		}
		if(_tCount != 0)
		{
			FileExt::writeShort(_file, _tCount);
			FileExt::writeShort(_file, _tile);
		}
	}
	_file.close();

	_file.open(std::string("zones\\" + m_zoneName + "\\World.tmf"));
	{
		FileExt::writeShort(_file, Uint16(m_worldObjects.size()));
		std::vector<Uint8> _objData;
		for(Uint16 i = 0; i < Uint16(m_worldObjects.size()); i++)
		{
			_objData.push_back(Uint8(min(m_worldObjects[i].m_name.length(), 128)));
			for(Uint16 j = 0; j < Uint16(min(m_worldObjects[i].m_name.length(), 128)); j++)
				_objData.push_back(m_worldObjects[i].m_name[j]);
			_objData.push_back(Uint8((m_worldObjects[i].m_interactionType & 0xFF00) >> 8));
			_objData.push_back(Uint8((m_worldObjects[i].m_interactionType & 0xFF)));
			_objData.push_back(Uint8((m_worldObjects[i].m_tileTex & 0xFF00) >> 8));
			_objData.push_back(Uint8((m_worldObjects[i].m_tileTex & 0xFF)));
			switch(m_worldObjects[i].m_interactionType)
			{
			case 0: // NONE

				break;
			case 1: // SOLID

				break;
			case 2: // SWITCH
				_objData.push_back(Uint8((m_worldObjects[i].m_frequency & 0xFF00) >> 8));
				_objData.push_back(Uint8((m_worldObjects[i].m_frequency & 0xFF)));
				break;
			case 3: // SOLID SWITCH
				_objData.push_back(Uint8((m_worldObjects[i].m_frequency & 0xFF00) >> 8));
				_objData.push_back(Uint8((m_worldObjects[i].m_frequency & 0xFF)));
				break;
			case 4: // PORTAL
				_objData.push_back(Uint8(min(m_worldObjects[i].m_portalDest.length(), 128)));
				for(Uint16 j = 0; j < Uint16(min(m_worldObjects[i].m_portalDest.length(), 128)); j++)
					_objData.push_back(m_worldObjects[i].m_portalDest[j]);
				_objData.push_back(Uint8((m_worldObjects[i].m_destX & 0xFF00) >> 8));
				_objData.push_back(Uint8((m_worldObjects[i].m_destX & 0xFF)));
				_objData.push_back(Uint8((m_worldObjects[i].m_destY & 0xFF00) >> 8));
				_objData.push_back(Uint8((m_worldObjects[i].m_destY & 0xFF)));
				break;
			default:

				break;
			}

			FileExt::writeShort(_file, Uint16(_objData.size()));
			for(Uint16 j = 0; j < Uint16(_objData.size()); j++)
			{
				FileExt::writeChar(_file, _objData[j]);
			}
			_objData.clear();
		}


		Uint16 _tCount = 0;
		Uint16 _tile = m_tileData[1][0][0];
		for(Uint16 x = 0; x < m_zoneSize.x; x++)
		{
			for(Uint16 y = 0; y < m_zoneSize.y; y++)
			{
				if(_tile == m_tileData[1][x][y] && _tCount < 255)
					_tCount++;
				else
				{
					FileExt::writeShort(_file, _tCount);
					FileExt::writeShort(_file, _tile);

					_tCount = 1;
					_tile = m_tileData[1][x][y];
				}
			}
		}
		if(_tCount != 0)
		{
			FileExt::writeShort(_file, _tCount);
			FileExt::writeShort(_file, _tile);
		}
	}
	_file.close();

	_file.open(std::string("zones\\" + m_zoneName + "\\Entity.tmf"));
	{
		FileExt::writeShort(_file, Uint16(m_entities.size()));
		std::vector<Uint8> _objData;
		for(Uint16 i = 0; i < Uint16(m_entities.size()); i++)
		{
			_objData.push_back(Uint8(min(m_entities[i].m_name.length(), 128)));
			for(Uint16 j = 0; j < Uint16(min(m_entities[i].m_name.length(), 128)); j++)
				_objData.push_back(m_entities[i].m_name[j]);
			_objData.push_back(Uint8(min(m_entities[i].m_entityTex.getName().length(), 128)));
			for(Uint16 j = 0; j < Uint16(min(m_entities[i].m_entityTex.getName().length(), 128)); j++)
				_objData.push_back(m_entities[i].m_entityTex.getName()[j]);
			_objData.push_back(Uint8((m_entities[i].m_entityTexId & 0xFF00) >> 8));
			_objData.push_back(Uint8((m_entities[i].m_entityTexId & 0xFF)));

			FileExt::writeShort(_file, Uint16(_objData.size()));
			for(Uint16 j = 0; j < Uint16(_objData.size()); j++)
			{
				FileExt::writeChar(_file, _objData[j]);
			}
			_objData.clear();
		}


		Uint16 _tCount = 0;
		Uint16 _tile = m_tileData[2][0][0];
		for(Uint16 x = 0; x < m_zoneSize.x; x++)
		{
			for(Uint16 y = 0; y < m_zoneSize.y; y++)
			{
				if(_tile == m_tileData[2][x][y] && _tCount < 255)
					_tCount++;
				else
				{
					FileExt::writeShort(_file, _tCount);
					FileExt::writeShort(_file, _tile);

					_tCount = 1;
					_tile = m_tileData[2][x][y];
				}
			}
		}
		if(_tCount != 0)
		{
			FileExt::writeShort(_file, _tCount);
			FileExt::writeShort(_file, _tile);
		}
	}
	_file.close();

	for(Uint16 i = 1; i < Uint16(m_entities.size()); i++)
	{
		_file.open(std::string("zones\\" + m_zoneName + "\\entities\\" + m_entities[i].m_name + ".script"));
		for(Uint16 j = 0; j < m_entities[i].m_script.length(); j++)
			FileExt::writeChar(_file, m_entities[i].m_script[j]);
		_file.close();
	}

	_file.open(std::string("zones\\" + m_zoneName + "\\Sky.tmf"));
	{
		Uint16 _tCount = 0;
		Uint16 _tile = m_tileData[3][0][0];
		for(Uint16 x = 0; x < m_zoneSize.x; x++)
		{
			for(Uint16 y = 0; y < m_zoneSize.y; y++)
			{
				if(_tile == m_tileData[3][x][y] && _tCount < 255)
					_tCount++;
				else
				{
					FileExt::writeShort(_file, _tCount);
					FileExt::writeShort(_file, _tile);

					_tCount = 1;
					_tile = m_tileData[3][x][y];
				}
			}
		}
		if(_tCount != 0)
		{
			FileExt::writeShort(_file, _tCount);
			FileExt::writeShort(_file, _tile);
		}
	}
	_file.close();

	std::cout << "Complete." << std::endl;
}

bool ZoneMap::load()
{
	return load(m_zoneName);
}

bool ZoneMap::load(std::string p_zoneName)
{
	std::cout << "Loading map zones\\" << p_zoneName << "... ";

	std::ifstream _file;
	_file.open(std::string("zones\\" + p_zoneName + "\\Info.cfg").c_str(), std::ios::binary);
	{
		if(!_file.good())
		{
			std::cout << "No Info.cfg file found." << std::endl;
			return false;
		}
	}
	_file.close();
	_file.open(std::string("zones\\" + p_zoneName + "\\Ground.tmf").c_str(), std::ios::binary);
	{
		if(!_file.good())
		{
			std::cout << "No Ground.tmf file found." << std::endl;
			return false;
		}
	}
	_file.close();
	_file.open(std::string("zones\\" + p_zoneName + "\\World.tmf").c_str(), std::ios::binary);
	{
		if(!_file.good())
		{
			std::cout << "No World.tmf file found." << std::endl;
			return false;
		}
	}
	_file.close();
	_file.open(std::string("zones\\" + p_zoneName + "\\Entity.tmf").c_str(), std::ios::binary);
	{
		if(!_file.good())
		{
			std::cout << "No Entity.tmf file found." << std::endl;
			return false;
		}
	}
	_file.close();
	_file.open(std::string("zones\\" + p_zoneName + "\\Sky.tmf").c_str(), std::ios::binary);
	{
		if(!_file.good())
		{
			std::cout << "No Sky.tmf file found." << std::endl;
			return false;
		}
	}
	_file.close();



	clear();

	m_zoneName = p_zoneName;

	Uint32 _length, _index;
	char* _data;
	_file.open(std::string("zones\\" + p_zoneName + "\\Info.cfg").c_str(), std::ios::binary);
	{
		_index = 0;

		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		_data = new char[_length];
		_file.read(_data, _length);

		m_zoneSize.x = FileExt::readShort(_data, _index);
		m_zoneSize.y = FileExt::readShort(_data, _index);

		init();
		delete[] _data;
	}
	_file.close();

	Uint16 _tileCount;
	Uint16 _tileId, _mapIndex;

	_file.open(std::string("zones\\" + p_zoneName + "\\Ground.tmf").c_str(), std::ios::binary);
	{
		_mapIndex = 0;
		_index = 0;

		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		_data = new char[_length];
		_file.read(_data, _length);

		while(_mapIndex < m_zoneSize.x * m_zoneSize.y && _index < _length)
		{
			_tileCount = FileExt::readShort(_data, _index);
			_tileId = FileExt::readShort(_data, _index);

			for(Uint16 i = 0; i < _tileCount; i++)
			{
				m_tileData[0][int(floor(GLfloat(_mapIndex) / m_zoneSize.x))][_mapIndex % m_zoneSize.x] = _tileId;
				_mapIndex++;
			}
		}
		delete[] _data;
	}
	_file.close();

	_file.open(std::string("zones\\" + p_zoneName + "\\World.tmf").c_str(), std::ios::binary);
	{
		_mapIndex = 0;
		_index = 0;

		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		_data = new char[_length];
		_file.read(_data, _length);


		Uint16 _objCount = FileExt::readShort(_data, _index);
		Uint16 _objNameLen = 0;
		std::string _objName;
		Uint16 _interactionType, _tileTex;

		m_worldObjects.clear();

		for(Uint16 i = 0; i < _objCount; i++)
		{
			FileExt::readShort(_data, _index); // Size of object chunk, if you want to skip this stuff
			_objName = "";
			_objNameLen = FileExt::readChar(_data, _index);
			for(Uint16 j = 0; j < Uint16(_objNameLen); j++)
				_objName = _objName + char(FileExt::readChar(_data, _index));
			_interactionType = FileExt::readShort(_data, _index);
			_tileTex = FileExt::readShort(_data, _index);
			m_worldObjects.push_back(WorldObject(_objName, _interactionType, _tileTex));
			switch(m_worldObjects[i].m_interactionType)
			{
			case 0: // NONE

				break;
			case 1: // SOLID

				break;
			case 2: // SWITCH
				m_worldObjects[i].m_frequency = FileExt::readShort(_data, _index);
				break;
			case 3: // SOLID SWITCH
				m_worldObjects[i].m_frequency = FileExt::readShort(_data, _index);
				break;
			case 4: // PORTAL
				_objName = "";
				_objNameLen = FileExt::readChar(_data, _index);
				for(Uint16 j = 0; j < Uint16(_objNameLen); j++)
					_objName = _objName + char(FileExt::readChar(_data, _index));
				m_worldObjects[i].m_portalDest = _objName;
				m_worldObjects[i].m_destX = FileExt::readShort(_data, _index);
				m_worldObjects[i].m_destY = FileExt::readShort(_data, _index);
				break;
			}
		}


		while(_mapIndex < m_zoneSize.x * m_zoneSize.y && _index < _length)
		{
			_tileCount = FileExt::readShort(_data, _index);
			_tileId = FileExt::readShort(_data, _index);

			for(Uint16 i = 0; i < _tileCount; i++)
			{
				m_tileData[1][int(floor(GLfloat(_mapIndex) / m_zoneSize.x))][_mapIndex % m_zoneSize.x] = _tileId;
				_mapIndex++;
			}
		}
		delete[] _data;
	}
	_file.close();

	_file.open(std::string("zones\\" + p_zoneName + "\\Entity.tmf").c_str(), std::ios::binary);
	{
		_mapIndex = 0;
		_index = 0;

		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		_data = new char[_length];
		_file.read(_data, _length);


		Uint16 _objCount = FileExt::readShort(_data, _index);
		Uint16 _objNameLen = 0;
		std::string _objName;
		Uint16 _texNameLen = 0;
		std::string _texName;

		m_entities.clear();

		for(Uint16 i = 0; i < _objCount; i++)
		{
			FileExt::readShort(_data, _index); // Size of object chunk, if you want to skip this stuff
			_objName = "";
			_objNameLen = FileExt::readChar(_data, _index);
			for(Uint16 j = 0; j < Uint16(_objNameLen); j++)
				_objName = _objName + char(FileExt::readChar(_data, _index));
			_texName = "";
			_texNameLen = FileExt::readChar(_data, _index);
			for(Uint16 j = 0; j < Uint16(_texNameLen); j++)
				_texName = _texName + char(FileExt::readChar(_data, _index));
			m_entities.push_back(Entity(_objName, MTexture::getInstance().getUnit(LTexture::getInstance().loadImage(_texName))));
			m_entities[i].m_entityTexId = FileExt::readShort(_data, _index);
		}


		while(_mapIndex < m_zoneSize.x * m_zoneSize.y && _index < _length)
		{
			_tileCount = FileExt::readShort(_data, _index);
			_tileId = FileExt::readShort(_data, _index);

			for(Uint16 i = 0; i < _tileCount; i++)
			{
				m_tileData[2][int(floor(GLfloat(_mapIndex) / m_zoneSize.x))][_mapIndex % m_zoneSize.x] = _tileId;
				_mapIndex++;
			}
		}
		delete[] _data;
	}
	_file.close();

	for(Uint16 i = 1; i < m_entities.size(); i++)
	{
		_file.open(std::string("zones\\" + p_zoneName + "\\entities\\" + m_entities[i].m_name + ".script").c_str(), std::ios::binary);
		{
			_index = 0;
			_file.seekg(0, _file.end);
			_length = Uint32(_file.tellg());
			_file.seekg(0, _file.beg);
			_data = new char[_length];
			_file.read(_data, _length);
			for(Uint16 j = 0; j < _length; j++)
				m_entities[i].m_script += FileExt::readChar(_data, _index);
			delete[] _data;
		}
		_file.close();
	}

	_file.open(std::string("zones\\" + p_zoneName + "\\Sky.tmf").c_str(), std::ios::binary);
	{
		_mapIndex = 0;
		_index = 0;

		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		_data = new char[_length];
		_file.read(_data, _length);

		while(_mapIndex < m_zoneSize.x * m_zoneSize.y && _index < _length)
		{
			_tileCount = FileExt::readShort(_data, _index);
			_tileId = FileExt::readShort(_data, _index);

			for(Uint16 i = 0; i < _tileCount; i++)
			{
				m_tileData[3][int(floor(GLfloat(_mapIndex) / m_zoneSize.x))][_mapIndex % m_zoneSize.x] = _tileId;
				_mapIndex++;
			}
		}
		delete[] _data;
	}
	_file.close();

	std::cout << "Complete." << std::endl;

	return true;
}

void ZoneMap::createNew()
{
	std::cout << "Clearing map... ";
	init();
	std::cout << "Complete." << std::endl;
}

void ZoneMap::clear()
{
	if(m_initialized)
	{
		for(Uint16 i = 0; i < 4; i++)
		{
			for(Uint16 x = 0; x < m_zoneSize.x; x++)
			{
				delete[] m_tileData[i][x];
			}
			delete[] m_tileData[i];
		}
		m_initialized = false;
		m_zoneSize = {0, 0};
	}
}
