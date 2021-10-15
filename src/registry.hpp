#ifndef REGISTRY_H
#define REGISTRY_H
#pragma once

#include "Map.h"
#include "Person.h"
#include "interactable.hpp"
#include <sstream>
#include <limits>

template<typename T>
using ulist = std::vector<std::unique_ptr<T>>;

class registry  
{
private:


public:

	ulist<Map> maps_;
	ulist<person> people_;
	ulist<interactable> interactables_;
	registry() : maps_{}, people_{}, interactables_{} {

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

	template<class...Params>
	void add_map(Params&&... args)
	{
		maps_.emplace_back(std::make_unique<Map>(args...));
	}

	bool try_get_nearest_interactable_object_in_radius(const Vector2 center, const float radius, interactable* nearest_interactable)
	{
		float min_distance{std::numeric_limits<float>::max()};
		bool success{false};
		for(auto& interactable : interactables_)
		{
			float distance = center.distance(interactable->position_);
			if(radius > distance && distance < min_distance)
			{
				success = true;
				min_distance = distance;
				nearest_interactable = interactable.get();
			}
		}
		return success;
	}

};
#endif