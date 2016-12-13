#pragma once

#include "..\Editor.h"

class BoardEditor : public Singleton<BoardEditor>, public Editor
{
public:
	void init();
protected:
	Container* m_guiWorldSlow;
	Container* m_guiWorldTrap;
	Container* m_guiWorldSwitch;
	Container* m_guiWorldSolidSwitch;
	Container* m_guiWorldPortal;
};
