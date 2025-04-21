#include "../include/fern.h"

void create_model(mesh_t* mesh, shader_t shader, model_t* output) {
	// Generate OpenGL buffers
	glGenVertexArrays(1, &output->VAO);
	glGenBuffers(1, &output->VBO);
	glGenBuffers(1, &output->EBO);

	// Bind VAO
	glBindVertexArray(output->VAO);

	// Load vertex and index data into respective buffers
	glBindBuffer(GL_ARRAY_BUFFER, output->VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertex_t) * mesh->vertex_count,
		mesh->vertices,
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, output->EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(u32) * mesh->index_count,
		mesh->indices,
		GL_STATIC_DRAW
	);

	// Initalize model data
	output->shader = shader;
	output->vertex_count = mesh->vertex_count;
	output->index_count = mesh->index_count;

	// Fill vertex attributes
	// Position vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)0);
	glEnableVertexAttribArray(0);
	// UV vertex attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	// Unbind vertex arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize model matrix
	glm_mat4_identity(output->transform.matrix);
}

void render_model(model_t* model) {
	glUseProgram(model->shader);
	shader_set_mat4(&model->shader, "u_model", model->transform.matrix);
	glBindVertexArray(model->VAO);
	glDrawElements(GL_TRIANGLES, model->index_count, GL_UNSIGNED_INT, 0);
}

void destroy_model(model_t* model) {
	glDeleteVertexArrays(1, &model->VAO);
	glDeleteBuffers(1, &model->VBO);
	glDeleteBuffers(1, &model->EBO);
}

// Instance Grid

void render_instance_grid(
	instance_grid_t *grid,
	model_t *model,
	shader_t instance_shader
) {
	
	glUseProgram(instance_shader);
	glBindVertexArray(model->VAO);
	
	shader_set_mat4(&instance_shader, "model", model->transform.matrix);

	vec4 grid_offsets = {
		grid->x_offset,
		grid->y_offset,
		grid->line_offset,
		(f32)grid->width
	};
	shader_set_vec4(&instance_shader, "grid_offsets", grid_offsets);

	glDrawElementsInstanced(GL_TRIANGLES, model->index_count, GL_UNSIGNED_INT, 0, grid->width * grid->height);
}

