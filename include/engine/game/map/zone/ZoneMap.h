#pragma once

#pragma warning( disable : 6031 )

#include "..\Map.h"

class ZoneMap : public Map
{
public:
	struct Entity
	{
		std::string m_name;
		Vector2<Uint16> m_pos;
		Texture m_entityTex;
		Uint16 m_entityTexId;
		Uint8 m_entityType; // Neutral=0, Opponent, Trader
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

	ZoneMap();
	ZoneMap(Vector2<Uint16> p_zoneSize);
	~ZoneMap();

	Uint16 addEntity(Entity p_entity);
	Entity& getEntity(Uint16 p_index);
	Uint16 getEntitySize();
	void removeEntity(Uint16 p_index);
	void setEntity(Uint16 p_index, Vector2<Sint32> p_pos);

	void save();
	void save(std::string p_zoneName);

	bool load();
	bool load(std::string p_zoneName);
private:
	std::vector<Entity> m_entities;
};
