#include "ui/ui.h"

#include "graphics/color.h"
#include "graphics/font.h"
#include "graphics/renderer.h"
#include "graphics/vao.h"
#include "graphics/vbo.h"

#include <cglm/struct.h>
#include <string.h>

static ui_t ui;

static void render_text(vec2s pos, float scale, vec4s color, const char *text) {
    shader_uniform_3f(*ui.shader, "color", color.r, color.g, color.b);
    glActiveTexture(GL_TEXTURE0);
    vao_bind(&ui.text.vao);

    size_t len = strnlen(text, 64);
    for (size_t c = 0; c < len; c++) {
        size_t idx = text[c];
        const fontchar_t *ch = font_get_char(idx);

        float pos_x = pos.x + ch->bearing.x * scale,
              pos_y = pos.y - (ch->dim.y - ch->bearing.y) * scale;

        ivec2s dim = glms_ivec2_scale(ch->dim, scale);

        float vertices[6][4] = {
            {         pos_x, pos_y + dim.y, 0.0f, 0.0f },
            {         pos_x,         pos_y, 0.0f, 1.0f },
            { pos_x + dim.x,         pos_y, 1.0f, 1.0f },

            {         pos_x, pos_y + dim.y, 0.0f, 0.0f },
            { pos_x + dim.x,         pos_y, 1.0f, 1.0f },
            { pos_x + dim.x, pos_y + dim.y, 1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch->id);

        vbo_bind(&ui.text.vbo);
        vbo_buffer_data(vertices, sizeof(vertices));

        glDrawArrays(GL_TRIANGLES, 0, 6);

        pos.x += (ch->advance >> 6) * scale;
    }
}

void ui_init() {
    memset(&ui, 0, sizeof(ui_t));

    ui.text.vao = vao_create();
    vao_bind(&ui.text.vao);

    ui.text.vbo = vbo_create(NULL, sizeof(float) * 6 * 4);
    vao_attrib(0, 4, GL_FLOAT, 4 * sizeof(float), 0);

    queue_init(&ui.text.items, sizeof(ui_text_t), UI_MAX);
    queue_init(&ui.texture.items, sizeof(ui_texture_t), UI_MAX);
}

void ui_destroy() {
    queue_destroy(&ui.text.items);
    queue_destroy(&ui.texture.items);
}

void ui_render(const camera_t *camera) {
    /* ui_texture_t *texture; */
    /* ui.shader = renderer_use_shader(SHADER_UI_TEXTURE); */
    /* for (size_t i = 0; i < ui.texture.items.len; i++) { */
    /*     texture = queue_pop(&ui.texture.items); */
    /*     // render texture */
    /* } */

    vao_bind(&ui.text.vao);
    vbo_bind(&ui.text.vbo);

    ui_text_t *text;
    ui.shader = renderer_use_shader(SHADER_UI_TEXT);
    shader_uniform_mat4f(*ui.shader, "projection", camera->projection);

    for (size_t i = 0; i < ui.text.items.len; i++) {
        text = queue_pop(&ui.text.items);
        render_text(text->pos, text->scale, RGBA(text->color), text->text);
    }
}

void ui_clear() {
    queue_clear(&ui.text.items);
    queue_clear(&ui.texture.items);
}

void ui_draw_text(vec2s pos, float scale, uint32_t color, const char *text) {
    ui_text_t elem = (ui_text_t) {
        .pos = pos,
        .scale = scale,
        .color = color,
    };
    strncpy(elem.text, text, strnlen(text, 64));
    queue_push(&ui.text.items, &elem);
}
