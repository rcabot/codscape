#include "text_input.h"

#include <SDL.h>

void text_input::process_sdl_event(const SDL_Event& sdl_event)
{
	if (!active_) return;
	if (sdl_event.type == SDL_TEXTINPUT)
	{
		current_value_ += sdl_event.text.text;
		current_value_.erase(std::remove(current_value_.begin(), current_value_.end(), '`'), current_value_.end());
	}
	else if(sdl_event.type==SDL_KEYDOWN)
	{
		switch (sdl_event.key.keysym.sym)
		{
		case SDLK_RETURN:
			set_active(false);
			on_submit_text_(current_value_);
			current_value_.clear();
			break;
		case SDLK_BACKSPACE:
			if(!current_value_.empty())
				current_value_.pop_back();
			break;
		default: 
			break;
		}
	}
}

void text_input::draw(TCODConsole* console) const
{
	if (!active_) return;
	console->setDefaultForeground(TCOD_white);
	console->setDefaultBackground(TCOD_black);
	auto i = 0;
	console->printf(0, 0, (current_value_+"_").c_str());
}

void text_input::set_active(const bool active)
{
	active_ = active;
	if(active)
	{
		SDL_StartTextInput();
	}
	else
	{
		SDL_StopTextInput();
	}
}
