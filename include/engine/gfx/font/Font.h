#pragma once

#include "..\LTexture.h"
#include "engine\utils\Utilities.h"
#include "engine\utils\Singleton.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

enum Alignment
{
	ALIGN_LEFT = 0,
	ALIGN_CENTER = 1,
	ALIGN_RIGHT = 2
};

class Font : public Singleton <Font>
{
public:
	void setAlignment(Alignment p_alignment);

	void init(std::string p_src, Uint32 h);
	void clean();

	Sint16 getHeight() {return m_height;};
	Vector2<Sint32> getMessageWidth(std::string p_msg);
	GLfloat getSpacingHeight() {return m_height * 1.8f;};	/*Value with height multiplied*/
	GLfloat getSpacing() {return 1.8f;};					/*Value without height multiplied*/

	void print(std::string p_msg, Sint32 p_x, Sint32 p_y);
private:
	Sint16 m_height;
	GLuint* m_textures;
	GLuint* m_charWidth;
	GLuint m_listBase;

	Alignment m_alignment;
	GLfloat m_spacing;
};
