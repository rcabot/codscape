#include "dialogue_ui.hpp"
#include <libtcod.h>
#include "registry.hpp"
#include "util_functions.h"
#include "rect_transform.hpp"
#include "dialogue_state.hpp"
#include <format>
dialogue_ui::~dialogue_ui()
{
	
}

void dialogue_ui::draw(TCODConsole* console)
{
	if(registry_.dialogue_state_.get_talking_to_name()=="") return;

	console->setDefaultBackground(TCOD_black);
	console->setDefaultForeground(TCOD_white);
	// print dialogue feed
	console->rect(rect_transform_.position.x,rect_transform_.position.y,
		rect_transform_.size.x,rect_transform_.size.y,true,TCOD_BKGND_OVERLAY);
	console->printFrame(
		rect_transform_.position.x,rect_transform_.position.y,
		rect_transform_.size.x,rect_transform_.size.y,false,
		TCOD_BKGND_OVERLAY,
		registry_.dialogue_state_.get_talking_to_name().c_str());
	
	// dialogue text
	std::string dialogue_text{state_.get_current_text()};

	// print options
	int op_num = 1;
	for (auto&& option : registry_.dialogue_state_.get_options())
	{
		auto option_message{util::string_format("%s (%d)",option.text.c_str(),op_num)};
		dialogue_text.append("\n");
		dialogue_text.append(option_message);
		op_num++;
	}
	
	console->printRect(
		rect_transform_.position.x+1,rect_transform_.position.y+1,
		rect_transform_.size.x-2,rect_transform_.size.y-2,
		dialogue_text.c_str());
}