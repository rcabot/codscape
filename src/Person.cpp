#include "Person.h"



#include <sstream>
#include <utility>

#include "rex_paint_chars.h"
int head = 0;
std::unique_ptr<olcSprite> person::generate_random_sprite()
{
	auto new_sprite{ std::make_unique<olcSprite>(1, 2,0,1) };
	short heads[] = {  TCOD_CHAR_BULLET_SQUARE, TCOD_CHAR_DIAMOND, BLOCK_HALF_H, TCOD_CHAR_ARROW2_N,SPOKE_BULLET};
	short bodies[] = { TCOD_CHAR_TEEN,BLOCK_HALF_V, TCOD_CHAR_TEES, 173,BLOCK_HALF_H,TCOD_CHAR_NOTE_DOUBLE };
	const auto rand_head{ heads[head++ % 5] };
	const auto rand_body{ bodies[head % 6] };
	new_sprite->SetGlyph(0, 0, rand_head);
	new_sprite->SetFColour(0, 0, TCODColor(100 - rand() * 255, 100 - rand() * 255, 100 - rand() * 255));
	new_sprite->SetBColour(0, 0, TCODColor(100 + rand() * 255, 100 + rand() * 255, 100 + rand() * 255));
	new_sprite->SetGlyph(0, 1, rand_body);
	new_sprite->SetFColour(0, 1, TCODColor(100 - rand() * 255, 100 - rand() * 255, 100 - rand() * 255));
	new_sprite->SetBColour(0, 1, TCODColor(100 + rand() * 255, 100 + rand() * 255, 100 + rand() * 255));
	return std::move(new_sprite);
}

void person::move(Map* new_location)
{
	location_ = new_location;
}

Vector2 person::get_pos() const
{
	return position_;
}

olcSprite* person::get_sprite() const
{
	return sprite_.get();
}

Map* person::get_location() const
{
	return location_;
}

std::wstring person::to_json_string() const
{
	std::wstringstream ss;
	ss
		<< "{"
			<< R"("name" : ")" << name_ << R"(",)"
			<< R"("x" : )" << position_.x << ","
			<< R"("y" : )" << position_.y
		<< "}";
	return ss.str();
}

std::wstring person::generate_random_name()
{
	return std::to_wstring(rand() * 10000);
}
