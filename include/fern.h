#ifndef ENGINE_H
#define ENGINE_H

// Standard types
#include "cglm/types.h"
#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

typedef struct {
	f32 r, g, b, a;
} color_rgba;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

// File loading utilities

typedef struct {
	char* buffer;
	size_t size;
} filesrc_t;

#define FILESRC_OK 1
#define FILESRC_FILE_NOT_EXISTS -1
#define FILESRC_ALLOC_FAILED -2
#define FILESRC_READ_ERROR -3 

s32 load_filesrc(char* file_path, filesrc_t* filesrc);
void destroy_filesrc(filesrc_t* filesrc);

// Shaders

typedef u32 shader_t;

shader_t create_shader(
		char* vertex_shader_path,
		char* fragment_shader_path
		);
shader_t create_shader_code(
		const char* const* vertex_shader_code,
		const char* const* fragment_shader_code
		);

void shader_set_mat4(shader_t* shader, const char* name, mat4 mat);
void shader_set_vec3(shader_t* shader, const char* name, vec3 vec);
void shader_set_vec4(shader_t* shader, const char* name, vec4 vec);
void shader_set_int(shader_t* shader, const char* name, s32 val);

void destroy_shader(shader_t* shader);


// Renderer and windowing

typedef struct {
	GLFWwindow* glfw_window;

	mat4 view, projection;
} renderer_t;

s32 init_renderer(
		renderer_t* renderer,
		u32 window_width,
		u32 window_height,
		char* window_title
	);

void quit_renderer(renderer_t* renderer);
s32 renderer_should_close(renderer_t* renderer);
void set_clear_color(renderer_t* renderer, color_rgba color);
void render_clear(renderer_t* renderer);
void render_swap(renderer_t* renderer);
void poll_events();


// Input

s32 input_keydown(renderer_t* renderer, s32 keycode);


// Transform

typedef struct {
	vec3 position;
	versor rotation;
	vec3 scale;

	mat4 matrix;
} transform_t;

void transform_translate(transform_t* transform, vec3 v);

void transform_rotate(transform_t* transform, vec3 axis, f32 angle);

void transform_scale(transform_t* transform, vec3 s);
void transform_scalef(transform_t* transform, f32 s);

void transform_apply(transform_t* transform);


// Camera

typedef struct {
	transform_t transform;
} camera_t;


// Textures

typedef struct {
	u32 id;
	u32 width, height;
} texture_t;

void create_texture_from_image(const char* path, texture_t* texture);
void destroy_texture(texture_t* texture);
void use_texture(texture_t* texture);


// Models and meshes

typedef struct {
	vec3 postion;
	vec2 uv;
} vertex_t;

typedef struct {
	vertex_t* vertices;
	u32 vertex_count;
	u32* indices;
	u32 index_count;
} mesh_t;

typedef struct {
	u32 VBO, VAO, EBO;
	u32 vertex_count;
	u32 index_count;
	u32 shader;
	transform_t transform;
} model_t;

void create_model(
		mesh_t* mesh,
		shader_t shader,
		model_t* out
		);

void destroy_model(model_t* model);
void render_model(model_t* model);

typedef struct {
	u32 width, height;
	f32 x_offset, y_offset;
	f32 line_offset;
} instance_grid_t;

void render_instance_grid(instance_grid_t* grid, model_t* model, shader_t instance_shader);

// Primitives
void create_primitive_rect(f32 width, f32 height, shader_t shader, model_t* out);

#endif
