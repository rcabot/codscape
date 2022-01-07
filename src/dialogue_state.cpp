#include "dialogue_state.hpp"  
#include "registry.hpp"
#include <sstream>

dialogue_state::~dialogue_state()
{
	
}


void dialogue_state::advance()
{
    bool wait_for_next_advance = false;

    do
    {
        current_line_index_++;

        const node& current_node{nodes_[current_node_]};
        // reached the end of the current node?
        if(current_line_index_ >= current_node.expressions.size())
        {
            // end dialogue and reset
            talking_to_name_ = "";
            current_text_ = "";
            registry_.player_state_machine_.set_state("neutral");
            current_line_index_ = -1;
            return;
        }
        // otherwise, execute command
        const expression& current_expression{current_node.expressions[current_line_index_]};
        switch (current_expression.command)
        {
        case command::DISPLAY:
            current_text_ = current_expression.operands[0];
            wait_for_next_advance = true;
            break;
        case command::GOTO_NODE:
            current_line_index_ = -1;
            current_node_ = current_expression.operands[0];
            break;
        case command::ADD_OPTION:
            options_.emplace_back(current_expression.operands[0],current_expression.operands[1]);
            break;
        
        default:
            break;
        }
    }
    while(!wait_for_next_advance);
}

std::string dialogue_state::get_current_text()
{
    return current_text_;
    
}

std::string dialogue_state::get_talking_to_name()
{
    return talking_to_name_;
    
}

void dialogue_state::start_talking_to(std::string person_name)
{
    current_text_ = "";
    talking_to_name_ = person_name;
    current_node_ = person_name;
    current_line_index_ = -1;
    advance();
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
        if(line.rfind("[[",0) == 0)
        {
            auto divider_pos{line.rfind("|")};
            if(divider_pos!=-1)
            {
                // assumes that the pointer has no whitespace
                std::vector<std::string> operands{line.substr(2,divider_pos-2),line.substr(divider_pos+1,(line.size() - (divider_pos+1)) - 2)};
                nodes_[title].expressions.emplace_back(command::ADD_OPTION,operands);
            }
            else
            {
                // assumes that the pointer has no whitespace
                std::vector<std::string> operands{line.substr(2,line.size()-4)};
                nodes_[title].expressions.emplace_back(command::GOTO_NODE,operands);
            }
        }
        // if this line is dialogue to be displayed
        else if (!line.empty())
        {
            std::vector<std::string> operands{line};
            nodes_[title].expressions.emplace_back(command::DISPLAY,operands);
        }
    }
    
}