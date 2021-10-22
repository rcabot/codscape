//#ifndef DIALOGUE_INITIATOR_H
//#define DIALOGUE_INITIATOR_H
#pragma once

#include <string>
	
class dialogue_initiator  
{
	private:

	public:

		dialogue_initiator() {}
		~dialogue_initiator() = default;
		void start_dialogue_with(const std::wstring& person_name);

};
//#endif