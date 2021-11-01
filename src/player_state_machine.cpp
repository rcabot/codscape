#include "player_state_machine.hpp"  

	
player_state_machine::~player_state_machine()
{
	
}


void player_state_machine::set_state(const std::string& new_state)
{
    state_ = new_state;
}