#include "dialogue_state.hpp"  
#include "registry.hpp"
dialogue_state::~dialogue_state()
{
	
}


void dialogue_state::advance()
{
    talking_to_name_ = L"";
    registry_.player_state_machine_.set_state("neutral");
}