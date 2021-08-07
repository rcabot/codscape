#pragma once
#include <string>
#include <utility>
#include "Person.h"

class text_input
{
	std::string current_value_;
	bool active_ = false;
	std::function<void(std::string)> on_submit_text_;
public:
	explicit text_input(std::function<void(std::string)> on_submit) : on_submit_text_{ std::move(on_submit) } {}
	void process_sdl_event(const SDL_Event& sdl_event);
	void draw(TCODConsole* console) const;
	void set_active(bool active);
};

