#ifndef DIALOGUE_STATE_MACHINE_H
#define DIALOGUE_STATE_MACHINE_H
#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class registry;

class dialogue_state
{
	enum command
	{
		DISPLAY,
		GOTO_NODE,
		ADD_OPTION,
		PAUSE,
	};

	struct expression
	{
		expression(command c, std::vector<std::string> o) : command{c}, operands{o} {}
		command command;
		std::vector<std::string> operands;
	};

	struct node
	{
		std::vector<expression> expressions;
	};
	struct option
	{
		option(std::string t, std::string n) : text{t}, node{n} {}
		std::string text;
		std::string node;
	};
	private:

		std::string talking_to_name_ = "";
		std::string current_node_ = "";
		std::string current_text_ = "";
		registry& registry_;
		std::unordered_map<std::string,std::string> raw_text_nodes_;
		std::unordered_map<std::string,node> nodes_;
		std::vector<option> options_;
		int next_line_index_ = 0;


	public:
		dialogue_state(registry& registry) : registry_{registry} {}
		~dialogue_state();
		void advance();
		void choose_option(int choice);
		void create_node(std::string title,std::string fulltext);
		void start_talking_to(std::string person_name);
		std::string get_current_text();
		std::string get_talking_to_name();
		const std::vector<option>& get_options();

};
#endif