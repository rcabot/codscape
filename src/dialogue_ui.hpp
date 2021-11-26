#ifndef DIALOGUE_UI_H
#define DIALOGUE_UI_H
#pragma once
class registry;
class TCODConsole;
class rect_transform;
class dialogue_state;

class dialogue_ui  
{
	private:
		registry& registry_;
		rect_transform& rect_transform_;
		dialogue_state& state_;
	public:

		dialogue_ui(registry& registry, rect_transform& rtrans, dialogue_state& state) : registry_{registry}, rect_transform_{rtrans}, state_{state} {}
		~dialogue_ui();

		void draw(TCODConsole* console);

};
#endif