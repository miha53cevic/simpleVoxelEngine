#pragma once
#include <SFML/Graphics.hpp>

class ToggleKey
{
public:
	ToggleKey(sf::Keyboard::Key l_key, float l_fTimeDelay = 0.2f)
		: m_key(l_key)
		, m_fTimeDelay(l_fTimeDelay)
	{}

	bool isKeyPressed()
	{
		if (m_delay.getElapsedTime().asSeconds() > m_fTimeDelay) {
			if (sf::Keyboard::isKeyPressed(m_key)) {
				m_delay.restart();
				return true;
			}
		}
		return false;
	}

private:
	sf::Keyboard::Key m_key;
	sf::Clock m_delay;

	float m_fTimeDelay;
};