#include "TextureHolder.hpp"
void TextureHolder::Load(Textures id, const std::string& filename)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	texture->loadFromFile(filename);
	m_texture_map.insert(std::make_pair(id, std::move(texture)));
}

sf::Texture& TextureHolder::Get(Textures id)
{
	auto found = m_texture_map.find(id);
	return *found->second;
}

const sf::Texture& TextureHolder::Get(Textures id) const
{
	auto found = m_texture_map.find(id);
	return *found->second;
}
