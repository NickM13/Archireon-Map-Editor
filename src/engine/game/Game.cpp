#include "engine\game\Game.h"

#include "engine\gfx\font\Font.h"

#include <functional>
#include <iostream>

bool Game::init()
{
	m_editorState = ZONE;

	Font::getInstance().init("res\\segoeui.ttf", 10);

	m_boardEditor = &BoardEditor::getInstance();
	m_boardEditor->init();
	
	m_zoneEditor = &ZoneEditor::getInstance();
	m_zoneEditor->init();

	return true;
}

void Game::resize()
{

}

void Game::setEditorState(EditorState p_state)
{
	if(m_editorState != p_state)
	{
		m_editorState = p_state;
		switch(m_editorState)
		{
		case BOARD:
			setFadeMessage("Editor set to Board", 1);
			break;
		case ZONE:
			setFadeMessage("Editor set to Zone", 1);
			break;
		}
	}
}

void Game::setFadeMessage(std::string p_msg, GLfloat p_time)
{
	m_fadeMsg = p_msg;
	m_fadeTimer = p_time;
}

void Game::input()
{
	Vector2<Sint32> _mousePos = Globals::getInstance().m_mousePos;
	Sint8 _rValue = 0; // input flags: 1 = mouse click, 2 = key press, 4 = mouse scroll

	switch(m_editorState)
	{
	case BOARD:
		m_boardEditor->input();
		if(m_boardEditor->switchEditor())
			setEditorState(EditorState(ZONE));
		break;
	case ZONE:
		m_zoneEditor->input();
		if(m_zoneEditor->switchEditor())
			setEditorState(EditorState(BOARD));
		break;
	}
}

void Game::update()
{
	m_deltaUpdate = GLfloat(glfwGetTime() - m_lastUpdate);
	m_lastUpdate = GLfloat(glfwGetTime());

	switch(m_editorState)
	{
	case BOARD:
		m_boardEditor->update();
		break;
	case ZONE:
		m_zoneEditor->update();
		break;
	}

	m_fadeTimer -= m_deltaUpdate;
}

void Game::render()
{
	switch(m_editorState)
	{
	case BOARD:
		m_boardEditor->render();
		break;
	case ZONE:
		m_zoneEditor->render();
		break;
	}

	glColor4f(1, 1, 1, m_fadeTimer);

	Font::getInstance().setAlignment(ALIGN_CENTER);
	Font::getInstance().print(m_fadeMsg, 100, 0);
}
