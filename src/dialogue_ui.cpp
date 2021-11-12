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
	// todo: chache conversion on load
	console->printFrame(
		rect_transform_.position.x,rect_transform_.position.y,
		rect_transform_.size.x,rect_transform_.size.y,false,
		TCOD_BKGND_OVERLAY,
		util::ws2s(registry_.dialogue_state_.talking_to_name_).c_str());
}