#pragma once

#include "..\Editor.h"
#include "engine\game\map\zone\ZoneMap.h"

static class ZoneEditor : public Singleton<ZoneEditor>, public Editor
{
public:
	void init();
	std::string getZoneName();

	void input();
	void update();
	void render();

	void autosave();
	void refresh();
protected:
	ZoneMap* m_map;

	// Window that is opened
	Container* m_guiEntityBoard;
	Container* m_guiEntityInventory;

	//Extension containers
	Container* m_guiWorldSwitch;
	Container* m_guiWorldSolidSwitch;
	Container* m_guiWorldPortal;
	Container* m_guiWorldDirection;
	Container* m_guiWorldLight;

	Container* m_guiEntityNeutral;
	Container* m_guiEntityOpponent;
	Container* m_guiEntityTrader;
	Container* m_guiEntityLootBag;
};
