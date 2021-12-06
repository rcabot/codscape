#include "dialogue_state.hpp"  
#include "registry.hpp"
#include <sstream>

dialogue_state::~dialogue_state()
{
	
}


void dialogue_state::advance()
{
    current_line_index_++;
    if(current_line_index_ >= nodes_[talking_to_name_].lines.size())
    {
        current_line_index_ = 0;
        talking_to_name_ = "";
        registry_.player_state_machine_.set_state("neutral");
    }
}

std::string dialogue_state::get_current_text()
{
    auto el {nodes_.find(talking_to_name_)};
    if(el==nodes_.end()) return "";
    if(current_line_index_ >= el->second.lines.size()) return "";
    return el->second.lines[current_line_index_];
}

void dialogue_state::start_talking_to(std::string person_name)
{
    talking_to_name_ = person_name;
    current_node_ = person_name;
}

void dialogue_state::create_node(std::string title,std::string fulltext)
{
    raw_text_nodes_[title] = fulltext;

    std::istringstream text_stream(fulltext);
    std::string line;
    while (std::getline(text_stream, line))
    {
        if (!line.empty()) nodes_[title].lines.push_back(line);
    }
    
}