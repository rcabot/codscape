#include "dialogue_state.hpp"  
#include "registry.hpp"
#include <sstream>

dialogue_state::~dialogue_state()
{
	
}


void dialogue_state::advance()
{
    if(options_.size() > 0)
        return;

    // encapsulate this function as "execute next statement"
    while(next_line_index_ < nodes_[current_node_].expressions.size())
    {
        const node& current_node{nodes_[current_node_]};
        const expression& current_expression{current_node.expressions[next_line_index_]};
        switch (current_expression.command)
        {
        case command::DISPLAY:
        {
            current_text_ = current_expression.operands[0];
            next_line_index_++;
            break;
        }
        case command::GOTO_NODE:
        {
            next_line_index_ = 0;
            current_node_ = current_expression.operands[0];
            break;
        }
        case command::ADD_OPTION:
        {
            options_.emplace_back(current_expression.operands[0],current_expression.operands[1]);
            next_line_index_++;
            break;
        }
        case command::PAUSE:
        {
            // skip and return
            next_line_index_++;
            return;
        }
        default:
            next_line_index_++;
            break;
        }
    }

    // end dialogue and reset
    talking_to_name_ = "";
    current_text_ = "";
    registry_.player_state_machine_.set_state("neutral");
    next_line_index_ = 0;
    options_.clear();
}

std::string dialogue_state::get_current_text()
{
    return current_text_;
    
}

void dialogue_state::choose_option(int choice)
{
    if(options_.size() == 0) return;
    
    next_line_index_ = 0;
    current_node_ = options_[choice].node;
    options_.clear();
    advance();
}

std::string dialogue_state::get_talking_to_name()
{
    return talking_to_name_;
    
}


const std::vector<dialogue_state::option>& dialogue_state::get_options()
{
    return options_;
}

void dialogue_state::start_talking_to(std::string person_name)
{
    current_text_ = "";
    talking_to_name_ = person_name;
    current_node_ = person_name;
    next_line_index_ = 0;
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
        // i.e. [[somenode]] or [[Option text|optionnode]]
        // todo: these symbols need to be a constant
        const std::string pointer_prefix_symbol = "[[";
        const int pointer_prefix_symbol_length = 2;
        const std::string divider_prefix_symbol = "|";
        const int divider_prefix_symbol_length = 1;

        if(line.rfind(pointer_prefix_symbol,0) == 0)
        {
            auto divider_pos{line.rfind("|")};
            if(divider_pos!=-1)
            {
                // assumes that the pointer has no whitespace
                int operand_0_start = pointer_prefix_symbol_length;
                int operand_0_length = divider_pos - pointer_prefix_symbol_length;
                int operand_1_start = divider_pos + divider_prefix_symbol_length;
                int operand_1_length = (line.size() - operand_1_start) - pointer_prefix_symbol_length;
                std::vector<std::string> operands{
                    line.substr(operand_0_start,operand_0_length),
                    line.substr(operand_1_start,operand_1_length)};
                nodes_[title].expressions.emplace_back(command::ADD_OPTION,operands);
            }
            else
            {
                // assumes that the pointer has no whitespace
                int operand_0_start = pointer_prefix_symbol_length;
                int operand_0_length = line.size()-pointer_prefix_symbol_length*2;
                std::vector<std::string> operands{line.substr(operand_0_start,operand_0_length)};
                nodes_[title].expressions.emplace_back(command::GOTO_NODE,operands);
            }
        }
        else if (!line.empty()) 
        {
            // if this line is dialogue to be displayed
            std::vector<std::string> operands{line};
            nodes_[title].expressions.emplace_back(command::DISPLAY,operands);
            nodes_[title].expressions.emplace_back(command::PAUSE,operands);
        }
    }
    // append pauses to the end of options series
    for (int i = 0; i < nodes_[title].expressions.size(); i++)
    {
        if(nodes_[title].expressions[i].command == command::ADD_OPTION)
        {
            if(i+1 >= nodes_[title].expressions.size())
            {
                std::vector<std::string> operands;
                nodes_[title].expressions.emplace_back(command::PAUSE,operands);
            }
            if(nodes_[title].expressions[i].command != command::ADD_OPTION)
            {
                std::vector<std::string> operands;
                auto pos = nodes_[title].expressions.begin() + i + 1;
                nodes_[title].expressions.emplace(pos,command::PAUSE,operands);
            }
        }
    }
    
    
}