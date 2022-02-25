#include "inventory_ui.hpp"  
#include <libtcod.h>

#include "registry.hpp"
#include "util_functions.h"
#include "rect_transform.hpp"
#include "inventory.hpp"
	
inventory_ui::~inventory_ui()
{
	
}

void inventory_ui::draw(TCODConsole* console)
{
	console->setDefaultBackground(TCOD_black);
	console->setDefaultForeground(TCOD_white);
	// print dialogue feed
	console->rect(rect_transform_.position.x,rect_transform_.position.y,
		rect_transform_.size.x,rect_transform_.size.y,true,TCOD_BKGND_OVERLAY);
	console->printFrame(
		rect_transform_.position.x,rect_transform_.position.y,
		rect_transform_.size.x,rect_transform_.size.y,false,
		TCOD_BKGND_OVERLAY,
		"inventory");
	

	std::string inventory_text;
	for (auto &&item : inventory_.get_items())
	{
		inventory_text.append(item.name+ "\n");
	}
	
	
	 console->printRect(
	 	rect_transform_.position.x+1,rect_transform_.position.y+1,
	 	rect_transform_.size.x-2,rect_transform_.size.y-2,
	 	inventory_text.c_str());
}