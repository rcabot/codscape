#include <string>
#include <vector>
#pragma once
	
class inventory  
{
	struct item{
		item(std::string name) : name{name} {}
		std::string name;
	};
	private:
		std::vector<item> items;

	public:

		inventory();
		~inventory();
		const std::vector<item>& get_items() const;

};