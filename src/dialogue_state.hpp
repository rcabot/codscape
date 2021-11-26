#ifndef DIALOGUE_STATE_MACHINE_H
#define DIALOGUE_STATE_MACHINE_H
#pragma once
#include <string>
#include <map>

class registry;

class dialogue_state
{
	private:

	public:
		std::string talking_to_name_;
		registry& registry_;
		std::map<std::string,std::string> raw_text_nodes_;

		dialogue_state(registry& registry) : talking_to_name_{""}, registry_{registry} {}
		~dialogue_state();

		void advance();
		void create_node(std::string title,std::string fulltext);
		std::string get_current_text();

};
#endif