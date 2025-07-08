#include "../include/fern.h"
#include <stdio.h>
#include <stdlib.h>

fern_error_t load_shader_source(shader_source_t *source, const char *path) {
	char* contents;
	u32 size;
	read_file_all(&contents, &size, path);
	source->shader_code = contents;
	source->size = size;
	return FERN_ERROR_OK;
}

void destroy_shader_source(shader_source_t *source) {
	source->size = 0;
	free(source->shader_code);
}

static fern_error_t compile_component(u32 component) {
	glCompileShader(component);

	s32 success;
	glGetShaderiv(component, GL_COMPILE_STATUS, &success);
	
#ifdef FERN_ERROR_LOG
	if(!success) {
		char log[512];
		glGetShaderInfoLog(component, 512, NULL, log);
		return error_msg(FERN_SHADER_COMPILATION_FAILED, "%s\n", log);
	}
#endif
	
	if(!success) { return FERN_SHADER_COMPILATION_FAILED; }
	else return FERN_ERROR_OK;
}

static fern_error_t create_shader_component(
	u32* component,
	shader_source_t src,
	const s32 shader_type
) {
	u32 c = glCreateShader(shader_type);
	glShaderSource(c, 1, (const GLchar* const*)&src.shader_code, NULL);
	*component = c;
	return compile_component(c);
}

static fern_error_t link_components(shader_t shader, u32 vertex, u32 fragment) {
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);

	glLinkProgram(shader);

	s32 success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

 #ifdef FERN_ERROR_LOG
	if(!success) {
		char log[512];
		glGetProgramInfoLog(shader, 512, NULL, log);
		return error_msg(FERN_LINKING_SHADERS_FAILED, "%s\n", log);
	}
#endif

	if(!success) { return FERN_LINKING_SHADERS_FAILED; }
	return FERN_ERROR_OK;
}

fern_error_t create_shader_from_source( shader_t* shader, shader_source_t vertex_source, shader_source_t fragment_source ) {
	
	u32 vertex_shader, fragment_shader;

	fern_error_t vertex_status = create_shader_component(
		&vertex_shader,
		vertex_source,
		GL_VERTEX_SHADER
	);
	fern_error_t fragment_status = create_shader_component(
		&fragment_shader,
		fragment_source,
		GL_FRAGMENT_SHADER
	);

	if(vertex_status != FERN_ERROR_OK) { return vertex_status; }
	if(fragment_status != FERN_ERROR_OK) { return fragment_status; }

	shader_t out = glCreateProgram();

	fern_error_t linking_status = link_components(out, vertex_shader, fragment_shader);
	if(linking_status != FERN_ERROR_OK) { return linking_status; }
		
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	*shader = out;

	return FERN_ERROR_OK;
}

fern_error_t create_shader(
	shader_t* shader,
	const char* vertex_path,
	const char* fragment_path
) {
	
	u32 vertex_shader, fragment_shader;

	shader_source_t src;

	load_shader_source(&src, vertex_path);
	fern_error_t vertex_status = create_shader_component(
		&vertex_shader,
		src,
		GL_VERTEX_SHADER
	);

	load_shader_source(&src, fragment_path);
	fern_error_t fragment_status = create_shader_component(
		&fragment_shader,
		src,
		GL_FRAGMENT_SHADER
	);

	destroy_shader_source(&src);

	if(vertex_status != FERN_ERROR_OK) { return vertex_status; }
	if(fragment_status != FERN_ERROR_OK) { return fragment_status; }

	shader_t out = glCreateProgram();
	
	fern_error_t linking_status = link_components(out, vertex_shader, fragment_shader);
	if(linking_status != FERN_ERROR_OK) { return linking_status; }
	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	*shader = out;

	return FERN_ERROR_OK;
}

void destroy_shader(shader_t *shader) {
	glDeleteProgram(*shader);
}
