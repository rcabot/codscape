#ifndef INTERACTABLE_H
#define INTERACTABLE_H
#pragma once

#include "Vector2.h"
#include <functional>
	
class interactable  
{
	public:

		interactable(Vector2 position, std::function<void()>&& on_interact) : position_(position), on_interact_(std::move(on_interact)){}
		~interactable() = default;

		Vector2 position_;

		void interact();

	private:
		
		std::function<void()> on_interact_;

};
#endif