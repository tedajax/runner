#include "debughud.h"
#include "snprintf.h"

int debug_hud_watch_internal_to_int(DebugHudWatch* self);
SDL_Color debug_hud_watch_internal_get_color(DebugHudWatch* self);

int debug_hud_watch_internal_to_int(DebugHudWatch* self) {
    switch (self->type) {
        default:
        case WATCH_TYPE_INT:
            return *self->value.i;

        case WATCH_TYPE_FLOAT:
            return (int)*self->value.f;

        case WATCH_TYPE_BOOL:
            return (*self->value.b) ? 1 : 0;
    }
}

SDL_Color debug_hud_watch_internal_get_color(DebugHudWatch* self) {
    const SDL_Color white = { 255, 255, 255, 255 };
    const SDL_Color green = { 0, 255, 0, 255 };
    const SDL_Color yellow = { 255, 255, 0, 255 };
    const SDL_Color red = { 255, 0, 0, 255 };
    
    int intVal = debug_hud_watch_internal_to_int(self);

    switch (self->warningType) {
        default:
        case DEBUG_HUD_WATCH_WARNING_NONE:
            return white;

        case DEBUG_HUD_WATCH_WARNING_ADDITIVE:
            if (intVal >= self->errorThreshold) {
                return red;
            } else if (intVal >= self->warningThreshold) {
                return yellow;
            } else {
                return green;
            }

        case DEBUG_HUD_WATCH_WARNING_SUBTRACTIVE:
            if (intVal <= self->errorThreshold) {
                return red;
            } else if (intVal <= self->warningThreshold) {
                return yellow;
            } else {
                return green;
            }
    }
}

void debug_hud_watch_build_string(DebugHudWatch* self, char* dest, u32 max) {
    switch (self->type) {
        case WATCH_TYPE_INT:
            snprintf(dest, max, "%s : %d", self->displayName, *self->value.i);
            break;

        case WATCH_TYPE_FLOAT:
            snprintf(dest, max, "%s : %0.2f", self->displayName, *self->value.f);
            break;

        case WATCH_TYPE_BOOL:
            snprintf(dest, max, "%s : %s", self->displayName, (*self->value.b) ? "true" : "false");
            break;

        default: break;
    }   
}

void debug_hud_init(DebugHud* self, const char* fontName, int ptSize) {
    TTF_Font* font = TTF_OpenFont(fontName, ptSize);
    if (!font) {
        log_error_format("Debug HUD", "Error loading font %s, %s", fontName, TTF_GetError());
        return;
    }

    self->debugFont = font;
    self->ptSize = ptSize;

    self->capacity = 8;
    self->count = 0;
    self->watches = CALLOC(self->capacity, DebugHudWatch);
}

void debug_hud_free(DebugHud* self) {
    for (u32 i = 0; i < self->count; ++i) {
        DebugHudWatch* watch = &self->watches[self->count];
        if (watch->texture) {
            SDL_DestroyTexture(watch->texture);
            watch->texture = NULL;
        }
    }

    free(self->watches);
    TTF_CloseFont(self->debugFont);
}

DebugHudWatch* debug_hud_add_watch(DebugHud* self, char* name, DebugHudWatchType type, void* value) {
    if (self->count >= self->capacity) {
        self->capacity <<= 1;
        self->watches = (DebugHudWatch*)realloc(self->watches, self->capacity * sizeof(DebugHudWatch));
    }

    DebugHudWatch* watch = &self->watches[self->count];
    watch->texture = NULL;
    watch->displayName = name;
    watch->type = type;
    watch->warningType = DEBUG_HUD_WATCH_WARNING_NONE;
    watch->warningThreshold = 0;
    watch->errorThreshold = 0;
    
    switch (watch->type) {
        case WATCH_TYPE_INT:
            watch->value.i = (int*)value;
            break;

        case WATCH_TYPE_FLOAT:
            watch->value.f = (float*)value;
            break;

        case WATCH_TYPE_BOOL:
            watch->value.b = (bool*)value;
            break;

        default: break;
    }

    ++self->count;

    return watch;
}

void debug_hud_watch_set_warnings(DebugHudWatch* self, bool additiveWarnings, int warning, int error) {
    self->warningType = (additiveWarnings) ? DEBUG_HUD_WATCH_WARNING_ADDITIVE : DEBUG_HUD_WATCH_WARNING_SUBTRACTIVE;
    self->warningThreshold = warning;
    self->errorThreshold = error;
}

void debug_hud_update_surfaces(DebugHud* self, SDL_Renderer* renderer) {
    for (u32 i = 0; i < self->count; ++i) {
        DebugHudWatch* watch = &self->watches[i];

        if (watch->texture) {
            SDL_DestroyTexture(watch->texture);
            watch->texture = NULL;
        }

        char watchOut[128];
        debug_hud_watch_build_string(watch, watchOut, 128);

        SDL_Color color = debug_hud_watch_internal_get_color(watch);
        SDL_Surface* surface = TTF_RenderText_Solid(self->debugFont, watchOut, color);
        watch->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void debug_hud_render(DebugHud* self, SDL_Renderer* renderer, int x, int y) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
    SDL_Rect bg;
    bg.x = x;
    bg.y = y;
    bg.w = 200;
    bg.h = self->count * (self->ptSize + 2) + 10;
    SDL_RenderFillRect(renderer, &bg);

    int wx = x + 5;
    int wy = y + 5;
    
    for (u32 i = 0; i < self->count; ++i) {
        DebugHudWatch* watch = &self->watches[i];
        
        if (!watch->texture) {
            continue;
        }
        
        SDL_Rect r;
        r.x = wx;
        r.y = wy;
        SDL_QueryTexture(watch->texture, NULL, NULL, &r.w, &r.h);
        SDL_RenderCopy(renderer, watch->texture, NULL, &r);

        wy += self->ptSize + 2;
    }
}

void debug_hud_dump(DebugHud* self, FILE* f) {
    for (u32 i = 0; i < self->count; ++i) {
        DebugHudWatch* watch = &self->watches[i];

        char watchOut[128];
        debug_hud_watch_build_string(watch, watchOut, 128);

        fprintf(f, "%s\n", watchOut);
    }
}