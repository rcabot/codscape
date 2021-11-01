//#ifndef DIALOGUE_INITIATOR_H
//#define DIALOGUE_INITIATOR_H
#pragma once
#include "player_state_machine.hpp"
#include "dialogue_state.hpp"
#include <string>
	
class dialogue_initiator  
{
	private:
		player_state_machine& player_state_;
		dialogue_state& dialogue_state_;
	public:

		dialogue_initiator(player_state_machine& player_state, dialogue_state& dialogue_state)
			: player_state_(player_state), dialogue_state_(dialogue_state) {}
		~dialogue_initiator() = default;
		void start_dialogue_with(const std::wstring& person_name);

};
//#endif