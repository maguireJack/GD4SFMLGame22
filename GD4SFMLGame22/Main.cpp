#include "Game.hpp"

int main()
{
	TextureHolder game_textures;
	game_textures.Load(Textures::kAircraft, "Media/Textures/Eagle.png");
	Game game(game_textures);
	game.Run();
}

