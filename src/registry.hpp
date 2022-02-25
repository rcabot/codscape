#ifndef REGISTRY_H
#define REGISTRY_H
#pragma once

#include "dialogue_initiator.hpp"
#include "Map.h"
#include "Person.h"
#include "interactable.hpp"
#include "dialogue_ui.hpp"
#include "rect_transform.hpp"
#include "inventory.hpp"
#include "inventory_ui.hpp"
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
	dialogue_ui dialogue_ui_;
	rect_transform dialogue_ui_transform_;
	inventory inventory_;
	inventory_ui inventory_ui_;
	rect_transform inventory_ui_transform_;


	registry() : 
		maps_{}, 
		people_{}, 
		interactables_{}, 
		player_state_machine_{}, 
		dialogue_state_{*this}, 
		dialogue_initiator_{player_state_machine_, dialogue_state_},
		dialogue_ui_transform_{} ,
		dialogue_ui_{*this,dialogue_ui_transform_,dialogue_state_},
		inventory_{},
		inventory_ui_{*this,inventory_ui_transform_,inventory_}
		{}

	void add_new_person_at(const Vector2& position, const std::string name, int map_index_)
    {
        auto start_dialogue_with_person = std::function([name](registry& r){
            r.dialogue_initiator_.start_dialogue_with(name);
        });
        people_.emplace_back(std::make_unique<person>(name, maps_[map_index_].get(), position));
        interactables_.emplace_back(position,std::move(start_dialogue_with_person),*this);
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