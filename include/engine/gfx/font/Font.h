#pragma once

#include "..\LTexture.h"
#include "engine\utils\Utilities.h"
#include "engine\utils\Singleton.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

enum Alignment
{
	ALIGN_LEFT = 0,
	ALIGN_CENTER = 1,
	ALIGN_RIGHT = 2
};

class Font : public Singleton<Font>
{
public:
	Font();
	~Font();

	bool init();
	bool loadFreeType(std::string p_src, GLuint p_size);

	void setAlignment(Alignment p_alignment);

	void setFont(std::string p_src);
	void setFontId(GLuint p_id);

	void setFontSize(Sint32 p_fontSize);

	GLuint getFontId();

	void print(std::string p_msg, Sint32 p_x, Sint32 p_y);

	GLuint m_vao, m_vbo;
private:
	FT_Library m_ftLib;
	FT_Face m_face;
	struct Character
	{
		GLuint m_textureID;
		Vector2<Sint32> m_size;
		Vector2<Sint32> m_bearing;
		GLuint m_advance;
	};
	std::map<GLchar, Character> m_characters;


	Alignment m_alignment;
	Sint32 m_fontSize;
};
