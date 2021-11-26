#pragma once
#include <filesystem>

#include "Map.h"
#include "Person.h"
#include "Vector2.h"
#include "registry.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL.h>
#include "interactable.hpp"

#include "JSON.h"
#include "util_functions.h"


constexpr inline auto operator "" _(const char* p, const size_t size) -> unsigned int { return util::hash(p,size); }
class game
{
public:
	game() : 
		player_pos_{ 0,0 },
		time_to_next_input_{ 0 },
		input_interval_{ .02f },
		player_pos_onscreen_{},
		current_map_{ 0 }
	{
	}

	void update(const float f_elapsed_time)
	{
		time_to_next_input_ -= f_elapsed_time;
	}
	
	void draw(TCODConsole* console)
	{
		console->setDefaultBackground(TCOD_black);
		const Vector2 player_offset{ player_pos_.x - player_pos_onscreen_.x, player_pos_.y - player_pos_onscreen_.y };
		for (auto x = 0; x < console->getWidth(); x++)
		{
			for (auto y = 0; y < console->getHeight(); y++)
			{
				const auto terrain{ registry_.maps_[current_map_]->get_terrain_at(Vector2(x,y) + player_offset) };
				
				console->putCharEx(x, y, terrain.character_, terrain.fg_color_,terrain.bg_color_);
			}
		}


		//draw player
		draw_sprite(player_pos_onscreen_.x, player_pos_onscreen_.y, player_sprite_.get(), console);
		for (const auto& person :  registry_.people_)
		{
			if (person == nullptr) continue;
			const auto person_pos{ person->get_pos() };
			if (is_onscreen(person_pos, person->get_location()))
			{
				//auto screen_space_pos{ person_pos - player_offset };
				draw_sprite(person_pos.x - player_offset.x, person_pos.y - player_offset.y, person->get_sprite(), console);
			}
		}

		registry_.dialogue_ui_.draw(console);

	}

	void process_sdl_event(const SDL_Event& sdl_event)
	{
		auto current_state_info = registry_.player_state_machine_.get_current_state();
		auto player_interaction_radius{5.f};
		switch (sdl_event.type)
		{
		case SDL_KEYDOWN:
		{
			switch(sdl_event.key.keysym.sym)
			{
			case SDLK_F3:
				save_game_setup();
				break;
			case SDLK_F2:
				registry_.add_new_person_at(player_pos_, person::generate_random_name(),current_map_);
				break;
			case SDLK_SPACE:
				if(current_state_info.can_advance_dialogue_)
				{
					registry_.dialogue_state_.advance();
				}
				else if(current_state_info.can_interact_)
				{
					auto* nearest_interactable_object = registry_.try_get_nearest_interactable_object_in_radius(player_pos_, player_interaction_radius);
					if(nearest_interactable_object!=nullptr)
						nearest_interactable_object->interact();
				}
				break;
			default:
				break;
			}
			
		}
		default: 
			break;
		}
	}

	void process_keyboard_state(const Uint8* keyboard_state)
	{
		auto current_state_info = registry_.player_state_machine_.get_current_state();
		// todo: move this to a "mover" class
		if (time_to_next_input_ <= 0.f)
		{
			if(current_state_info.can_move_)
			{
				auto& current_map = * registry_.maps_[current_map_];
				if (keyboard_state[SDL_SCANCODE_LEFT] && player_can_move_to(Vector2(player_pos_.x - 1, player_pos_.y), current_map))
					player_pos_.x -= 1;
				if (keyboard_state[SDL_SCANCODE_RIGHT] && player_can_move_to(Vector2(player_pos_.x + 1, player_pos_.y), current_map))
					player_pos_.x += 1;
				if (keyboard_state[SDL_SCANCODE_UP] && player_can_move_to(Vector2(player_pos_.x, player_pos_.y - 1), current_map))
					player_pos_.y -= 1;
				if (keyboard_state[SDL_SCANCODE_DOWN] && player_can_move_to(Vector2(player_pos_.x, player_pos_.y + 1), current_map))
					player_pos_.y += 1;
			}


			time_to_next_input_ = input_interval_;
		}

	}

	static void draw_sprite(const int x, const int y, olcSprite* sprite, TCODConsole* console)
	{
		if (sprite == nullptr)
			return;

		for (auto i = 0; i < sprite->nWidth; i++)
		{
			for (auto j = 0; j < sprite->nHeight; j++)
			{
				if (sprite->GetGlyph(i, j) != L' ')
					console->putCharEx(x + i - sprite->x_pivot_, y + j - sprite->y_pivot_, sprite->GetGlyph(i, j), sprite->GetFColour(i, j), sprite->GetBColour(i, j));
			}
		}
	}
	void init(const TCODConsole* console)
	{
		registry_.add_map("Overworld");
		registry_.add_map("Underworld");

		player_sprite_ = person::generate_random_sprite();
		player_pos_onscreen_ = { console->getWidth() / 2, console->getHeight() / 2 };
		player_pos_ =  registry_.maps_[0]->get_first_position_where_terrain_is([](const Terrain& t) { return !t.is_mountain_ && !t.is_water_; }, CACHE_DIMENSION);

		registry_.dialogue_ui_transform_.size = {console->getWidth(),console->getHeight()/3};
		registry_.dialogue_ui_transform_.position = {0,console->getHeight()-registry_.dialogue_ui_transform_.size.y};
		// padding
		registry_.dialogue_ui_transform_.position.x += 1;
		registry_.dialogue_ui_transform_.position.y -= 1;
		registry_.dialogue_ui_transform_.size.x -= 2;
		registry_.dialogue_ui_transform_.size.y -= 1;
		load_game_setup();
		load_dialogue();
	}

	void handle_command(const std::string& command)
	{
		std::cout << command << "\n";
		
		auto terms_list{ util::split(command,' ') };
		if (terms_list.empty()) return;

		switch(util::hash(terms_list[0].c_str(),terms_list[0].size()))
		{
		case "save"_:
		case "savec"_:
		case "saveconf"_:
			save_game_setup();
			break;
		case "add"_:
			if (terms_list.size() < 2) return;
			switch (util::hash(terms_list[1].c_str(), terms_list[1].size()))
			{
			case "person"_:
				if (terms_list.size() < 3) return;
				registry_.add_new_person_at(player_pos_, terms_list[2], current_map_);
				break;
			case "testperson"_:
				registry_.add_new_person_at(player_pos_, "test person",current_map_);
				break;
			default: 
				break;
			}
			break;
		default: 
			break;
		}
	}

private:
	Vector2 player_pos_;
	int current_map_;

	std::unique_ptr<olcSprite> player_sprite_;
	float time_to_next_input_;
	float input_interval_;
	Vector2 player_pos_onscreen_;
	registry registry_;
	
	

	bool object_blocks_cell(const Vector2& pos, const Map& location) const
	{
		for (const auto& person :  registry_.people_)
		{
			if (person->get_location() == &location
				&& pos.x == person->get_pos().x
				&& pos.y == person->get_pos().y) return true;
		}
		return false;
	}

	bool player_can_move_to(const Vector2& pos, const Map& location) const
	{
		const auto t{  registry_.maps_[current_map_]->get_terrain_at(pos) };
		return !t.is_mountain_ && !t.is_water_ && !t.is_void_ && !object_blocks_cell(pos, location);
	}

	

	bool is_onscreen(const Vector2& pos, Map* map) const
	{
		if (map !=  registry_.maps_[current_map_].get()) return false;
		return true;
	}

	void save_game_setup(const std::filesystem::path& path_to_game_setup_config = "./resources/game_setup.json")
	{
		std::wfstream s;
		s.open(path_to_game_setup_config, std::wfstream::out | std::wfstream::trunc);
		s << R"({"people":[)";
		for (const auto& person :  registry_.people_)
		{
			s << person->to_json_string();
			if (person !=  registry_.people_.back()) s << "," << std::endl;
		}
		s << "]}";
		util::log_format("saved game state with",  registry_.people_.size(),"people", "to", path_to_game_setup_config);

	}

	void load_dialogue(const std::filesystem::path& path_to_dialogue_file = "./resources/dialogue.json")
	{
		std::fstream s;
		s.open(path_to_dialogue_file, std::fstream::in);
		if (!s)
		{
			std::cout << "dialogue file did not exist...\n";
			return;
		}
		std::stringstream json_file_buffer;
		json_file_buffer << s.rdbuf();
		const auto json_file_str{ json_file_buffer.str() };
		std::unique_ptr<JSONValue> file_as_json;
		file_as_json.reset(JSON::Parse(json_file_str.c_str()));

		if (file_as_json->IsArray())
		{
			auto nodes{ file_as_json->AsArray() };

			for (auto* json_value : nodes)
			{
				const auto& json_value_as_object{ json_value->AsObject() };
				const auto title {util::ws2s(json_value_as_object.at(L"title")->AsString())};
				const auto text {util::ws2s(json_value_as_object.at(L"body")->AsString())};
				registry_.dialogue_state_.create_node(title, text);
			}
		}
	}

	void load_game_setup(const std::filesystem::path& path_to_game_setup_config = "./resources/game_setup.json")
	{
		std::fstream s;
		s.open(path_to_game_setup_config, std::fstream::in);
		if (!s)
		{
			std::cout << "setup file did not exist! creating one...\n";
			save_game_setup();
			s.open(path_to_game_setup_config, std::fstream::in);
		}

		std::stringstream json_file_buffer;
		json_file_buffer << s.rdbuf();
		const auto json_file_str{ json_file_buffer.str() };
		std::unique_ptr<JSONValue> file_as_json;
		file_as_json.reset(JSON::Parse(json_file_str.c_str()));

		if (file_as_json->IsObject())
		{
			auto people_to_init{ file_as_json->AsObject().at(L"people")->AsArray() };

			for (auto* json_value : people_to_init)
			{
				const auto& json_value_as_object{ json_value->AsObject() };
				registry_.add_new_person_at(
					Vector2(
						json_value_as_object.at(L"x")->AsNumber(),
						json_value_as_object.at(L"y")->AsNumber()),
					util::ws2s(json_value_as_object.at(L"name")->AsString()),
					current_map_);
			}
		}
	}

	
};
