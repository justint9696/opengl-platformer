#include "level/editor.h"

#include "game/input.h"

void level_editor_init(leveleditor_t *self) {
    memset(self, 0, sizeof(leveleditor_t));
    self->current_tile = -1;
    self->curent_entity = -1;
}

void level_editor_update(leveleditor_t *self, world_t *world) {
    /* if (mouse_held(SDL_BUTTON_LEFT, 0) && self->current_tile == -1 */
    /*     && self->curent_entity == -1) { */
    /*     // TODO: check if there is an entity or tile under mouse */
    /* } */
}
