#include <chrono>
#include <libtcod.h>
#include <SDL.h>

#include "game.h"
#include "key_event.h"
#include "text_input.h"


int main(int argc, char* argv[])
{
    auto g = std::make_unique<game>();
    text_input dev_console([&](auto command) {g->handle_command(command); });
	
    TCODConsole::setCustomFont("resources/polyducks_12x12.png", TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW, 16,16);

    const int reference_resolution { 20 };
    const auto font_resolution{ 12 };
    const Vector2 resolution{ 1152,648 };
	
    TCODConsole::initRoot(resolution.x / font_resolution, resolution.y / font_resolution, "tcodscape", true, TCOD_RENDERER_SDL);

    TCODSystem::setRenderer(TCOD_RENDERER_SDL2);
    TCODSystem::forceFullscreenResolution(1920,1080);

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();
	
    g->init(TCODConsole::root);
    auto quit = false;
    do
    {

        // Handle Timing
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_time{ tp2 - tp1 };
        tp1 = tp2;
        const auto f_elapsed_time{ elapsed_time.count() };

    	// update the game
        g->update(f_elapsed_time);

    	// draw the game onto the console
        g->draw(TCODConsole::root);
        dev_console.draw(TCODConsole::root);
        TCODConsole::flush();

    	// process input events
        SDL_Event event;
        std::vector<key_event> sdl_key_events;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
	            switch (event.key.keysym.sym)
	            {
                case SDLK_BACKQUOTE:
                    dev_console.set_active(true);
                    break;
                default: 
                    break;
	            }
                
            default:
                break;
            }
            g->process_sdl_event(event);
            dev_console.process_sdl_event(event);
        }

    	// process keyboard state
        const auto* keyboard_state{ SDL_GetKeyboardState(nullptr) };
        g->process_keyboard_state(keyboard_state);
    	
    } while (!TCODConsole::isWindowClosed() && !quit);
    TCOD_quit();
    return 0;
}
