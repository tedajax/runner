#include "bulletcontrollercomponent.h"

BulletControllerComponent* bullet_controller_component_new(Entity entity, BulletConfig* config) {
    BulletControllerComponent* self = (BulletControllerComponent*)calloc(1,
        sizeof(BulletControllerComponent));

    component_init((Component*)self, COMPONENT_BULLET_CONTROLLER, sizeof(BulletControllerComponent), entity);

    self->config = *config;
    self->speed = self->config.speed;
    self->angle = self->config.angle;
    self->baseAngle = self->config.baseAngle;
    self->lifeTimer = self->config.lifetime;
    self->destroyFlag = false;

    dynf32_copy(&self->config.speed, &self->speed);
    dynf32_copy(&self->config.angle, &self->angle);

    dynf32_start_tween(&self->speed, &globals.tweens);
    dynf32_start_tween(&self->angle, &globals.tweens);

    return self;
}

BulletControllerComponent* bullet_controller_component_deserialize(Config* config, const char* table) {
    BulletConfig bulletConfig;
    dynf32 zero;
    dynf32_zero(&zero);
    bulletConfig.speed = CONFIG_TRY_GET(dynf32)(config, table, "speed", zero);
    bulletConfig.angle = CONFIG_TRY_GET(dynf32)(config, table, "angle", zero);
    bulletConfig.baseAngle = CONFIG_TRY_GET(float)(config, table, "base_angle", 0.f);
    bulletConfig.lifetime = CONFIG_TRY_GET(float)(config, table, "lifetime", 99999.f);
    bulletConfig.damage = CONFIG_TRY_GET(int)(config, table, "damage", 0);
    return bullet_controller_component_new(0, &bulletConfig);
}

void bullet_controller_on_collision_enter(Component* component, Message message) {
    BulletControllerComponent* self = (BulletControllerComponent*)component;
    self->destroyFlag = true;
}