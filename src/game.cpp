#include "game.h"
#include "Person.h"

template <>
void game::emplace_gameobject<person>(std::unique_ptr<person> gameobject)
{
	registry_.people_.emplace_back(std::move(gameobject));
}

template <>
void game::emplace_gameobject<Map>(std::unique_ptr<Map> gameobject)
{
	registry_.maps_.emplace_back(std::move(gameobject));
}
