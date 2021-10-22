#pragma once
#include "Textures.hpp"

//Forward declaration of SFML texture class
namespace sf
{
	class Texture;
}

template<typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures> TextureHolder;