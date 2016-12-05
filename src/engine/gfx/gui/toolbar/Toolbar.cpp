#include "engine\gfx\gui\toolbar\Toolbar.h"


CToolbar::CToolbar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_colorTheme)
{
	m_compName = p_compName;
	m_pos = p_pos;
	m_size = p_size;
	m_colorTheme = m_colorThemes[p_colorTheme];
	m_panelMain = new Panel("", "", {0, 0}, p_size, 0, true, LTexture::getInstance().loadImage("gui\\BarUnderline.png"), COMPONENT_TEXTURE_STYLE_SCALE);
	m_panelSub = new Panel("", "", {0, 0}, {0, 0}, 0, true, LTexture::getInstance().loadImage("gui\\BarDrop.png"), COMPONENT_TEXTURE_STYLE_SCALE);

	m_slideCounter = 0;
}

//Directory splits with '/' or '\\' 
void CToolbar::addButton(std::string p_dir, std::string p_buttonName, function p_func)
{
	Uint16 i = 0, j = 0;
	std::vector<std::string> _splitDir;
	if(p_dir != "")
	{
		while(i < p_dir.length())
		{
			if(p_dir[i] == '/' || p_dir[i] == '\\')
			{
				_splitDir.push_back(p_dir.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if(p_dir[p_dir.length() - 1] != '/' && p_dir[p_dir.length() - 1] != '\\')
			_splitDir.push_back(p_dir.substr(j, i));
	}

	SubList* _subList = &m_buttonsMain;
	for(i = 0; i < _splitDir.size(); i++)
		_subList = _subList->find(_splitDir[i]);
	_subList->addButton(p_buttonName, p_func);
}

void CToolbar::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	Uint16 i = 0, j = 0;
	if((p_interactFlags & 1) == 0 && p_mouseStates[0] == 1)
	{
		m_slideCounter = 0;
		SubList* _subList = &m_buttonsMain;
		std::vector<std::string> _splitDir;
		if(m_selected != "")
		{
			while(i < m_selected.length())
			{
				if(m_selected[i] == '/' || m_selected[i] == '\\')
				{
					_splitDir.push_back(m_selected.substr(j, i));
					j = i + 1;
				}
				i++;
			}
			if(m_selected[m_selected.length() - 1] != '/' && m_selected[m_selected.length() - 1] != '\\')
				_splitDir.push_back(m_selected.substr(j, i));
		}

		for(Uint16 k = 0; k < _splitDir.size(); k++)
			_subList = _subList->find(_splitDir[k]);

		if(_splitDir.size() == 1 && m_currDir == m_selected)
			m_currDir = "";
		else if(_subList->getFunction() != 0)
		{
			m_currDir = "";
			_subList->callFunction();
			p_interactFlags += 1;
		}
		else
			m_currDir = m_selected;
	}
	m_selected = "";
	if((p_interactFlags & 1) == 0)
	{
		i = j = 0;
		std::vector<std::string> _splitDir;
		if(m_currDir != "")
		{
			while(i < m_currDir.length())
			{
				if(m_currDir[i] == '/' || m_currDir[i] == '\\')
				{
					_splitDir.push_back(m_currDir.substr(j, i));
					j = i + 1;
				}
				i++;
			}
			if(m_currDir[m_currDir.length() - 1] != '/' && m_currDir[m_currDir.length() - 1] != '\\')
				_splitDir.push_back(m_currDir.substr(j, i));
		}

		Sint16 w = 0;
		for(i = 0; i < m_buttonsMain.m_buttons.size(); i++)
		{
			if(m_currDir != "" && _splitDir[0] == m_buttonsMain.m_buttons[i].m_name)
			{
				std::vector<std::string> _path;
				SubList* _subList = &m_buttonsMain;
				for(j = 0; j < _splitDir.size(); j++)
				{
					_path.push_back(_splitDir[j]);
					_subList = _subList->find(_splitDir[j]);
					for(Uint16 g = 0; g < _subList->m_buttons.size(); g++)
					{
						if(p_mousePos.x - w >= 0 && p_mousePos.x - w < _subList->m_width * 16 + 48 &&
							p_mousePos.y - m_size.y >= g * 16 && p_mousePos.y - m_size.y < g * 16 + 16)
						{
							m_selected = "";
							for(Uint16 h = 0; h < _path.size(); h++)
								m_selected += (_path[h] + "\\");
							m_selected += _subList->m_buttons[g].m_name;
							p_interactFlags += 1;
							break;
						}	
					}
				}
			}
			if(p_mousePos.x - w >= 0 && p_mousePos.x - w < Sint32(m_buttonsMain.m_buttons[i].m_name.length() * 16 + 32) &&
				p_mousePos.y >= 0 && p_mousePos.y < m_size.y)
			{
				m_selected = m_buttonsMain.m_buttons[i].m_name;
				p_interactFlags += 1;
				break;
			}
			w += Sint16(m_buttonsMain.m_buttons[i].m_name.length() * 16 + 32);
		}
	}
	if((p_interactFlags & 1) == 0 &&
		p_mousePos.x > m_pos.x && p_mousePos.x < m_pos.x + m_size.x &&
		p_mousePos.y > m_pos.y && p_mousePos.y < m_pos.y + m_size.y)
		p_interactFlags += 1;
}
void CToolbar::update(GLfloat p_deltaUpdate)
{
	m_slideCounter += p_deltaUpdate * 16;
	if(m_slideCounter > 1)
		m_slideCounter = 1;
}
void CToolbar::render()
{
	std::vector<std::string> _splitDir, _splitSelect;
	if(m_currDir != "")
	{
		Uint16 i = 0, j = 0;
		while(i < m_currDir.length())
		{
			if(m_currDir[i] == '/' || m_currDir[i] == '\\')
			{
				_splitDir.push_back(m_currDir.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if(m_currDir[m_currDir.length() - 1] != '/' && m_currDir[m_currDir.length() - 1] != '\\')
			_splitDir.push_back(m_currDir.substr(j, i));
	}
	else
		_splitDir.push_back("");

	if(m_selected != "")
	{
		Uint16 i = 0, j = 0;
		while(i < m_selected.length())
		{
			if(m_selected[i] == '/' || m_selected[i] == '\\')
			{
				_splitSelect.push_back(m_selected.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if(m_selected[m_selected.length() - 1] != '/' && m_selected[m_selected.length() - 1] != '\\')
			_splitSelect.push_back(m_selected.substr(j, i));
	}
	else
		_splitSelect.push_back("");

	m_panelMain->renderFill();
	m_colorTheme.m_text.useColor();
	glPushMatrix();
	{
		Font::getInstance().setFontSize(16);
		Font::getInstance().setAlignment(ALIGN_LEFT);
		for(Uint16 i = 0; i < m_buttonsMain.m_buttons.size(); i++)
		{
			if(_splitDir[0] == m_buttonsMain.m_buttons[i].m_name || m_selected == m_buttonsMain.m_buttons[i].m_name)
			{
				glColor4f(0, 0, 0, 0.25f);
				glBegin(GL_QUADS);
				{
					glVertex2f(0, 0);
					glVertex2f(GLfloat(m_buttonsMain.m_buttons[i].m_name.length() * 16 + 32), 0);
					glVertex2f(GLfloat(m_buttonsMain.m_buttons[i].m_name.length() * 16 + 32), GLfloat(m_size.y));
					glVertex2f(0, GLfloat(m_size.y));
				}
				glEnd();
			}
			m_colorTheme.m_text.useColor();
			Font::getInstance().print(m_buttonsMain.m_buttons[i].m_name, 16, (m_size.y - 16) / 2);

			if(_splitDir[0] == m_buttonsMain.m_buttons[i].m_name)
			{
				SubList* _subList = &m_buttonsMain;
				for(Uint16 j = 0; j < _splitDir.size(); j++)
				{
					_subList = _subList->find(_splitDir[j]);
					if(_subList->m_buttons.size() > 0)
					{
						glPushMatrix();
						{
							glTranslatef(GLfloat(j * 92), GLfloat(m_size.y), 0);
							m_panelSub->setSize(Vector2<Sint32>(_subList->m_width * 16 + 48, _subList->m_buttons.size() * 16));
							if(j == _splitDir.size() - 1)
								glScalef(1, m_slideCounter, 1);
							m_panelSub->renderFill();

							m_colorTheme.m_text.useColor();
							for(Uint16 k = 0; k < _subList->m_buttons.size(); k++)
							{
								if((Uint16(_splitDir.size()) > j + 1 && _splitDir[j + 1] == _subList->m_buttons[k].m_name) || (Uint16(_splitSelect.size()) > j + 1 && _splitSelect[j + 1] == _subList->m_buttons[k].m_name))
								{
									glColor4f(0, 0, 0, 0.25f);
									glBegin(GL_QUADS);
									{
										glVertex2f(0, GLfloat(k * 16));
										glVertex2f(GLfloat(_subList->m_width * 16 + 48), GLfloat(k * 16));
										glVertex2f(GLfloat(_subList->m_width * 16 + 48), GLfloat(k * 16 + 16));
										glVertex2f(0, GLfloat(k * 16 + 16));
									}
									glEnd();
									m_colorTheme.m_text.useColor();
								}
								Font::getInstance().print(_subList->m_buttons[k].m_name, 0, k * 16);
							}
						}
						glPopMatrix();
					}
				}
			}
			glTranslatef(GLfloat(m_buttonsMain.m_buttons[i].m_name.length() * 16) + 32, 0, 0);
		}
	}
	glPopMatrix();
}