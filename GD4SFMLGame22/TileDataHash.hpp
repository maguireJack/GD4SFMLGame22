#pragma once
#include <memory>

#include "TileData.hpp"

/// <summary>
/// A hash function used to hash a TileNode.
///	Implementation modified from boost hash_combine
/// </summary>
///	<see cref="https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x"/>
struct TileDataHash {
	std::size_t operator()(const TileData& tile) const
	{
		const size_t hash1 = std::hash<int>{}(static_cast<int>(tile.GetTexture()));
		const size_t hash2 = std::hash<int>{}(static_cast<int>(tile.GetPlatformType()));

		return hash1 ^ hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
	}
};