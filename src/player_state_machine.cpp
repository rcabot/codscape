#include "player_state_machine.hpp"  

	
player_state_machine::~player_state_machine()
{
	
}


void player_state_machine::set_state(const std::string& new_state)
{
    state_ = new_state;
}


const player_state_machine::state_info& player_state_machine::get_current_state()
{
    return state_info_map_.at(state_);
}