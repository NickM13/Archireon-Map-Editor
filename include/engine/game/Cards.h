#pragma once

#include "engine\gfx\LTexture.h"
#include "engine\utils\variable\datatype\Macros.h"

#include <vector>

class Cards : public Singleton<Cards>
{
public:
	struct Card
	{
		Uint16 id;
		std::string m_name;
		Uint16 m_attack, m_health; // Health can also be turns trapped
		Texture m_cardTexture;
		Texture m_cardSprite;

		Card(Uint16 p_id = 0, std::string p_name = "", Uint16 p_attack = 0, Uint16 p_health = 0, Texture p_texture = Texture(), Texture p_sprite = Texture()) : id(p_id), m_name(p_name),
			m_attack(p_attack), m_health(p_attack), m_cardTexture(p_texture), m_cardSprite(p_sprite) {}
	};

	void init();

	Uint16 addCard(Card p_item);
	Card getCard(Uint16 p_id);
	std::vector<Card> getCards() { return m_cards; };
	void save();
	void load();
private:
	std::vector<Card> m_cards;
};
