#include "engine\game\Items.h"

void Items::init()
{
	load();
	addItem({0, "None"});
	addItem({1, "Random Card"});
}

Uint16 Items::addItem(Item p_item)
{
	Uint16 i = 0;
	while(i < m_items.size() && p_item.id > m_items[i].id)
		i++;
	if(i < m_items.size() && p_item.id == m_items[i].id)
		m_items.erase(m_items.begin() + i);
	m_items.insert(m_items.begin() + i, p_item);
	return i;
}

Items::Item Items::getItem(Uint16 p_id)
{
	for(Uint16 i = 0; i < m_items.size(); i++)
	{
		if(m_items[i].id == p_id)
			return m_items[i];
		if(m_items[i].id > p_id)
			return m_items[0];
	}
}

void Items::save()
{
	std::ofstream _file;

	_file.open(std::string("res\\Items"), std::ios::binary);
	{
		for(Uint16 i = 1; i < m_items.size(); i++)
		{
			FileExt::writeShort(_file, m_items[i].id);
			FileExt::writeShort(_file, m_items[i].m_name.length());
			for(Uint16 j = 0; j < m_items[i].m_name.length(); j++)
				FileExt::writeChar(_file, m_items[i].m_name[j]);
			FileExt::writeShort(_file, m_items[i].m_texture.getName().length());
			for(Uint16 j = 0; j < m_items[i].m_texture.getName().length(); j++)
				FileExt::writeChar(_file, m_items[i].m_texture.getName()[j]);
		}
	}
	_file.close();
}

void Items::load()
{
	std::ifstream _file;
	Uint32 _length, _index = 0;
	char* _data;

	Uint16 _id;
	Uint16 _itemNameLen, _texNameLen;
	std::string _itemName, _texName;

	m_items.clear();

	_file.open(std::string("res\\Items").c_str(), std::ios::binary);
	{
		if(!_file.good())
		{
			std::cout << "No Items file found." << std::endl;
			return;
		}
		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		_data = new char[_length];
		_file.read(_data, _length);

		while(_index < _length)
		{
			_itemName = _texName = "";

			_id = FileExt::readShort(_data, _index);
			_itemNameLen = FileExt::readShort(_data, _index);
			for(Uint16 i = 0; i < _itemNameLen; i++)
				_itemName += FileExt::readChar(_data, _index);
			_texNameLen = FileExt::readShort(_data, _index);
			for(Uint16 i = 0; i < _texNameLen; i++)
				_texName += FileExt::readChar(_data, _index);
			addItem(Item(_id, _itemName, LTexture::getInstance().getImage(_texName)));
		}
		delete[] _data;
	}
	_file.close();
}
