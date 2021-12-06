#ifndef DIALOGUE_STATE_MACHINE_H
#define DIALOGUE_STATE_MACHINE_H
#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class registry;

class dialogue_state
{
	struct node{
		std::vector<std::string> lines;
	};
	private:

	public:
		std::string talking_to_name_;
		std::string current_node_;
		registry& registry_;
		std::unordered_map<std::string,std::string> raw_text_nodes_;
		std::unordered_map<std::string,node> nodes_;
		int current_line_index_;

		dialogue_state(registry& registry) : talking_to_name_{""}, registry_{registry} {}
		~dialogue_state();

		void advance();
		void create_node(std::string title,std::string fulltext);
		void start_talking_to(std::string person_name);
		std::string get_current_text();

};
#endif