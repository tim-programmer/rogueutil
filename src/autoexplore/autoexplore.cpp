#include <SDL3/SDL_rect.h>
#include <sstream>
#include <string>

#include <argparse/argparse.hpp>

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "base_map.h"
#include <format>
#include <print>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

constexpr std::string org_string{ "com.rogueutil" };
constexpr std::string app_string{ "autoexplore" };

// struct autoexplore_appstate
// {
//     rogueutil::DungeonMapPtr dungeon_map;
// };
//
// autoexplore_appstate global_appstate;

rogueutil::DungeonMap dungeon_map{ 80, 50 };

constexpr float CHAR_WIDTH{ 12.0f };
constexpr float CHAR_HEIGHT{ 12.0f };

const std::vector<SDL_FRect> tile_type_src{
    {  0 * CHAR_WIDTH, 0 * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT },       // TILE_VOID
    { 14 * CHAR_WIDTH, 2 * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT },       // TILE_FLOOR
    {  3 * CHAR_WIDTH, 2 * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT },       // TILE_WALL
};

void print_map(rogueutil::DungeonMap& dmap)
{
    std::stringstream row;
    for(auto&& [coord, tile] : dmap.cells()) {
        // std::print("{},{} {}\n", coord.x, coord.y, tile);
        row << "*";
        if(coord.x == dmap.width() - 1) {
            std::cout << row.str() << std::endl;
            row.str(std::string());
        }
    }
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface *surface = NULL;
    char *png_path = NULL;

    std::string app_id = org_string + app_string;
    SDL_SetAppMetadata(app_string.c_str(), "2026.0.1", app_id.c_str());

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(app_string.c_str(), WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    /* Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
       engines refer to these as "sprites." We'll do a static texture (upload once, draw many
       times) with data from a png file. */

    std::cerr << "Base Path:        \"" << SDL_GetBasePath() << "\"" << std::endl;
    std::cerr << "Current Path:     \"" << SDL_GetCurrentDirectory() << "\"" << std::endl;
    std::cerr << "Preferences Path: \"" << SDL_GetPrefPath(org_string.c_str(), app_string.c_str()) << "\"" << std::endl;

    /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
       Load a .png into a surface, move it to a texture from there. */
    SDL_asprintf(&png_path, "%s/assets/Alloy_curses_12x12.png", SDL_GetCurrentDirectory());  /* allocate a string of the full file path */
    surface = SDL_LoadPNG(png_path);
    if (!surface) {
        SDL_Log("Couldn't load png: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(png_path);  /* done with this, the file is loaded. */

    texture_width = surface->w;
    texture_height = surface->h;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */

    // global_appstate.dungeon_map = std::make_shared<rogueutil::DungeonMap>(80, 50, 0);
    // *appstate = &global_appstate;
    //

    // dungeon_map.generate();
    for(auto&& [coord, tile] : dungeon_map.cells()) {
        tile = rogueutil::DungeonMap::TILE_FLOOR;
    }

    for(int x = 0; x != dungeon_map.width(); ++x) {
        dungeon_map.setTileType(x, 0, rogueutil::DungeonMap::TILE_WALL);
        dungeon_map.setTileType(x, dungeon_map.height() - 1, rogueutil::DungeonMap::TILE_WALL);
    }

    for(int y = 0; y != dungeon_map.height(); ++y) {
        dungeon_map.setTileType(0, y, rogueutil::DungeonMap::TILE_WALL);
        dungeon_map.setTileType(dungeon_map.width() - 1, y, rogueutil::DungeonMap::TILE_WALL);
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    // autoexplore_appstate* local_appstate = static_cast<autoexplore_appstate*>(appstate);

    // SDL_FRect dst_rect;
    // const Uint64 now = SDL_GetTicks();

    /* we'll have some textures move around over a few seconds. */
    // const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    // const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    /* Just draw the static texture a few times. You can think of it like a
       stamp, there isn't a limit to the number of times you can draw with it. */

    for(auto&& [coord, tile] : dungeon_map.cells()) {
        SDL_FRect dst_rect;
        dst_rect.x = static_cast<float>(coord.x) * CHAR_WIDTH;
        dst_rect.y = static_cast<float>(coord.y) * CHAR_HEIGHT;
        dst_rect.w = CHAR_WIDTH;
        dst_rect.h = CHAR_HEIGHT;
        SDL_RenderTexture(renderer, texture, &tile_type_src[tile & rogueutil::DungeonMap::TILE_TYPE_MASK], &dst_rect);
    }

    // Draw player & mobs

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);
    /* SDL will clean up the window/renderer for us. */
}
