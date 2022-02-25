
#pragma once

class registry;
class TCODConsole;
class rect_transform;
class inventory;
	
class inventory_ui  
{
	private:
		registry& registry_;
		rect_transform& rect_transform_;
		inventory& inventory_;

	public:
		inventory_ui(registry& registry, rect_transform& rtrans, inventory& inventory) : registry_{registry}, rect_transform_{rtrans}, inventory_{inventory} {}
		
		~inventory_ui();
		void inventory_ui::draw(TCODConsole* console);

};