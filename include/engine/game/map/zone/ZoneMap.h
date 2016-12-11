#pragma once

#pragma warning( disable : 6031 )

#include "..\Map.h"

class ZoneMap : public Map
{
public:
	ZoneMap();
	ZoneMap(Vector2<Uint16> p_zoneSize);
	~ZoneMap();

	void save();
	void save(std::string p_zoneName);

	bool load();
	bool load(std::string p_zoneName);
private:

};
