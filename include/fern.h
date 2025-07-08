#ifndef FERN_H
#define FERN_H

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

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>


// Error code type
//
// FERN_ERROR_OK (0) when no error occured
typedef enum fern_error {
	
	FERN_ERROR_OK = 0,

	FERN_GLFW_INITIALIZATION_FAILED,
	FERN_WINDOW_CREATION_FAILED,
	FERN_GLAD_INITIALIZATION_FAILED,
	
	FERN_FILE_NOT_FOUND,
	FERN_FILE_READ_FAILED,
	
	FERN_ALLOC_FAILED,

	FERN_SHADER_COMPILATION_FAILED,
	FERN_LINKING_SHADERS_FAILED,

	FERN_INCORRECT_MATERIAL,
	FERN_UNKNOWN_MATERIAL_TYPE

} fern_error_t;

typedef struct rgba {
	f32 r, g, b, a;
} rgba_t;

typedef struct window_info {
	u32 width, height;
	char* title;
	rgba_t clear_color;
} window_info_t;

typedef struct renderer {
	GLFWwindow* glfw_window;
} renderer_t;

typedef struct vertex {
	vec3 position;
	vec2 uv;
	vec3 normal;
} vertex_t;

typedef struct mesh {
	vertex_t* vertices;
	u32* indices;
	u32 vertex_count, index_count;
} mesh_t;

typedef u32 shader_t;

typedef enum material_property_type {

	MATERIAL_PROPERTY_TYPE_MAT4

} material_property_type_t;

typedef struct material_property {
	const char* name;
	material_property_type_t type;
	void* valueptr;
} material_property_t;

typedef struct material {
	shader_t shader;
	material_property_t* properties;
	u32 property_count;
} material_t;

typedef struct model {
	u32 VBO, VAO, EBO;
	u32 vertex_count, index_count;
	material_t* material;
} model_t;

typedef struct object {
	model_t model;
} object_t;

typedef struct shader_source {
	char* shader_code;
	u32 size;
} shader_source_t;


const char* get_error_str(fern_error_t error);

fern_error_t error_msg(fern_error_t type, const char* msg_format, ...);

fern_error_t read_file_all(char** contents, u32* size, const char* path);

//	Creates and initializes a renderer_t and creates a corresponding
//	window with given parameters.
fern_error_t create_renderer(renderer_t* renderer, window_info_t window_info);

// Frees resources associated with the renderer
void destroy_renderer(renderer_t* renderer);

// Returns true when the user closes the window
u32 renderer_should_close(renderer_t *renderer);

// Clears the buffer with the specified color in the renderer
void clear_renderer(renderer_t* renderer);

// Swaps the render buffers
void swap_renderer(renderer_t* renderer);

// Polls window events
void poll_events();

// Binds a shader for rendering and supplies it with the required material properties
fern_error_t use_material(material_t* material);

// Creates a model_t from a given mesh and shader, allocating the data on
// the GPU
fern_error_t create_model(model_t* model, mesh_t* mesh, material_t* material);

// Frees up the corresponding GPU resources
void destroy_model(model_t* model);

// Renders a model
void render_model(model_t* model);

// Loads shader source code from specified path
//
// WARNING: always use destroy_shader_source to free this resource
fern_error_t load_shader_source(shader_source_t* source, const char* path);
// Frees resources associated with a shader_source_t
void destroy_shader_source(shader_source_t* source);

// Creates a shader program from given shader sources
fern_error_t create_shader_from_source(
	shader_t* shader,
	shader_source_t vertex_source,
	shader_source_t fragment_source
);

// Creates a shader program from the given source files
fern_error_t create_shader(
	shader_t* shader,
	const char* vertex_path,
	const char* fragment_path
);

// Destroys the shader program
void destroy_shader(shader_t *shader);

fern_error_t load_obj(model_t* model, const char* path);

#define FERN_ERROR_LOG

#endif
