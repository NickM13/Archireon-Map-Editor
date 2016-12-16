#pragma once

#include "..\Editor.h"
#include "engine\game\map\board\BoardMap.h"

class BoardEditor : public Singleton<BoardEditor>, public Editor
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
	BoardMap* m_map;

	Container* m_guiWorldSlow;
	Container* m_guiWorldTrap;
	Container* m_guiWorldSwitch;
	Container* m_guiWorldSolidSwitch;
	Container* m_guiWorldPortal;
	Container* m_guiWorldDirection;
};
