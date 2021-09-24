#include "registry.hpp"  


template <>
void registry::emplace_gameobject<person>(std::unique_ptr<person> gameobject)
{
	people_.emplace_back(std::move(gameobject));
}

template <>
void registry::emplace_gameobject<Map>(std::unique_ptr<Map> gameobject)
{
	maps_.emplace_back(std::move(gameobject));
}
