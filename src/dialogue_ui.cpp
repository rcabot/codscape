#include "dialogue_ui.hpp"
#include <libtcod.h>
#include "registry.hpp"
#include "util_functions.h"
#include "rect_transform.hpp"
dialogue_ui::~dialogue_ui()
{
	
}

void dialogue_ui::draw(TCODConsole* console)
{
	if(registry_.dialogue_state_.talking_to_name_==L"") return;
	;
	console->setDefaultBackground(TCOD_black);
	console->setDefaultForeground(TCOD_white);
	// todo: cache conversion on load
	console->rect(rect_transform_.position.x,rect_transform_.position.y,
		rect_transform_.size.x,rect_transform_.size.y,true,TCOD_BKGND_OVERLAY);
	console->printFrame(
		rect_transform_.position.x,rect_transform_.position.y,
		rect_transform_.size.x,rect_transform_.size.y,false,
		TCOD_BKGND_OVERLAY,
		util::ws2s(registry_.dialogue_state_.talking_to_name_).c_str());
	console->printRect(
		rect_transform_.position.x+1,rect_transform_.position.y+1,
		rect_transform_.size.x-2,rect_transform_.size.y-2,
		"salutations!");
}