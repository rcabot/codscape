#include "Map.h"
#include "Vector2.h"

double Map::distance_squared(const double x, const double y) const
{
	const auto dx = 2 * x / island_width_ - 1;
	const auto dy = 2 * y / island_height_ - 1;
	return dx * dx + dy * dy;
}

short Map::compute_swatch_index_at(const Vector2 pos)
{
	const auto zoomed_x{ pos.x / zoom_factor_ };
	const auto zoomed_y{ pos.y / zoom_factor_ };
	auto elevation{ map_terrain_.noise(zoomed_x, zoomed_y, 1.0) };
	elevation = (1 + elevation - distance_squared(pos.x, pos.y)) / 2.0;
	elevation = elevation < 0 ? 0 : elevation;
	elevation = elevation > 1 ? 1 : elevation;
	const auto swatch{ static_cast<short>(floor(elevation * (TERRAIN_SWATCHES - 1))) };
	return swatch;
}

const Terrain& Map::get_terrain_at(const Vector2 pos) const
{
	if (pos.x < 0 || pos.x >= CACHE_DIMENSION || pos.y < 0 || pos.y >= CACHE_DIMENSION)
		return void_terrain_swatch_;
	return terrain_swatch_[cached_terrain_indices_[pos.x + pos.y * CACHE_DIMENSION]];
}

Vector2 Map::get_first_position_where_terrain_is(const std::function<bool(const Terrain&)>& matches, const int skip = 0) const
{
	const auto half_cache_dim{ CACHE_DIMENSION / 2 };
	auto found{ 0 };
	for (auto x = 0; x < CACHE_DIMENSION; ++x)
	{
		for (auto y = 0; y < CACHE_DIMENSION; ++y)
		{
			const Vector2 pos{ x,y };
			if(matches(get_terrain_at(pos)) && ++found > skip)
			{
				return pos;
			}
		}
	}
	return Vector2{ 0,0 };
}
