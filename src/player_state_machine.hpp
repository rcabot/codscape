#ifndef PLAYER_STATE_MACHINE_H
#define PLAYER_STATE_MACHINE_H
#pragma once

#include <string>

class player_state_machine  
{
	private:
		std::string state_;
	public:

		player_state_machine() : state_{"init"} {}
		~player_state_machine();
		void set_state(const std::string& new_state);

};
#endif