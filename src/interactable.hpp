#ifndef INTERACTABLE_H
#define INTERACTABLE_H
#pragma once

#include "Vector2.h"
	
class interactable  
{
	public:

		interactable(Vector2 position) : position_{position} {}
		~interactable();

		Vector2 position_;

		void interact();

};
#endif