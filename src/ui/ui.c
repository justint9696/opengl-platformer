/**
 * @file ui.c
 * @author Justin Tonkinson
 * @date 2024/06/08
 * @brief UI implementation functions.
 */

#include "ui/ui.h"

#include "graphics/drawing.h"
#include "graphics/font.h"
#include "graphics/renderer.h"

#include <cglm/struct.h>
#include <string.h>

static ui_t ui;

static void render_text(vec2s pos, float scale, vec4s color, const char *text) {
    shader_uniform_3f(*ui.shader, "color", color.r, color.g, color.b);
    glActiveTexture(GL_TEXTURE0);

    size_t len = strlen(text);
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

static void render_texture(vec2s pos, vec2s dim, uint32_t color) {
    float vertices[] = {
        pos.x, pos.y, 0.f,
        pos.x, pos.y + dim.y, 0.f,
        pos.x + dim.x, pos.y + dim.y, 0.f,
        pos.x + dim.x, pos.y, 0.f,
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    vao_attrib(0, 3, GL_FLOAT, 3 * sizeof(float), NULL);

    vbo_buffer_data(vertices, sizeof(vertices));
    ibo_buffer_data(indices, sizeof(indices));

    shader_uniform_vec4f(*ui.shader, "color", RGBA(color));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ui_init() {
    memset(&ui, 0, sizeof(ui_t));

    ui.text.vao = vao_create();
    vao_bind(&ui.text.vao);

    ui.text.vbo = vbo_create(NULL, sizeof(float) * 6 * 4);
    vao_attrib(0, 4, GL_FLOAT, 4 * sizeof(float), 0);

    vao_unbind();
    vbo_unbind();

    ui.texture.vao = vao_create();
    ui.texture.vbo = vbo_create(NULL, 0);
    ui.texture.ibo = ibo_create(NULL, 0);

    queue_init(&ui.text.items, sizeof(ui_text_t), UI_MAX);
    queue_init(&ui.texture.items, sizeof(ui_texture_t), UI_MAX);

    vao_unbind();
    vbo_unbind();
    ibo_unbind();
}

void ui_destroy() {
    queue_destroy(&ui.text.items);
    queue_destroy(&ui.texture.items);
}

void ui_render(const camera_t *camera) {
    ui_texture_t *texture;
    ui.shader = renderer_use_shader(SHADER_UI_TEXTURE);

    vao_bind(&ui.texture.vao);
    vbo_bind(&ui.texture.vbo);
    ibo_bind(&ui.texture.ibo);

    shader_uniform_mat4f(*ui.shader, "projection", camera->projection);

    for (size_t i = 0; i < ui.texture.items.len; i++) {
        texture = queue_pop(&ui.texture.items);
        render_texture(texture->pos, texture->dim, texture->color);
    }

    vao_unbind();
    vbo_unbind();
    ibo_unbind();

    ui_text_t *text;
    ui.shader = renderer_use_shader(SHADER_UI_TEXT);

    vao_bind(&ui.text.vao);
    vbo_bind(&ui.text.vbo);

    shader_uniform_mat4f(*ui.shader, "projection", camera->projection);

    size_t len = ui.text.items.len;
    for (size_t i = 0; i < len; i++) {
        text = queue_pop(&ui.text.items);
        render_text(text->pos, text->scale, RGBA(text->color), text->text);
    }

    ui.text.n = 0;

    vao_unbind();
    vbo_unbind();
}

void ui_clear() {
    ui.text.n = 0;
    queue_clear(&ui.text.items);
    queue_clear(&ui.texture.items);
}

void ui_draw_debug(const char *text) {
    ui_text_t elem = {
        .pos = { UI_DEBUG_HORZ, UI_DEBUG_VERT - (ui.text.n++ * 20.f)},
        .scale = 1.f,
        .color = COLOR_WHITE,
    };
    assert(strlen(text) < 64);
    strcpy(elem.text, text);
    queue_push(&ui.text.items, &elem);
}

void ui_draw_text(vec2s pos, float scale, uint32_t color, const char *text) {
    ui_text_t elem = {
        .pos = pos,
        .scale = scale,
        .color = color,
    };
    assert(strlen(text) < 64);
    strcpy(elem.text, text);
    queue_push(&ui.text.items, &elem);
}

void ui_draw_quad(vec2s pos, vec2s dim, uint32_t color) {
    queue_push(&ui.texture.items, &(ui_texture_t) {
        .pos = pos,
        .dim = dim,
        .color = color,
    });
}
