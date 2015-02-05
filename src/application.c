#include "application.h"
#include "prefab.h"

int app_run(int argc, char* argv[]) {
    App self;
    _app_init(&self);

    IF_DEBUG(bool appInit = )_app_initialize(&self);
    ASSERT(appInit, "Application failed to initialize");

    game_init(&self.game);
    prefab_system_init(self.game.entityManager, "assets/prefabs");

    game_start(&self.game);

    while (!self.shouldQuit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            _app_handle_event(&self, &event);
        }

        _app_update(&self);
        _app_render(&self);
        _app_frame_end(&self);
    }

    game_quit(&self.game);

    profiler_dump_log();

    _app_terminate(&self);

    return 0;
}

void _app_handle_event(App* self, SDL_Event* event) {
    switch (event->type) {
        case SDL_QUIT:
            self->shouldQuit = true;
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            input_handle_event(&event->key);
            break;
    }
}

void _app_update(App* self) {
    game_time_update(&globals.time);

    if (input_key_down(SDL_SCANCODE_ESCAPE)) {
        self->shouldQuit = true;
    }

    game_update(&self->game);

    input_update();
}

void _app_render(App* self) {
    SDL_SetRenderDrawColor(globals.renderer, 31, 31, 31, 255);
    SDL_RenderClear(globals.renderer);

    game_render(&self->game);

    SDL_RenderPresent(globals.renderer);
}

void _app_frame_end(App* self) {
    game_frame_end(&self->game);
}

void app_quit(App* self) {
    ASSERT(self, "NULL application structure passed to app_quit");

    self->shouldQuit = true;
}

bool _app_initialize(App* self) {
    config_init();
    config_load("game.ini");
    config_load("particles.ini");
    config_load("player.ini");

    globals_init();
    globals_config("game.ini");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("SDL", SDL_GetError());
        return false;
    }

    if (TTF_Init() < 0) {
        log_error("SDL_TTF", TTF_GetError());
        return false;
    }

    u32 windowFlags = SDL_WINDOW_SHOWN;
    if (globals.screen.fullscreen) {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }

    globals.window = window_create("Runner",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        globals.screen.width,
        globals.screen.height,
        windowFlags);

    if (!globals.window) {
        return false;
    }

    globals.renderer = SDL_CreateRenderer(globals.window, -1, SDL_RENDERER_ACCELERATED);
    if (!globals.renderer) {
        log_error("SDL", SDL_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(globals.renderer, globals.world.width, globals.world.height);

    self->shouldQuit = false;

    input_initialize();

    profiler_init(config_try_get_bool(config_get("game.ini"), "game", "enable_profiling", true));
    globals.time.on_second = config_reload_all;

    return true;
}

void _app_terminate(App* self) {
    profiler_terminate();

    prefab_system_terminate();
    config_terminate();

    SDL_DestroyWindow(globals.window);
    SDL_DestroyRenderer(globals.renderer);

    TTF_Quit();
    SDL_Quit();
}

void  _app_init(App* self) {
    self->shouldQuit = false;
}

void _app_print_fps() {
    printf("FPS: %d\n", globals.time.fps);
}