
#pragma once
#include "Vector2.h"
	
class rect_transform  
{
	private:
	
	public:
		Vector2 position;
		Vector2 size;
		rect_transform() : position{},size{} {}
		~rect_transform();

};