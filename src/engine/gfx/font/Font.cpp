#include "engine\gfx\font\Font.h"
#include "engine\utils\Utilities.h"

Font::Font()
{
	
}

Font::~Font()
{

}

bool Font::init()
{
#ifndef __FREEGLUT_H__ 
	FT_Error _error = FT_Init_FreeType(&m_ftLib);
	if(_error)
	{
		printf("FreeType error:%X", _error);
		return false;
	}
#endif
	return true;
}
bool Font::loadFreeType(std::string p_src, GLuint p_size)
{
	FT_Error _error = 0;
#ifdef __FREEGLUT_H__
	_error = FT_Init_FreeType(&m_ftLib);
	if(error)
	{
		printf("FreeType error:%X", error);
		return false;
	}
#endif
	GLuint cellW = 0;
	GLuint cellH = 0;
	int maxBearing = 0;
	int minHang = 0;

	Texture bitmaps[256];
	FT_Glyph_Metrics metrics[256];

	m_face = 0;
	_error = FT_New_Face(m_ftLib, p_src.c_str(), 0, &m_face);
	if(!_error)
	{
		for(int i = 0; i < 256; i++)
		{
			_error = FT_Set_Pixel_Sizes(m_face, 0, p_size);
			if(!_error)
			{
				metrics[i] = m_face->glyph->metrics;

				bitmaps[i].copyPixels8(m_face->glyph->bitmap.buffer, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows);

				if(metrics[i].horiBearingX / 64 > maxBearing)
				{
					maxBearing = metrics[i].horiBearingY / 64;
				}

				if(metrics[i].width / 64 > cellW)
				{
					cellW = metrics[i].width / 64;
				}

				int glyphHang = (metrics[i].horiBearingY - metrics[i].height) / 64;
				if(glyphHang < minHang)
				{
					minHang = glyphHang;
				}
			}
			else
			{
				printf("Unable to load glyph, FreeType error:%X\n", _error);
				_error = 0;
			}
		}
		cellH = maxBearing - minHang;
		createPixels8(cellW * 16, cellH * 16);
		GLuint currentChar = 0;
		Rect nextClip = {0, 0, cellW, cellH};

		int bX = 0;
		int bY = 0;

		for(Uint16 r = 0; r < 16; r++)
		{
			for(Uint16 c = 0; c < 16; c++)
			{
				bX = cellW * c;
				bY = cellH * r;

				nextClip.x = bX;
				nextClip.y = bY;
				nextClip.w = metrics[currentChar].width / 64;
				nextClip.h = cellH;
				bitmaps[currentChar].blitPixels8(bX, bY + maxBearing - metrics[currentChar].horiBearingY / 64, *this);

				mClips.push_back(nextClip);
				currentChar++;
			}
		}
		padPixels8();

		if(loadTextureFromPixels8())
		{
			if(!generateDataBuffer(LSPRITE_ORIGIN_TOP_LEFT))
			{
				printf("Unable to create vertex buffer for bitmap font!");
				_error = 0xA2;
			}
			glBindTexture(GL_TEXTURE_2D, getTextureID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			mSpace = cellW / 2;
			mNewLine = maxBearing;
			mLineHeight = cellH;
		}
		else
		{
			printf("Unable to set font size. FreeType error:%X\n", _error);
		}

		FT_Done_Face(m_face);
	}
	else
	{
		printf("Unable to load load font face. FreeType error:%X\n", _error);
		return false;
	}

#ifdef __FREEGLUT_H__
	FT_Done_FreeType(m_ftLib);
#endif
	
	return (_error == 0);
}

void Font::setAlignment(Alignment ALIGN)
{
	m_alignment = ALIGN;
}

void Font::setFont(std::string src)
{
}

void Font::setFontId(GLuint id)
{
}

void Font::setFontSize(Sint32 p_fontSize)
{
	m_fontSize = p_fontSize;
}

GLuint Font::getFontId()
{
	return 0;
}

// TODO: Add wrapping text
void Font::print(std::string message, Sint32 x, Sint32 y)
{
	/*
	FT_GlyphSlot _slot = _face->glyph;
	FT_UInt _glyphIndex;
	// Offset values for wrapping
	GLfloat ox, oy;
	ox = oy = 0;

	switch(m_alignment)
	{
	case ALIGN_LEFT:
		ox = 0;
		break;
	case ALIGN_CENTER:
		ox = GLfloat(GLfloat(-m_fontSize * Sint32(message.length())) / 2.f);
		break;
	case ALIGN_RIGHT:
		ox = GLfloat(GLfloat(-m_fontSize * Sint32(message.length())));
		break;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Texture coordinates
	GLfloat tx, ty;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1, 1, 1, 1);

	glPushMatrix();
	{
		glTranslatef(GLfloat(x), GLfloat(y), 0);

		for(Uint16 i = 0; i < message.length(); i++)
		{
			if(FT_Load_Char(_face, message[i], FT_LOAD_RENDER))
				continue;

			glBindTexture(GL_TEXTURE_2D, m_characters[i].m_textureID);

			glPushMatrix();
			{
				glTranslatef(ox + _slot->bitmap_left, _slot->bitmap_top, 0);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0, 0);	glVertex2f(0, 0);
					glTexCoord2f(1, 0);	glVertex2f(_slot->bitmap.width, 0);
					glTexCoord2f(1, 1);	glVertex2f(_slot->bitmap.width, _slot->bitmap.rows);
					glTexCoord2f(0, 1);	glVertex2f(0, _slot->bitmap.rows);
				}
				glEnd();
			}
			glPopMatrix();

			ox += _slot->advance.x >> 6;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
	*/
}
