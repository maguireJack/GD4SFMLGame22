#pragma once
//SceneNode category used to dispatch commands
namespace Category
{
	enum Type
	{
		kNone = 0,
		kScene = 1 << 0,
		kPlayerCharacter = 1 << 1,
		kPlatform = 1 << 2,
	};
}