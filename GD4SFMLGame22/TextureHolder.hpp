#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Textures.hpp"
class TextureHolder
{
public:
	void Load(Textures id, const std::string& filename);
	sf::Texture& Get(Textures id);
	const sf::Texture& Get(Textures id) const;

private:
	std::map<Textures, std::unique_ptr<sf::Texture>> m_texture_map;
};
