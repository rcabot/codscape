#ifndef DIALOGUE_STATE_MACHINE_H
#define DIALOGUE_STATE_MACHINE_H
#pragma once
#include <string>

class registry;

class dialogue_state
{
	private:

	public:
		std::wstring talking_to_name_;
		registry& registry_;

		dialogue_state(registry& registry) : talking_to_name_{L""}, registry_{registry} {}
		~dialogue_state();

		void advance();

};
#endif