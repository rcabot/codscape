#include "dialogue_initiator.hpp"
#include <string>
#include "util_functions.h"
#include <iostream>
void dialogue_initiator::start_dialogue_with(const std::string& person_name)
{
    player_state_.set_state("dialogue");
    dialogue_state_.talking_to_name_ = person_name;
}