#pragma once
#include <utility>


#include "Map.h"
#include "olcSprite.h"

class person
{
public:
	person() = delete;
	static std::unique_ptr<olcSprite> generate_random_sprite();

	person(std::string name, Map* location, const Vector2& position) :
		location_{ location },
		position_{position},
		name_{name}
	{
		sprite_ = generate_random_sprite();
	}

	void move(Map* new_location);
	Vector2 get_pos() const;
	olcSprite* get_sprite() const;
	Map* get_location() const;
	std::wstring to_json_string() const;
	static std::string generate_random_name();
private:
	Map* location_;
	std::unique_ptr<olcSprite> sprite_; //todo: sprites should be owned by the game
	Vector2 position_;
	std::string name_;
};

