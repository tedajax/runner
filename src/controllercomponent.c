#include "controllercomponent.h"
#include "config.h"

ControllerComponent* controller_component_new(Entity entity, Config* config, char* section) {
    ControllerComponent* self = (ControllerComponent*)calloc(1, sizeof(ControllerComponent));

    component_init((Component*)self, COMPONENT_CONTROLLER, sizeof(ControllerComponent), entity);

    self->moveSpeed = config_get_float(config, section, "movement_speed");

    //TODO get rid of these ini calls
    self->bulletSourceCount = ini_get_array_count(&config->data, section, "bullet_sources");
    self->bulletSources = CALLOC(self->bulletSourceCount, BulletSource);

    for (u32 i = 0; i < self->bulletSourceCount; ++i) {
        bullet_source_init(&self->bulletSources[i], config_get_BulletSourceConfig_at(config, section, "bullet_sources", i));
    }

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_CONTROLLER) {
    return (Component*)controller_component_new(0, config, (char*)table);
}