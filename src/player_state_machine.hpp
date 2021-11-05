#ifndef PLAYER_STATE_MACHINE_H
#define PLAYER_STATE_MACHINE_H
#pragma once

#include <string>
#include <unordered_map>

class player_state_machine  
{
	struct state_info
	{
		bool can_interact_;
		bool can_move_;
		bool can_advance_dialogue_;
	};
	private:
		std::string state_;
		const std::unordered_map<std::string,state_info> state_info_map_;
	public:

		player_state_machine() : state_{"neutral"}, state_info_map_{
			{"neutral", {true,true,false}},
			{"dialogue",{false,false,true}}
		} {}
		~player_state_machine();
		void set_state(const std::string& new_state);
		const state_info& get_current_state();
	
};
#endif