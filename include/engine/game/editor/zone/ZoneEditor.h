#pragma once

#include "..\Editor.h"

static class ZoneEditor : public Singleton<ZoneEditor>, public Editor
{
public:
	void init();

	void input();
	void update();
protected:
	Container* m_guiWorldSwitch;
	Container* m_guiWorldSolidSwitch;
	Container* m_guiWorldPortal;
	Container* m_guiWorldDirection;
};
