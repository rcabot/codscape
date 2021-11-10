#include "dialogue_ui.hpp"
#include <libtcod.h>
#include "registry.hpp"
#include "util_functions.h"
dialogue_ui::~dialogue_ui()
{
	
}

void dialogue_ui::draw(TCODConsole* console)
{
	if(registry_.dialogue_state_.talking_to_name_==L"") return;
	console->setDefaultBackground(TCOD_black);
	console->setDefaultForeground(TCOD_white);
	console->printFrame(0,0,50,10,false,TCOD_BKGND_OVERLAY,util::ws2s(registry_.dialogue_state_.talking_to_name_).c_str());
}