#pragma once

#include "engine\utils\variable\datatype\Color.h"
#include "engine\utils\variable\datatype\Rectangle.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Vector4.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\Globals.h"
#include "engine\utils\OpenGL.h"
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

namespace Math
{
	static GLfloat round2f(GLfloat p_f, Sint32 p_decimalPlaces)
	{
		return GLfloat(roundf(p_f * GLfloat(pow(10, p_decimalPlaces))) / pow(10, p_decimalPlaces));
	}

	static GLfloat sind(GLfloat p_f) /* sin of decimal number */
	{
		return GLfloat(sin((double)(p_f * 3.14159265359 / 180)));
	}

	static GLfloat cosd(GLfloat p_f) /* cos of decimal number */
	{
		return GLfloat(cos((double)(p_f * 3.14159265359 / 180)));
	}

	static GLfloat tand(GLfloat p_f) /* tan of decimal number */
	{
		return GLfloat(tan((double)(p_f * 3.14159265359 / 180)));
	}

	static Vector3<GLfloat> computeDirection(Vector3<GLfloat> p_rotation)
	{
		GLfloat ydist = GLfloat(2 * sind(p_rotation.x));
		GLfloat xdist = GLfloat(sqrt(pow(2, 2) - pow(ydist, 2)));
		GLfloat zdist = GLfloat(xdist * sind(p_rotation.y));
		xdist = xdist * sind((p_rotation.y - 90));
		return Vector3< GLfloat >(zdist, ydist, xdist).getNormal();
	}

	static std::vector<Rect> m_scissorStack;

	static Rect pushScissor(Rect p_area)
	{
		Rect _rect;
		float _mat[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, _mat);
		bool _sci = (glIsEnabled(GL_SCISSOR_TEST) != 0);
		int _scissorBox[4];

		if(m_scissorStack.empty())
		{
			_rect = p_area;
			_rect.x += _mat[12] + Globals::getInstance().m_screenSize.x / 2;
			_rect.y = Globals::getInstance().m_screenSize.y - (p_area.y + _mat[13] + Globals::getInstance().m_screenSize.y / 2 + p_area.h);
			glEnable(GL_SCISSOR_TEST);
		}
		else
		{
			glGetIntegerv(GL_SCISSOR_BOX, _scissorBox);
			_rect.x = GLfloat(max(GLint(_mat[12] + p_area.x + Globals::getInstance().m_screenSize.x / 2), _scissorBox[0]));
			_rect.y = GLfloat(max(GLint(Globals::getInstance().m_screenSize.y - (p_area.y + _mat[13] + Globals::getInstance().m_screenSize.y / 2 + p_area.h)), _scissorBox[1]));
			if((_mat[12] + p_area.x + Globals::getInstance().m_screenSize.x / 2) + p_area.w <= _scissorBox[0] + _scissorBox[2])
				_rect.w = p_area.w;
			else
				_rect.w = max(0, p_area.w - (((_mat[12] + p_area.x + Globals::getInstance().m_screenSize.x / 2) + p_area.w) - (_scissorBox[0] + _scissorBox[2])));
			if((Globals::getInstance().m_screenSize.y - (p_area.y + _mat[13] + Globals::getInstance().m_screenSize.y / 2 + p_area.h)) + p_area.h < _scissorBox[1] + _scissorBox[3])
				_rect.h = p_area.h;
			else
				_rect.h = max(0, p_area.h - (((Globals::getInstance().m_screenSize.y - (p_area.y + _mat[13] + Globals::getInstance().m_screenSize.y / 2 + p_area.h)) + p_area.h) - (_scissorBox[1] + _scissorBox[3])));
		}
		glScissor(GLint(_rect.x), GLint(_rect.y), GLsizei(_rect.w), GLsizei(_rect.h));
		m_scissorStack.push_back(_rect);
		return _rect;
	}

	static void popScissor()
	{
		m_scissorStack.pop_back();
		if(!m_scissorStack.empty())
		{
			Rect _rect = m_scissorStack[m_scissorStack.size() - 1];
			glScissor(GLint(_rect.x), GLint(_rect.y), GLsizei(_rect.w), GLsizei(_rect.h));
		}
		else
			glDisable(GL_SCISSOR_TEST);
	}

	static Rect getScissor()
	{
		if(!m_scissorStack.empty())
			return m_scissorStack[m_scissorStack.size() - 1];
		return Rect();
	}
};

class Util
{
public:
	template<class T>
	static std::string numToString(T n, Uint16 p_decimalPlaces = 0)
	{
		try
		{
			return std::string(std::to_string(n)).substr(0, std::string(std::to_string(n)).find('.', 0) + p_decimalPlaces + (p_decimalPlaces > 0 ? 1 : 0));
		}
		catch(...) {
			std::cout << "Warning: Arg passed to Util::numToString( T n ) was not a number." << std::endl;;
		}
		return "0";
	}

	template<class T>
	static std::string stringToNum(T s)
	{
		std::string save = "";
		std::ostringstream convert;
		convert.str(std::string());
		try {
			convert << s;
			save = convert.str();
			return save;
		}
		catch(std::exception e) {
			return std::string("Incorrect Variable Type!");
		}
	}

	static std::string subchar(char* chararray, Uint32 start, Uint32 end)
	{
		std::string subbed;
		for(Uint32 i = start; i < end + 1; i++)
		{
			subbed += chararray[i];
		}
		return subbed;
	}

};

class FileExt
{
public:
	static void writeInt(std::ofstream& p_fileStream, Uint32 p_uint)
	{
		p_fileStream << Uint8((p_uint & 0xFF000000) >> 24);
		p_fileStream << Uint8((p_uint & 0xFF0000) >> 16);
		p_fileStream << Uint8((p_uint & 0xFF00) >> 8);
		p_fileStream << Uint8((p_uint & 0xFF));
	}
	static void writeShort(std::ofstream& p_fileStream, Uint16 p_ushort)
	{
		p_fileStream << Uint8((p_ushort & 0xFF00) >> 8);
		p_fileStream << Uint8((p_ushort & 0xFF));
	}
	static void writeChar(std::ofstream& p_fileStream, Uint8 p_uchar)
	{
		p_fileStream << p_uchar;
	}

	static Uint32 readInt(char* p_fileStream, Uint32& p_index)
	{
		Uint16 _value;
		_value = readChar(p_fileStream, p_index) << 24;
		_value += readChar(p_fileStream, p_index) << 16;
		_value += readChar(p_fileStream, p_index) << 8;
		_value += readChar(p_fileStream, p_index);
		return _value;
	}
	static Uint16 readShort(char* p_fileStream, Uint32& p_index)
	{
		Uint16 _value;
		_value = readChar(p_fileStream, p_index) << 8;
		_value += readChar(p_fileStream, p_index);
		return _value;
	}
	static Uint8 readChar(char* p_fileStream, Uint32& p_index)
	{
		p_index = p_index + 1;
		return Uint8(p_fileStream[p_index - 1]);
	}
};
