#pragma once

#include "entity/entity.h"
#include "game/camera.h"

typedef struct world_s {
    camera_s camera;
    entity_s *player;
    entity_s *entities;
} world_s;

void world_init(world_s *);
void world_destroy(world_s *);
void world_update(world_s *);
void world_render(world_s *);
void world_tick(world_s *);
