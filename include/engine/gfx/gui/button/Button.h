#pragma once

#include "engine\utils\Utilities.h"
#include "..\base\Component.h"
#include "..\..\font\Font.h"

class CButton : public Component
{
private:

	Sint32 m_fontSize;

	Texture m_buttonTex;

	bool m_hover;
	bool m_stuck;

	typedef void (*function)();
	function m_function;
public:
	CButton(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32 p_fontSize, Sint8 p_colorTheme, function p_function = 0);
	CButton(std::string p_compName, std::string p_title, Texture p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32 p_fontSize, Sint8 p_colorTheme, function p_function = 0);
	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();

	Sint8 isSelected(); // 0 = not selected, 1 = JUST pressed, 2 = is held, 3 = JUST released
};

class CButtonToggle : public Component
{
private:
	Sint32 m_fontSize;
	Texture m_buttonTex[2];
	Sint8 m_texType;

	bool m_hover;
public:
	CButtonToggle(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32 p_fontSize, Sint8 p_colorTheme, Sint8 p_state = 0);
	CButtonToggle(std::string p_compName, Texture p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32 p_fontSize, Sint8 p_colorTheme, Sint8 p_state = 0);
	CButtonToggle(std::string p_compName, Texture p_activeTex, Texture p_inactiveTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32 p_fontSize, Sint8 p_colorTheme, Sint8 p_state = 0);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();
};


class CButtonRadio : public Component
{
private:
	Uint16 m_selectedButton, m_prevSelectedButton;

	std::vector<CButtonToggle*> m_buttonList;
public:
	CButtonRadio(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_colorTheme);

	void addButton(CButtonToggle* p_button);

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void update(GLfloat p_deltaUpdate);
	void render();

	void setSelectedButton(Uint16 p_selectedButton);
	Uint16 getSelectedButton();
	Uint16 getPrevSelectedButton();
};
