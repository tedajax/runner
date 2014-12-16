#include "controllercomponent.h"
#include "config.h"

ControllerComponent* controller_component_new(char* config, char* section) {
    ControllerComponent* self = (ControllerComponent*)calloc(1, sizeof(ControllerComponent));

    component_init((Component*)self, COMPONENT_CONTROLLER);

    Ini* cfg = config_get(config);

    self->moveSpeed = ini_get_float(cfg, section, "movement_speed");
    
    Vec2 sourceOffset = ini_get_vec2(cfg, section, "bullet_source_offset");
    char* sourceName = ini_get_string(cfg, section, "bullet_source");
    bullet_source_init(&self->bulletSource, &sourceOffset, config, sourceName);

    return self;
}