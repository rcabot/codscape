#pragma once
#include <codecvt>
#include <locale>
#include <filesystem>

#include "Map.h"
#include "Person.h"
#include "Vector2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL.h>

#include "JSON.h"
#include "util_functions.h"

template<typename T>
using list = std::vector<std::unique_ptr<T>>;

constexpr inline auto operator "" _(const char* p, const size_t size) -> unsigned int { return util::hash(p,size); }
class game
{
public:
	game() : player_pos_{ 0,0 },
		time_to_next_input_{ 0 },
		input_interval_{ .02f },
		maps_{},
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
				const auto terrain{ maps_[current_map_]->get_terrain_at(Vector2(x,y) + player_offset) };
				
				console->putCharEx(x, y, terrain.character_, terrain.fg_color_,terrain.bg_color_);
			}
		}


		//draw player
		draw_sprite(player_pos_onscreen_.x, player_pos_onscreen_.y, player_sprite_.get(), console);
		for (const auto& person : people_)
		{
			if (person == nullptr) continue;
			const auto person_pos{ person->get_pos() };
			if (is_onscreen(person_pos, person->get_location()))
			{
				//auto screen_space_pos{ person_pos - player_offset };
				draw_sprite(person_pos.x - player_offset.x, person_pos.y - player_offset.y, person->get_sprite(), console);
			}
		}

	}

	void process_sdl_event(const SDL_Event& sdl_event)
	{
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
				add_new_person_at(player_pos_, person::generate_random_name());
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
		if (time_to_next_input_ <= 0.f)
		{
			auto& current_map = *maps_[current_map_];
			if (keyboard_state[SDL_SCANCODE_LEFT] && player_can_move_to(Vector2(player_pos_.x - 1, player_pos_.y), current_map))
				player_pos_.x -= 1;
			if (keyboard_state[SDL_SCANCODE_RIGHT] && player_can_move_to(Vector2(player_pos_.x + 1, player_pos_.y), current_map))
				player_pos_.x += 1;
			if (keyboard_state[SDL_SCANCODE_UP] && player_can_move_to(Vector2(player_pos_.x, player_pos_.y - 1), current_map))
				player_pos_.y -= 1;
			if (keyboard_state[SDL_SCANCODE_DOWN] && player_can_move_to(Vector2(player_pos_.x, player_pos_.y + 1), current_map))
				player_pos_.y += 1;

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
		make_new_gameobject<Map>("Overworld");
		make_new_gameobject<Map>("Underworld");

		player_sprite_ = person::generate_random_sprite();
		player_pos_onscreen_ = { console->getWidth() / 2, console->getHeight() / 2 };
		player_pos_ = maps_[0]->get_first_position_where_terrain_is([](const Terrain& t) { return !t.is_mountain_ && !t.is_water_; }, CACHE_DIMENSION);

		load_game_setup();
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
				add_new_person_at(player_pos_, terms_list[2]);
				break;
			case "testperson"_:
				make_new_gameobject<person>(L"test person", maps_[current_map_].get(), player_pos_);
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
	std::unique_ptr<olcSprite> player_sprite_;
	float time_to_next_input_;
	float input_interval_;
	list<Map> maps_;
	Vector2 player_pos_onscreen_;
	list<person> people_;
	int current_map_;
	
	

	bool object_blocks_cell(const Vector2& pos, const Map& location) const
	{
		for (const auto& person : people_)
		{
			if (person->get_location() == &location
				&& pos.x == person->get_pos().x
				&& pos.y == person->get_pos().y) return true;
		}
		return false;
	}

	bool player_can_move_to(const Vector2& pos, const Map& location) const
	{
		const auto t{ maps_[current_map_]->get_terrain_at(pos) };
		return !t.is_mountain_ && !t.is_water_ && !t.is_void_ && !object_blocks_cell(pos, location);
	}

	void add_new_person_at(const Vector2& position, const std::wstring& name)
	{
		people_.emplace_back(std::make_unique<person>(name,maps_[current_map_].get(), position));
	}

	void add_new_person_at(const Vector2& pos, const std::string& name)
	{
		std::wstringstream converter;
		converter << name.c_str();
		add_new_person_at(pos, converter.str());
	}

	template<typename T>
	void emplace_gameobject(std::unique_ptr<T> gameobject)
	{
		static_assert(false,"This game object isn't supported yet.");
	}

	template<typename T, typename...Params>
	void make_new_gameobject(Params&&... args)
	{
		auto ptr{ std::make_unique<T>(args...) };
		emplace_gameobject(std::move(ptr));
	}

	bool is_onscreen(const Vector2& pos, Map* map) const
	{
		if (map != maps_[current_map_].get()) return false;
		return true;
	}

	void save_game_setup(const std::filesystem::path& path_to_game_setup_config = "./resources/game_setup.json")
	{
		std::wfstream s;
		s.open(path_to_game_setup_config, std::wfstream::out | std::wfstream::trunc);
		s << R"({"people":[)";
		for (const auto& person : people_)
		{
			s << person->to_json_string();
			if (person != people_.back()) s << "," << std::endl;
		}
		s << "]}";
		util::log_format("saved game state with", people_.size(),"people", "to", path_to_game_setup_config);

	}

	void load_game_setup(const std::filesystem::path& path_to_game_setup_config = "./resources/game_setup.json")
	{
		std::fstream s;
		s.open(path_to_game_setup_config, std::fstream::in);
		if (!s)
		{
			// setup file did not exist! creating one...
			save_game_setup();
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
				add_new_person_at(
					Vector2(
						json_value_as_object.at(L"x")->AsNumber(),
						json_value_as_object.at(L"y")->AsNumber()),
					json_value_as_object.at(L"name")->AsString());
			}
		}
	}

	
};


template <>
void game::emplace_gameobject<person>(std::unique_ptr<person> gameobject);

template <>
void game::emplace_gameobject<Map>(std::unique_ptr<Map> gameobject);