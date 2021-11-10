#ifndef DIALOGUE_UI_H
#define DIALOGUE_UI_H
#pragma once
class registry;
class TCODConsole;

class dialogue_ui  
{
	private:
		registry& registry_;
	public:

		dialogue_ui(registry& registry) : registry_{registry} {}
		~dialogue_ui();

		void draw(TCODConsole* console);

};
#endif