#pragma once

#include "..\Editor.h"

class BoardEditor : public Singleton<BoardEditor>, public Editor
{
public:
	void init();
};
