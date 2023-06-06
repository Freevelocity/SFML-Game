#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
}

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& pValue);
std::string toString(sf::Keyboard::Key pKey);
void CenterOrigin(sf::Sprite& pSprite);
void CenterOrigin(sf::Text& pText);
float toDegree(float pRadian);
float toRadian(float pDegree);
int randomInt(int pExclusiveMax);
float length(sf::Vector2f pVector);
sf::Vector2f unitVector(sf::Vector2f pVector);

#include "Utility.Inl"

