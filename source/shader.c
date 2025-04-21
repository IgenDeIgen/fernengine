#include "../include/fern.h"

#include <stdio.h>

static s32 compile_shader(u32 shader) {
	glCompileShader(shader);

	s32 success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	char log[512];
	if(!success) {
		glGetShaderInfoLog(shader, 512, NULL, log);
		fprintf(stderr, "Error: shader compilation error: %s\n", log);
	}

	return success;
}

shader_t create_shader_code(
	const char* const* vertex_shader_code,
	const char* const* fragment_shader_code
) {	
	// load vertex shader
	u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, vertex_shader_code, NULL);
	
	if(!compile_shader(vertex_shader)) { return 0; }

	// load fragment shader
	u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragment_shader_code, NULL);

	if(!compile_shader(fragment_shader)) { return 0; }

	// link shaders
	
	shader_t shader = glCreateProgram();
	
	glAttachShader(shader, vertex_shader);
	glAttachShader(shader, fragment_shader);

	glLinkProgram(shader);

	// check for linking errors
	s32 success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if(!success) {
		char log[512];
		glGetProgramInfoLog(shader, 512, NULL, log);
		fprintf(stderr, "Error: error linking shaders: %s\n", log);
		return 0;
	}

	// free up resources
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader;
}

shader_t create_shader(
	char* vertex_shader_path,
	char* fragment_shader_path
) {
	filesrc_t src;

	// load vertex shader
	if(load_filesrc(vertex_shader_path, &src) != FILESRC_OK) { return 0; }

	u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, (const GLchar* const*)&src.buffer, NULL);
	
	if(!compile_shader(vertex_shader)) {	
		destroy_filesrc(&src);
		return 0;
	}

	// load fragment shader
	if(load_filesrc(fragment_shader_path, &src) != FILESRC_OK) { return 0; }

	u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, (const GLchar* const*)&src.buffer, NULL);

	if(!compile_shader(fragment_shader)) {
		destroy_filesrc(&src);
		return 0;
	}

	// link shaders
	
	shader_t shader = glCreateProgram();
	
	glAttachShader(shader, vertex_shader);
	glAttachShader(shader, fragment_shader);

	glLinkProgram(shader);

	// check for linking errors
	s32 success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if(!success) {
		char log[512];
		glGetProgramInfoLog(shader, 512, NULL, log);
		fprintf(stderr, "Error: error linking shaders: %s\n", log);
		return 0;
	}

	// free up resources
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	destroy_filesrc(&src);

	return shader;
}

void shader_set_mat4(shader_t* shader, const char* name, mat4 mat) {
	glProgramUniformMatrix4fv(
		*shader,
		glGetUniformLocation(*shader, name),
		1, GL_FALSE,
		mat[0]
	);
}

void shader_set_vec3(shader_t* shader, const char* name, vec3 vec) {
	glProgramUniform3fv(
		*shader,
		glGetUniformLocation(*shader, name),
		1, vec
	);
}

void shader_set_vec4(shader_t* shader, const char* name, vec4 vec) {
	glProgramUniform4fv(
		*shader,
		glGetUniformLocation(*shader, name),
		1, vec
	);
}

void shader_set_int(shader_t *shader, const char *name, s32 val) {
	glProgramUniform1i(
		*shader,
		glGetUniformLocation(*shader, name),
		val
	);
}

void destroy_shader(shader_t* shader) {
	glDeleteProgram(*shader);
}
