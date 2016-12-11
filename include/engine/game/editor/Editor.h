#pragma once

#define TILE_SIZE 32

#include "engine\utils\Globals.h"

#include "engine\game\map\Map.h"

#include "..\..\gfx\gui\container\Container.h"
#include "..\..\gfx\gui\container\ContainerPanel.h"
#include "..\..\gfx\gui\counter\Counter.h"
#include "..\..\gfx\gui\button\Button.h"
#include "..\..\gfx\gui\dropdown\DropDown.h"
#include "..\..\gfx\gui\list\List.h"
#include "..\..\gfx\gui\panel\Panel.h"
#include "..\..\gfx\gui\slider\Slider.h"
#include "..\..\gfx\gui\tabbar\TabBar.h"
#include "..\..\gfx\gui\text\Text.h"
#include "..\..\gfx\gui\text\TextField.h"
#include "..\..\gfx\gui\tileset\Tileset.h"
#include "..\..\gfx\gui\toolbar\Toolbar.h"

class Editor
{
public:
	virtual void init();
	virtual void resize();

	bool switchEditor();
	
	std::string getZoneName();
	void unpause();
	void pause(std::string p_screen);
	std::string getPause();

	virtual void input();
	virtual void update();
	virtual void render();
protected:
	Map* m_map;

	bool m_showGrid;

	CToolbar* m_toolbarMenu;
	Container* m_guiAll, *m_guiTop, *m_guiLeft;
	Container* m_guiLeftLayer, *m_guiLeftDetail;
	ContainerPanel* m_guiGround, *m_guiWorld, *m_guiEntity, *m_guiSky, *m_guiStamp;
	Container* m_guiWorldSwitch, *m_guiWorldPortal;
	CTileSet* m_tileSetGround, *m_tileSetWorld, *m_tileSetEntity, *m_tileSetSky, *m_tileSetStamps;
	CList* m_listWorld, *m_listStamps;
	CTabBar* m_tabWorld, *m_tabEntity, *m_tabStamps;

	//Pause screens
	Container* m_guiPause;
	Sint16 m_cPauseScreen;
	std::vector<Container*> m_pauseScreens;
	Container* m_guiSaveMap, *m_guiLoadMap, *m_guiClearMap, *m_guiResizeMap;
	Container* m_guiEntityTex, *m_guiEntityInteract, *m_guiEntityIdle;
	CButtonRadio* m_selectLayer;
	Container* m_guiExit;
	
	struct Stamp
	{
		Vector2<Sint32> m_size;
		Uint16 **m_ground, **m_world, **m_entity, **m_sky;

		Stamp(Vector2<Sint32> p_size = {}, Uint16** p_ground = 0, Uint16** p_world = 0, Uint16** p_entity = 0, Uint16** p_sky = 0) :
			m_size(p_size),
			m_ground(p_ground),
			m_world(p_world),
			m_entity(p_entity),
			m_sky(p_sky) {}
	};

	std::vector<Stamp> m_stamps;

	Vector2<GLfloat> m_camPos;
	GLfloat m_zoom, m_tZoom;

	Vector2<Sint32> m_mouseBuffer;
	bool m_mouseInArea;
	bool m_lmbDown, m_rmbDown;

	Rect m_tileMapArea;
	Texture m_backTexId;

	Vector2<Sint32> m_selectStart, m_selectEnd;

	GLfloat m_lastUpdate, m_deltaUpdate;
};
