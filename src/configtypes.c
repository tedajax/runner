#include "configtypes.h"
#include "collider.h"
#include "config.h"

void collider_config_deserialize(TypeConfig* super, Config* config, char* table) {
    ColliderConfig* self = (ColliderConfig*)super;

    self->layer = collider_layer_parse(config_get_string(config, table, "layer"));
    self->offset = config_get_Vec2(config, table, "offset");

    char* volumeStr = config_try_get_string(config, table, "type", NULL);
    self->type = BOUNDING_VOLUME_AA_BOX;
    if (volumeStr) {
        self->type = bounding_volume_type_parse(volumeStr);
    }

    switch (self->type) {
    	default:
    	case BOUNDING_VOLUME_AA_BOX:
            self->width = config_get_float(config, table, "width");
            self->height = config_get_float(config, table, "height");
    		break;

    	case BOUNDING_VOLUME_O_BOX:
            self->width = config_get_float(config, table, "width");
            self->height = config_get_float(config, table, "height");
            self->orientation = config_try_get_float(config, table, "orientation", 0.f);
    		break;

    	case BOUNDING_VOLUME_CIRCLE:
            self->radius = config_get_float(config, table, "radius");
    		break;
    }
}

ColliderLayer collider_layer_parse(char* str) {
    if (STR_ENUM_CMP(str, COLLIDER_LAYER_PLAYER)) {
        return COLLIDER_LAYER_PLAYER;
    } else if (STR_ENUM_CMP(str, COLLIDER_LAYER_ENEMY)) {
        return COLLIDER_LAYER_ENEMY;
    } else if (STR_ENUM_CMP(str, COLLIDER_LAYER_ENVIRONMENT)) {
        return COLLIDER_LAYER_ENVIRONMENT;
    } else if (STR_ENUM_CMP(str, COLLIDER_LAYER_PLAYER_PROJECTILE)) {
        return COLLIDER_LAYER_PLAYER_PROJECTILE;
    } else if (STR_ENUM_CMP(str, COLLIDER_LAYER_LAST)) {
        return COLLIDER_LAYER_LAST;
    } else {
        ASSERT(false, "Unable to parse ColliderLayer enum.");
        return 0;
    }
}

void bullet_source_config_deserialize(TypeConfig* super, Config* config, char* table) {
    BulletSourceConfig* self = (BulletSourceConfig*)super;

    self->offset = config_get_Vec2(config, table, "offset");
    self->count = config_get_int(config, table, "count");
    
    self->burstCount = config_try_get_int(config, table, "burst_count", 1);
    dynf32 defaultBurstDelay = { 0.f, DYN_F32_VALUE, NULL, NULL };
    self->burstDelay = config_try_get_dynf32(config, table, "burst_delay", defaultBurstDelay);
    self->spread = config_get_dynf32(config, table, "spread");
    self->lifetime = config_get_dynf32(config, table, "lifetime");
    self->speed = config_get_dynf32(config, table, "speed");
    self->angle = config_get_dynf32(config, table, "angle");
    self->fireDelay = config_get_dynf32(config, table, "fire_delay");
    self->startAngle = config_get_dynf32(config, table, "start_angle");
    self->damage = config_get_dynf32(config, table, "damage");
    
    self->textureName = config_get_string(config, table, "texture_name");

    dynf32_restart(&self->burstDelay, &globals.tweens);
    dynf32_restart(&self->spread, &globals.tweens);
    dynf32_restart(&self->lifetime, &globals.tweens);
    dynf32_restart(&self->speed, &globals.tweens);
    dynf32_restart(&self->angle, &globals.tweens);
    dynf32_restart(&self->fireDelay, &globals.tweens);
    dynf32_restart(&self->startAngle, &globals.tweens);
    dynf32_restart(&self->damage, &globals.tweens);

    self->colliderConfig = config_get_ColliderConfig(config, table, "collider");
}

void tween_config_deserialize(TypeConfig* super, Config* config, char* table) {
    TweenConfig* self = (TweenConfig*)super;

    char* tweenFuncName = config_get_string(config, table, "function");
    self->function = tween_parse(tweenFuncName);
    self->start = config_get_float(config, table, "start");
    self->end = config_get_float(config, table, "end");
    self->duration = config_get_float(config, table, "duration");
    self->timescale = config_try_get_float(config, table, "timescale", 1.f);
    self->loops = config_try_get_int(config, table, "loops", TWEEN_LOOP_INFINITE);
}

void level_config_deserialize(TypeConfig* super, Config* config, char* table) {
    LevelConfig* self = (LevelConfig*)super;

    self->remove_this_later = 0;
}