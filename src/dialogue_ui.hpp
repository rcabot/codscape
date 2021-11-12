#ifndef DIALOGUE_UI_H
#define DIALOGUE_UI_H
#pragma once
class registry;
class TCODConsole;
class rect_transform;

class dialogue_ui  
{
	private:
		registry& registry_;
		rect_transform& rect_transform_;
	public:

		dialogue_ui(registry& registry, rect_transform& rtrans) : registry_{registry}, rect_transform_{rtrans} {}
		~dialogue_ui();

		void draw(TCODConsole* console);

};
#endif