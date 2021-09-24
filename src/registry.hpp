#ifndef REGISTRY_H
#define REGISTRY_H
#pragma once

#include "Map.h"
#include "Person.h"
#include <sstream>

template<typename T>
using ulist = std::vector<std::unique_ptr<T>>;

class registry  
{
private:


public:

	ulist<Map> maps_;
	ulist<person> people_;
	registry() : maps_{}, people_{} {

	}

	void add_new_person_at(const Vector2& position, const std::wstring& name, int map_index_)
	{
		people_.emplace_back(std::make_unique<person>(name, maps_[map_index_].get(), position));
	}

	void add_new_person_at(const Vector2& pos, const std::string& name, int map_index_)
	{
		std::wstringstream converter;
		converter << name.c_str();
		add_new_person_at(pos, converter.str(),map_index_);
	}

	template<typename T>
	void emplace_gameobject(std::unique_ptr<T> gameobject)
	{
		static_assert(false,"This game object isn't supported yet.");
	}

	template<typename T, typename...Params>
	void make_new_gameobject(Params&&... args)
	{
		auto ptr{ std::make_unique<T>(args...) };
		emplace_gameobject(std::move(ptr));
	}

};

template <>
void registry::emplace_gameobject<person>(std::unique_ptr<person> gameobject);

template <>
void registry::emplace_gameobject<Map>(std::unique_ptr<Map> gameobject);
#endif