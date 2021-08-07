#pragma once
#include <libtcod.h>

class Terrain
{
public:
	Terrain() : bg_color_{ TCOD_black }, fg_color_{ TCOD_black }, character_{TCOD_CHAR_BLOCK1}, is_mountain_(false), is_water_(false), is_void_(true)
	{
	}

	Terrain(const TCODColor bg_color, const TCODColor fg_color,const int character, const bool is_mountain, const bool is_water) :
	bg_color_{ bg_color },
	fg_color_{fg_color},
	character_{ character },
	is_mountain_(is_mountain),
	is_water_(is_water),
	is_void_(false)
	{
	}

	const TCODColor bg_color_;
	const TCODColor fg_color_;
	const int character_;
	const bool is_mountain_;
	const bool is_water_;
	bool is_void_;
};

