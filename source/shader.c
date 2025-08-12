#include "../include/fern.h"
#include <stdio.h>
#include <stdlib.h>

frnError_t LoadShaderSource(ShaderSource *source, const char *path) {
	char* contents;
	u32 size;
	read_file_all(&contents, &size, path);
	source->shader_code = contents;
	source->size = size;
	return FERN_ERROR_OK;
}

void DestroyShaderSource(ShaderSource *source) {
	source->size = 0;
	free(source->shader_code);
}

static frnError_t compile_component(u32 component) {
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

static frnError_t create_shader_component(
	u32* component,
	ShaderSource src,
	const s32 shader_type
) {
	u32 c = glCreateShader(shader_type);
	glShaderSource(c, 1, (const GLchar* const*)&src.shader_code, NULL);
	*component = c;
	return compile_component(c);
}

static frnError_t link_components(Shader shader, u32 vertex, u32 fragment) {
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

frnError_t CreateShaderFromSource( Shader* shader, ShaderSource vertex_source, ShaderSource fragment_source ) {
	
	u32 vertex_shader, fragment_shader;

	frnError_t vertex_status = create_shader_component(
		&vertex_shader,
		vertex_source,
		GL_VERTEX_SHADER
	);
	frnError_t fragment_status = create_shader_component(
		&fragment_shader,
		fragment_source,
		GL_FRAGMENT_SHADER
	);

	if(vertex_status != FERN_ERROR_OK) { return vertex_status; }
	if(fragment_status != FERN_ERROR_OK) { return fragment_status; }

	Shader out = glCreateProgram();

	frnError_t linking_status = link_components(out, vertex_shader, fragment_shader);
	if(linking_status != FERN_ERROR_OK) { return linking_status; }
		
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	*shader = out;

	return FERN_ERROR_OK;
}

frnError_t CreateShader(
	Shader* shader,
	const char* vertex_path,
	const char* fragment_path
) {
	
	u32 vertex_shader, fragment_shader;

	ShaderSource src;

	LoadShaderSource(&src, vertex_path);
	frnError_t vertex_status = create_shader_component(
		&vertex_shader,
		src,
		GL_VERTEX_SHADER
	);

	LoadShaderSource(&src, fragment_path);
	frnError_t fragment_status = create_shader_component(
		&fragment_shader,
		src,
		GL_FRAGMENT_SHADER
	);

	DestroyShaderSource(&src);

	if(vertex_status != FERN_ERROR_OK) { return vertex_status; }
	if(fragment_status != FERN_ERROR_OK) { return fragment_status; }

	Shader out = glCreateProgram();
	
	frnError_t linking_status = link_components(out, vertex_shader, fragment_shader);
	if(linking_status != FERN_ERROR_OK) { return linking_status; }
	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	*shader = out;

	return FERN_ERROR_OK;
}

void DestroyShader(Shader *shader) {
	glDeleteProgram(*shader);
}
