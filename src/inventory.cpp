#include "inventory.hpp"  
#include <algorithm>
	
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

void inventory::remove_item(const std::string& item_name) 
{
	// searching on name is bad, use an item id of some sort
	auto it = std::find_if(items.begin(),items.end(),[item_name](inventory::item i){
		return i.name==item_name;
	});
	items.erase(it);
}