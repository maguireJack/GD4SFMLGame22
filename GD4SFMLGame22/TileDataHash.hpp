#pragma once
#include <memory>

#include "TileData.hpp"

/// <summary>
/// A hash function used to hash a TileNode.
/// </summary>
struct TileDataHash {
	std::size_t operator()(const TileData& tile) const
	{
		return std::hash<int>{}(static_cast<int>(tile.GetTexture()));
	}
};