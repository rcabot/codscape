#ifndef REGISTRY_H
#define REGISTRY_H
#pragma once

#include "dialogue_initiator.hpp"
#include "Map.h"
#include "Person.h"
#include "interactable.hpp"
#include <sstream>
#include <limits>

template<typename T>
using uvector = std::vector<std::unique_ptr<T>>;

class registry  
{
private:


public:

	uvector<Map> maps_;
	uvector<person> people_;
	std::vector<interactable> interactables_;
	player_state_machine player_state_machine_;
	dialogue_state dialogue_state_;
	dialogue_initiator dialogue_initiator_;
	registry() 
		: maps_{}, people_{}, interactables_{}, player_state_machine_{}, 
		dialogue_state_{*this}, dialogue_initiator_{player_state_machine_, dialogue_state_} {}

	void add_new_person_at(const Vector2& position, const std::wstring name, int map_index_)
    {
        auto start_dialogue_with_person = std::function([name](registry& r){
            r.dialogue_initiator_.start_dialogue_with(name);
        });
        people_.emplace_back(std::make_unique<person>(name, maps_[map_index_].get(), position));
        interactables_.emplace_back(position,std::move(start_dialogue_with_person),*this);
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

	interactable* try_get_nearest_interactable_object_in_radius(const Vector2 center, const float radius)
	{
		interactable* nearest_interactable = nullptr;
		float min_distance{std::numeric_limits<float>::max()};
		for(auto& interactable : interactables_)
		{
			float distance = center.distance(interactable.position_);
			if(radius > distance && distance < min_distance)
			{
				min_distance = distance;
				nearest_interactable = &interactable;
			}
		}
		return nearest_interactable;
	}

};
#endif