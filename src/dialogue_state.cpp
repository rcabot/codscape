#include "dialogue_state.hpp"  
#include "registry.hpp"
#include <iostream>
dialogue_state::~dialogue_state()
{
	
}


void dialogue_state::advance()
{
    talking_to_name_ = "";
    registry_.player_state_machine_.set_state("neutral");
}

std::string dialogue_state::get_current_text()
{
    return raw_text_nodes_[talking_to_name_];
}

void dialogue_state::create_node(std::string title,std::string fulltext)
{
    raw_text_nodes_[title] = fulltext;
}