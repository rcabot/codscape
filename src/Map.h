#pragma once
#include <functional>
#include <memory>
#include <string>

#include "PerlinNoise.h"
#include "rex_paint_chars.h"
#include "Terrain.h"
#include "Vector2.h"
constexpr int TERRAIN_SWATCHES = 9;
constexpr int  CACHE_DIMENSION = 1000;

class Map
{
public:
	double distance_squared(double x, double y) const;
	short compute_swatch_index_at(Vector2 pos);
	const Terrain& get_terrain_at(Vector2 pos) const;
	Vector2 get_first_position_where_terrain_is(const std::function<bool(const Terrain&)>& matches, const int skip) const;

	explicit Map(const std::string& name) : Map()
	{
		name_ = name;
	}

	explicit Map(const std::string& name, const unsigned int seed) : Map(name)
	{
		map_terrain_ = PerlinNoise{ seed };
	}

	Map() :
		terrain_swatch_{
			Terrain(TCOD_dark_blue, TCOD_blue,ROUGHLY_EQUALS,false,true),
			Terrain(TCOD_blue, TCOD_light_blue,ROUGHLY_EQUALS,false,true),
			Terrain(TCOD_yellow, TCOD_dark_yellow,TCOD_CHAR_BLOCK1,false,false),
			Terrain(TCOD_green, TCOD_yellow,TCOD_CHAR_BLOCK1,false,false),
			Terrain(TCOD_dark_green, TCOD_green,TCOD_CHAR_BLOCK1,false,false),
			Terrain(TCOD_grey, TCOD_green,TCOD_CHAR_BLOCK1,true,false),
			Terrain(TCOD_dark_grey, TCOD_grey,TCOD_CHAR_BLOCK1,true,false),
			Terrain(TCOD_white, TCOD_grey,TCOD_CHAR_BLOCK1,true,false)
		},
		zoom_factor_{ 50.0 },
		cached_terrain_indices_{new short[CACHE_DIMENSION*CACHE_DIMENSION]},
		island_width_{ CACHE_DIMENSION },
		island_height_{ CACHE_DIMENSION }
	{
		const auto half_cache_dim{ CACHE_DIMENSION / 2 };
		for (auto x = 0; x < CACHE_DIMENSION; ++x)
		{
			for (auto y = 0; y < CACHE_DIMENSION; ++y)
			{
				cached_terrain_indices_[x + y * CACHE_DIMENSION] = compute_swatch_index_at(Vector2(x, y));
			}
		}
	}

private:
	PerlinNoise map_terrain_;
	Terrain terrain_swatch_[TERRAIN_SWATCHES];
	double zoom_factor_;
	std::unique_ptr<short[]> cached_terrain_indices_;
	double island_width_;
	double island_height_;
	std::string name_;
	Terrain void_terrain_swatch_;
};

