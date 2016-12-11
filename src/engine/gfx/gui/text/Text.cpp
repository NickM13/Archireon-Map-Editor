#include "engine\gfx\gui\text\Text.h"


CText::CText(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Alignment p_align, Color p_fontColor)
{
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_size = p_size;
	m_align = p_align;
	m_color = p_fontColor;

	m_selected = 0;

	splitTitle();
}

void CText::setTitle(std::string p_title)
{
	m_title = p_title;
	splitTitle();
}
void CText::splitTitle()
{
	m_text.clear();
	Uint16 _i = 0;
	for(Uint16 i = 0; i < m_title.length() - 1; i++)
	{
		if(m_title[i] == '\n')
		{
			m_text.push_back(m_title.substr(_i, i - _i));
			_i = i + 1;
		}
	}
	m_text.push_back(m_title.substr(_i, m_title.length() - _i));
}

void CText::render()
{
	m_color.useColor();
	Font::getInstance().setAlignment(m_align);
	Font::getInstance().print(m_title, m_pos.x, m_pos.y);
}
