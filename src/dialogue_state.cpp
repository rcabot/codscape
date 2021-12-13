#include "dialogue_state.hpp"  
#include "registry.hpp"
#include <sstream>

dialogue_state::~dialogue_state()
{
	
}


void dialogue_state::advance()
{
    current_line_index_++;
    if(current_line_index_ >= nodes_[current_node_].lines.size())
    {
        current_line_index_ = 0;
        if(!nodes_[current_node_].next.empty())
        {
            current_node_ = nodes_[current_node_].next;
        }
        else
        {
            talking_to_name_ = "";
            registry_.player_state_machine_.set_state("neutral");
        }
    }
}

std::string dialogue_state::get_current_text()
{
    auto el {nodes_.find(current_node_)};
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
        // if this is a pointer to node
        // i.e. [[somenode]]
        if(line.rfind("[[",0) == 0){
            // assumes that the pointer has no whitespace
            nodes_[title].next = line.substr(2,line.size()-4);
        }
        else if (!line.empty()) nodes_[title].lines.push_back(line);
    }
    
}