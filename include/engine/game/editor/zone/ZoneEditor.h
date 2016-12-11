#pragma once

#include "..\Editor.h"

static class ZoneEditor : public Singleton<ZoneEditor>, public Editor
{
public:
	void init();
};
