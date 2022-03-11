#include "inventory.hpp"  
	
inventory::inventory()
{
	items.emplace_back("item1");
	items.emplace_back("item2");
	items.emplace_back("item3");
}
	
inventory::~inventory()
{
	
}


const std::vector<inventory::item>& inventory::get_items() const 
{
    return items;
}


void inventory::add_item(const std::string& item_name) 
{
    items.emplace_back(item_name);
}