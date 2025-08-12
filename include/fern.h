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
typedef enum {
	
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
	FERN_UNKNOWN_MATERIAL_TYPE,

	FERN_UNSUPPORTED_FORMAT

} frnError_t;

typedef struct {
	f32 r, g, b, a;
} rgba_t;

typedef struct {
	u32 width, height;
	char* title;
	rgba_t clear_color;
} WindowInfo;

typedef struct {
	GLFWwindow* glfw_window;
} Renderer;

typedef struct {
	vec3 position;
	vec2 uv;
	vec3 normal;
} Vertex;

typedef struct {
	Vertex* vertices;
	u32* indices;
	u32 vertex_count, index_count;
} Mesh;

typedef u32 Shader;

typedef enum {
	MATERIAL_PROPERTY_TYPE_MAT4,
	MATERIAL_PROPERTY_TYPE_TEXTURE2D,
	MATERIAL_PROPERTY_TYPE_INT
} MaterialPropertyType;

typedef struct {
	const char* name;
	MaterialPropertyType type;
	void* valueptr;
} MaterialProperty;

typedef struct {
	Shader shader;
	MaterialProperty* static_properties;
	u32 dynamic_property_count;
	MaterialProperty* dynamic_properties;
	u32 property_count;
} Material;

typedef struct {
	u32 VBO, VAO, EBO;
	u32 vertex_count, index_count;
	Material* material;
} Model;

typedef struct {
	Model model;
} Object;

typedef struct {
	char* shader_code;
	u32 size;
} ShaderSource;

typedef enum {
	PIXELFORMAT_RGB = GL_RGB,
	PIXELFORMAT_RGBA = GL_RGBA,
} PixelFormat;

typedef enum {
	TEXTUREWRAPMODE_REPEAT = GL_REPEAT,
	TEXTUREWRAPMODE_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	TEXTUREWRAPMODE_CLAMP_EDGE = GL_CLAMP_TO_EDGE,
	TEXTUREWRAPMODE_CLAMP_BORDER = GL_CLAMP_TO_BORDER
} TextureWrapMode;

typedef enum {
	TEXTUREFILTERMODE_LINEAR = GL_LINEAR,
	TEXTUREFILTERMODE_NEAREST = GL_NEAREST
} TextureFilterMode;

typedef struct {
	TextureWrapMode wrap_s, wrap_t;
	TextureFilterMode filter_mag, filter_min;
} TextureParams;

typedef struct texture {
	u32 id;
	u32 width, height;
} Texture;


const char* get_error_str(frnError_t error);

frnError_t error_msg(frnError_t type, const char* msg_format, ...);

frnError_t read_file_all(char** contents, u32* size, const char* path);

//	Creates and initializes a renderer_t and creates a corresponding
//	window with given parameters.
frnError_t CreateRenderer(Renderer* renderer, WindowInfo window_info);

// Frees resources associated with the renderer
void DestroyRenderer(Renderer* renderer);

// Returns true when the user closes the window
u32 RendererShouldClose(Renderer *renderer);

// Clears the buffer with the specified color in the renderer
void ClearRenderer(Renderer* renderer);

// Swaps the render buffers
void SwapRenderer(Renderer* renderer);

// Polls window events
void PollEvents();

frnError_t LoadMaterialProperty(MaterialProperty property, Shader shader);

frnError_t AddMaterialProperty(
	Material* material,
	MaterialPropertyType type,
	const char* name,
	void* ptr,
	u32 dynamic
);

// Binds a shader for rendering and supplies it with the required material properties
frnError_t UseMaterial(Material* material);

void DestroyMaterial(Material* material);

// Creates a model_t from a given mesh and shader, allocating the data on
// the GPU
frnError_t CreateModel(Model* model, Mesh* mesh, Material* material);

// Frees up the corresponding GPU resources
void DestroyModel(Model* model);

// Renders a model
void RenderModel(Model* model);

// Loads shader source code from specified path
//
// WARNING: always use destroy_shader_source to free this resource
frnError_t LoadShaderSource(ShaderSource* source, const char* path);
// Frees resources associated with a shader_source_t
void DestroyShaderSource(ShaderSource* source);

// Creates a shader program from given shader sources
frnError_t CreateShaderFromSource(
	Shader* shader,
	ShaderSource vertex_source,
	ShaderSource fragment_source
);

// Creates a shader program from the given source files
frnError_t CreateShader(
	Shader* shader,
	const char* vertex_path,
	const char* fragment_path
);

// Destroys the shader program
void DestroyShader(Shader *shader);

frnError_t CreateTexture(Texture *texture, TextureParams params, u8 *data,
                         u32 width, u32 height, int format);
//void DestroyTexture(Texture* texture);

void UseTexture(Texture* texture, s32 slot);

frnError_t LoadBMP(Texture* texture, TextureParams params, const char* path);

frnError_t LoadOBJ(Model* model, const char* path);

#define FERN_ERROR_LOG

#endif
