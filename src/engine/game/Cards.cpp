#include "engine\game\Cards.h"

void Cards::init()
{
	load();
	addCard({0, "None", 10, 10, LTexture::getInstance().getImage("Char.png"), LTexture::getInstance().getImage("CharCard.png")});
}

Uint16 Cards::addCard(Card p_item)
{
	Uint16 i = 0;
	while(i < m_cards.size() && p_item.id > m_cards[i].id)
		i++;
	if(i < m_cards.size() && p_item.id == m_cards[i].id)
		m_cards.erase(m_cards.begin() + i);
	m_cards.insert(m_cards.begin() + i, p_item);
	return i;
}

Cards::Card Cards::getCard(Uint16 p_id)
{
	for(Uint16 i = 0; i < m_cards.size(); i++)
	{
		if(m_cards[i].id == p_id)
			return m_cards[i];
		if(m_cards[i].id > p_id)
			return m_cards[0];
	}
}

void Cards::save()
{
	std::ofstream _file;

	_file.open(std::string("res\\Cards"), std::ios::binary);
	{
		for(Uint16 i = 1; i < m_cards.size(); i++)
		{
			FileExt::writeShort(_file, m_cards[i].id);
			FileExt::writeShort(_file, m_cards[i].m_name.length());
			for(Uint16 j = 0; j < m_cards[i].m_name.length(); j++)
				FileExt::writeChar(_file, m_cards[i].m_name[j]);
			FileExt::writeShort(_file, m_cards[i].m_attack);
			FileExt::writeShort(_file, m_cards[i].m_health);
			FileExt::writeShort(_file, m_cards[i].m_cardTexture.getName().length());
			for(Uint16 j = 0; j < m_cards[i].m_cardTexture.getName().length(); j++)
				FileExt::writeChar(_file, m_cards[i].m_cardTexture.getName()[j]);
			FileExt::writeShort(_file, m_cards[i].m_cardSprite.getName().length());
			for(Uint16 j = 0; j < m_cards[i].m_cardSprite.getName().length(); j++)
				FileExt::writeChar(_file, m_cards[i].m_cardSprite.getName()[j]);
		}
	}
	_file.close();
}

void Cards::load()
{
	std::ifstream _file;
	Uint32 _length, _index = 0;
	char* _data;

	Uint16 _id, _attack, _health;
	Uint16 _cardNameLen, _texNameLen, _spriteNameLen;
	std::string _cardName, _texName, _spriteName;

	m_cards.clear();

	_file.open(std::string("res\\Cards").c_str(), std::ios::binary);
	{
		if(!_file.good())
		{
			std::cout << "No Cards file found." << std::endl;
			return;
		}
		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		_data = new char[_length];
		_file.read(_data, _length);

		while(_index < _length)
		{
			_cardName = _texName = _spriteName = "";

			_id = FileExt::readShort(_data, _index);
			_cardNameLen = FileExt::readShort(_data, _index);
			for(Uint16 i = 0; i < _cardNameLen; i++)
				_cardName += FileExt::readChar(_data, _index);
			_attack = FileExt::readShort(_data, _index);
			_health = FileExt::readShort(_data, _index);
			_texNameLen = FileExt::readShort(_data, _index);
			for(Uint16 i = 0; i < _texNameLen; i++)
				_texName += FileExt::readChar(_data, _index);
			_spriteNameLen = FileExt::readShort(_data, _index);
			for(Uint16 i = 0; i < _spriteNameLen; i++)
				_spriteName += FileExt::readChar(_data, _index);
			addCard(Card(_id, _cardName, _attack, _health, LTexture::getInstance().getImage(_texName), LTexture::getInstance().getImage(_spriteName)));
		}
		delete[] _data;
	}
	_file.close();
}
