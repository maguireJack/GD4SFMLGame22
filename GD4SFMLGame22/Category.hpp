#pragma once
//SceneNode category used to dispatch commands
namespace Category
{
	enum Type
	{
		kNone = 0,
		kScene = 1 << 0,
		kPlayerAircraft = 1 << 1,
		kAlliedAircraft = 1 << 2,
		kEnemyAircraft = 1 << 3,
		kPickup = 1 << 4,
		kAlliedProjectile = 1 << 5,
		kEnemyProjectile = 1 << 6,
		kPlatform = 1 << 7,

		kAircraft = kPlayerAircraft | kAlliedAircraft | kEnemyAircraft,
		kProjectile = kAlliedProjectile | kEnemyProjectile,
	};
}