#pragma once

#include "engine\utils\Utilities.h"
#include "..\base\Component.h"
#include "..\..\font\Font.h"

class CTileSet : public Component
{
private:
	Vector2<Sint16> m_selectedTile;
	Uint16 m_tileSize;

	Texture m_tileSheet;
	Texture m_transparentTex, m_selectTex;
	Vector2<Uint16> m_tileCount;
	Vector2<Sint16> m_scroll;

	Vector2<Uint16> m_mouseBuffer;

	bool m_hover;
	bool m_dragging;
public:
	CTileSet(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_tileSize, Texture p_tileSheet, Sint8 p_colorTheme = 0)
	{
		m_compName = p_compName;
		m_title = p_title;
		m_pos = p_pos;
		m_size = p_size;
		m_tileSize = p_tileSize;
		m_tileSheet = p_tileSheet;
		m_colorTheme = m_colorThemes[p_colorTheme];

		m_transparentTex = MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("gui\\Transparent.png"));
		m_selectTex = MTexture::getInstance().getUnit(LTexture::getInstance().loadImage("gui\\TileSelect.png"));

		m_tileCount = m_tileSheet.getSize() / m_tileSize;

		m_borderThickness = 5;
	}
	void setTileSheet(Texture p_tileSheet)
	{
		m_tileSheet = p_tileSheet;
		m_tileCount = m_tileSheet.getSize() / m_tileSize;
	}
	Vector2<Sint32> getRealPosition()
	{
		return Vector2<Sint32>(m_pos - Vector2<Sint32>(m_borderThickness, 20 + m_borderThickness));
	}
	Vector2<Sint32> getRealSize()
	{
		return Vector2<Sint32>(m_size + Vector2<Sint32>(m_borderThickness * 2, 20 + m_borderThickness * 2));
	}
	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
	{
		if(p_mousePos.x >= m_pos.x - (m_borderThickness - 1) && p_mousePos.x <= m_pos.x + m_size.x + (m_borderThickness - 1) &&
			p_mousePos.y >= m_pos.y - 20 - (m_borderThickness - 1) && p_mousePos.y <= m_pos.y + m_size.y + (m_borderThickness - 1))
			m_hover = true;
		else
			m_hover = false;
		if((((p_interactFlags & 1) == 0) || m_dragging) && 
			p_mousePos.x >= m_pos.x && p_mousePos.x <= m_pos.x + m_size.x &&
			p_mousePos.y >= m_pos.y && p_mousePos.y <= m_pos.y + m_size.y)
		{
			if(p_mouseStates[0] == 1)
			{
				if(Sint16(GLfloat(p_mousePos.x - m_pos.x + floor(GLfloat(m_scroll.x) / m_tileSize) * m_tileSize) < m_tileSheet.getSize().x) &&
					Sint16(GLfloat(p_mousePos.y - m_pos.y + floor(GLfloat(m_scroll.y) / m_tileSize) * m_tileSize) < m_tileSheet.getSize().y))
					m_selectedTile = {Sint16(GLfloat(p_mousePos.x - m_pos.x + floor(GLfloat(m_scroll.x) / m_tileSize) * m_tileSize) / m_tileSize),
									  Sint16(GLfloat(p_mousePos.y - m_pos.y + floor(GLfloat(m_scroll.y) / m_tileSize) * m_tileSize) / m_tileSize)};

				m_dragging = false;
				if((p_interactFlags & 1) == 0)
					p_interactFlags += 1;

			}
			else if(p_mouseStates[1] == 1 || p_mouseStates[1] == 2 && m_dragging)
			{
				m_dragging = true;
				if((p_interactFlags & 1) == 0)
					p_interactFlags += 1;
			}
		}

		if(m_dragging)
		{
			if(p_mouseStates[1] == 0 || p_mouseStates[0] == 1)
				m_dragging = false;
			else
			{
				if(m_tileSheet.getSize().x > m_size.x)
					m_scroll.x -= (p_mousePos.x - m_mouseBuffer.x);
				if(m_tileSheet.getSize().y > m_size.y)
					m_scroll.y -= (p_mousePos.y - m_mouseBuffer.y);
				if(m_scroll.x <= 0)
					m_scroll.x = 0;
				else if(m_scroll.x > m_tileSheet.getSize().x - m_size.x + m_tileSize - 1)
					m_scroll.x = m_tileSheet.getSize().x - m_size.x + m_tileSize - 1;
				if(m_scroll.y <= 0)
					m_scroll.y = 0;
				else if(m_scroll.y > m_tileSheet.getSize().y - m_size.y + m_tileSize - 1)
					m_scroll.y = m_tileSheet.getSize().y - m_size.y + m_tileSize - 1;
				m_mouseBuffer = p_mousePos;
			}
		}

		m_mouseBuffer = p_mousePos;
	}
	void update(GLfloat p_deltaUpdate)
	{

	}
	void render()
	{
		glPushMatrix();
		{
			glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
			Math::pushScissor(Rect(-GLfloat(m_borderThickness), -GLfloat(m_borderThickness + 20), GLfloat(m_size.x + m_borderThickness * 2), GLfloat(m_size.y + m_borderThickness * 2 + 20)));

			m_colorTheme.m_back.useColor();
			glBegin(GL_QUADS);
			{
				glVertex2f(-GLfloat(m_borderThickness), -GLfloat(m_borderThickness) - 20);
				glVertex2f(GLfloat(m_size.x + m_borderThickness), -GLfloat(m_borderThickness) - 20);
				glVertex2f(GLfloat(m_size.x + m_borderThickness), GLfloat(m_size.y + m_borderThickness));
				glVertex2f(-GLfloat(m_borderThickness), GLfloat(m_size.y + m_borderThickness));
			}
			glEnd();

			m_colorTheme.m_fore.useColor();
			glBegin(GL_QUADS);
			{
				glVertex2f(-GLfloat(m_borderThickness - 1), GLfloat(m_size.y + (m_borderThickness - 1)));
				glVertex2f(GLfloat(m_size.x + (m_borderThickness - 1)), GLfloat(m_size.y + (m_borderThickness - 1)));
				glVertex2f(GLfloat(m_size.x + (m_borderThickness - 1)), -20 - GLfloat(m_borderThickness - 1));
				glVertex2f(-GLfloat(m_borderThickness - 1), -20 - GLfloat(m_borderThickness - 1));
			}
			glEnd();

			m_colorTheme.m_text.useColor();
			Font::getInstance().setFontSize(16);
			Font::getInstance().setAlignment(ALIGN_CENTER);
			Font::getInstance().print(m_title, m_size.x / 2, -20);

			glColor3f(1, 1, 1);

			Math::popScissor();

			Math::pushScissor(Rect(0, 0, GLfloat(m_size.x), GLfloat(m_size.y)));

			glBindTexture(GL_TEXTURE_2D, m_transparentTex.getId());
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex2f(0, 0);
				glTexCoord2f(GLfloat(ceil(GLfloat(m_size.x) / m_tileSize)), 0);
				glVertex2f(GLfloat(m_size.x), 0);
				glTexCoord2f(GLfloat(ceil(GLfloat(m_size.x) / m_tileSize)), GLfloat(ceil(GLfloat(m_size.y) / m_tileSize)));
				glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
				glTexCoord2f(0, GLfloat(ceil(GLfloat(m_size.y) / m_tileSize)));
				glVertex2f(0, GLfloat(m_size.y));
			}
			glEnd();

			glTranslatef(-GLfloat(m_scroll.x % m_tileSize), -GLfloat(m_scroll.y % m_tileSize), 0);

			glBindTexture(GL_TEXTURE_2D, m_tileSheet.getId());
			glBegin(GL_QUADS);
			{
				Vector2<GLfloat> m_tileRatio = m_tileSheet.getSize() / m_tileSize;
				for(Uint16 x = 0; x < Uint16(min(ceil(GLfloat(m_size.x) / m_tileSize), m_tileSheet.getSize().x / m_tileSize)) + 1; x++)
				{
					for(Uint16 y = 0; y < Uint16(min(ceil(GLfloat(m_size.y) / m_tileSize), m_tileSheet.getSize().y / m_tileSize)) + 1; y++)
					{
						glTexCoord2f(GLfloat((x + floor(GLfloat(m_scroll.x) / m_tileSize)) / m_tileRatio.x), 1.f - (GLfloat((y + floor(GLfloat(m_scroll.y) / m_tileSize)) / m_tileRatio.y)));
						glVertex2f(GLfloat(x * m_tileSize), GLfloat(y * m_tileSize));
						glTexCoord2f(GLfloat((x + floor(GLfloat(m_scroll.x) / m_tileSize)) / m_tileRatio.x) + (1.f / m_tileRatio.x), 1.f - (GLfloat((y + floor(GLfloat(m_scroll.y) / m_tileSize)) / m_tileRatio.y)));
						glVertex2f(GLfloat(x * m_tileSize) + m_tileSize, GLfloat(y * m_tileSize));
						glTexCoord2f(GLfloat((x + floor(GLfloat(m_scroll.x) / m_tileSize)) / m_tileRatio.x) + (1.f / m_tileRatio.x), 1.f - (GLfloat((y + floor(GLfloat(m_scroll.y) / m_tileSize)) / m_tileRatio.y) + (1.f / m_tileRatio.y)));
						glVertex2f(GLfloat(x * m_tileSize) + m_tileSize, GLfloat(y * m_tileSize) + m_tileSize);
						glTexCoord2f(GLfloat((x + floor(GLfloat(m_scroll.x) / m_tileSize)) / m_tileRatio.x), 1.f - (GLfloat((y + floor(GLfloat(m_scroll.y) / m_tileSize)) / m_tileRatio.y) + (1.f / m_tileRatio.y)));
						glVertex2f(GLfloat(x * m_tileSize), GLfloat(y * m_tileSize) + m_tileSize);
					}
				}
			}
			glEnd();
			
			glBindTexture(GL_TEXTURE_2D, m_selectTex.getId());
			if(m_selectedTile.x >= floor(m_scroll.x / m_tileSize) && m_selectedTile.x < floor(m_scroll.x / m_tileSize) + ceil(GLfloat(m_size.x) / m_tileSize)
				&& m_selectedTile.y >= floor(m_scroll.y / m_tileSize) && m_selectedTile.y < floor(m_scroll.y / m_tileSize) + ceil(GLfloat(m_size.y) / m_tileSize))
			{
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0, 0);
					glVertex2f(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize);
					glTexCoord2f(1, 0);
					glVertex2f(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize + m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize);
					glTexCoord2f(1, 1);
					glVertex2f(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize + m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize + m_tileSize);
					glTexCoord2f(0, 1);
					glVertex2f(GLfloat(m_selectedTile.x - floor(m_scroll.x / m_tileSize)) * m_tileSize, GLfloat(m_selectedTile.y - floor(m_scroll.y / m_tileSize)) * m_tileSize + m_tileSize);
				}
				glEnd();
			}

			glBindTexture(GL_TEXTURE_2D, 0);

			glTranslatef(GLfloat(m_scroll.x % m_tileSize), GLfloat(m_scroll.y % m_tileSize), 0);

			if(m_hover || m_dragging)
			{
				m_colorTheme.m_back.useColor();
				glBegin(GL_QUADS);
				{
					glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet.getSize().x - m_size.x + m_tileSize - 1)) * (m_size.x - 80) + 4, GLfloat(m_size.y - 12));
					glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet.getSize().x - m_size.x + m_tileSize - 1)) * (m_size.x - 80) + 60, GLfloat(m_size.y - 12));
					glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet.getSize().x - m_size.x + m_tileSize - 1)) * (m_size.x - 80) + 60, GLfloat(m_size.y - 4));
					glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet.getSize().x - m_size.x + m_tileSize - 1)) * (m_size.x - 80) + 4, GLfloat(m_size.y - 4));

					glVertex2f(GLfloat(m_size.x - 12), (GLfloat(m_scroll.y) / (m_tileSheet.getSize().y - m_size.y + m_tileSize - 1)) * (m_size.y - 80) + 4);
					glVertex2f(GLfloat(m_size.x - 4), (GLfloat(m_scroll.y) / (m_tileSheet.getSize().y - m_size.y + m_tileSize - 1)) * (m_size.y - 80) + 4);
					glVertex2f(GLfloat(m_size.x - 4), (GLfloat(m_scroll.y) / (m_tileSheet.getSize().y - m_size.y + m_tileSize - 1)) * (m_size.y - 80) + 60);
					glVertex2f(GLfloat(m_size.x - 12), (GLfloat(m_scroll.y) / (m_tileSheet.getSize().y - m_size.y + m_tileSize - 1)) * (m_size.y - 80) + 60);
				}
				glEnd();
				m_colorTheme.m_fore.useColor();
				glBegin(GL_QUADS);
				{
					glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet.getSize().x - m_size.x + m_tileSize - 1)) * (m_size.x - 80) + 5, GLfloat(m_size.y - 11));
					glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet.getSize().x - m_size.x + m_tileSize - 1)) * (m_size.x - 80) + 59, GLfloat(m_size.y - 11));
					glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet.getSize().x - m_size.x + m_tileSize - 1)) * (m_size.x - 80) + 59, GLfloat(m_size.y - 5));
					glVertex2f((GLfloat(m_scroll.x) / (m_tileSheet.getSize().x - m_size.x + m_tileSize - 1)) * (m_size.x - 80) + 5, GLfloat(m_size.y - 5));

					glVertex2f(GLfloat(m_size.x - 11), (GLfloat(m_scroll.y) / (m_tileSheet.getSize().y - m_size.y + m_tileSize - 1)) * (m_size.y - 80) + 5);
					glVertex2f(GLfloat(m_size.x - 5), (GLfloat(m_scroll.y) / (m_tileSheet.getSize().y - m_size.y + m_tileSize - 1)) * (m_size.y - 80) + 5);
					glVertex2f(GLfloat(m_size.x - 5), (GLfloat(m_scroll.y) / (m_tileSheet.getSize().y - m_size.y + m_tileSize - 1)) * (m_size.y - 80) + 59);
					glVertex2f(GLfloat(m_size.x - 11), (GLfloat(m_scroll.y) / (m_tileSheet.getSize().y - m_size.y + m_tileSize - 1)) * (m_size.y - 80) + 59);
				}
				glEnd();
			}

			Math::popScissor();
		}
		glPopMatrix();
	}

	void setSelectedTile(Uint16 p_index)
	{
		if(m_tileSheet.getId() > 0)
		{
			m_selectedTile.x = p_index % m_tileCount.x;
			m_selectedTile.y = Sint16(floor(GLfloat(p_index) / m_tileCount.x));
		}
		else
			m_selectedTile = {};
	}
	Uint16 getSelectedTile()
	{
		return Uint16(m_selectedTile.x + m_selectedTile.y * m_tileCount.x);
	}

	Sint8 isSelected()
	{
		return m_selected;
	}
};
