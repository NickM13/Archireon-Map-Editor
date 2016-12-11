#pragma once

#include "..\Map.h"

class BoardMap : public Map
{
public:
	BoardMap();
	BoardMap(Vector2<Uint16> p_boardSize);
	~BoardMap();

	void save();
	void save(std::string p_zoneName);

	bool load();
	bool load(std::string p_zoneName);
private:

};
