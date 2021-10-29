#ifndef INTERACTABLE_H
#define INTERACTABLE_H
#pragma once

#include "Vector2.h"
#include <functional>

class registry;
	
class interactable  
{
	public:

		interactable(Vector2 position, std::function<void(registry&)>&& on_interact, registry& reg) 
			: position_(position), on_interact_(std::move(on_interact)), reg(reg){}
		~interactable() = default;

		Vector2 position_;

		void interact();

	private:
		
		std::function<void(registry&)> on_interact_;
		registry& reg;

};
#endif