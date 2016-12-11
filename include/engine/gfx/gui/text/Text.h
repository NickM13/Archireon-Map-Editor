#pragma once

#include "engine\utils\Utilities.h"
#include "..\base\Component.h"
#include "..\..\font\Font.h"

struct CText : public Component
{
private:
	std::vector<std::string> m_text;
	Vector2<Sint32> m_pos;
	Alignment m_align;
	Color m_color;
public:
	CText(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Alignment p_align, Color p_fontColor);

	void setTitle(std::string p_title);
	void splitTitle();

	void render();
};
