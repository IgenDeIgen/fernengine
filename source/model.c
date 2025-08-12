#include "../include/fern.h"

frnError_t CreateModel(Model* model, Mesh* mesh, Material* material) {
	// Generate OpenGL buffers
	glGenVertexArrays(1, &model->VAO);
	glGenBuffers(1, &model->VBO);
	glGenBuffers(1, &model->EBO);

	// Bind VAO
	glBindVertexArray(model->VAO);

	// Load vertex and index data into respective buffers
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(Vertex) * mesh->vertex_count,
		mesh->vertices,
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(u32) * mesh->index_count,
		mesh->indices,
		GL_STATIC_DRAW
	);

	// Initalize model data
	model->material = material;
	model->vertex_count = mesh->vertex_count;
	model->index_count = mesh->index_count;

	// Fill vertex attributes
	//
	// Position vertex attribute
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0
	);
	glEnableVertexAttribArray(0);

	// UV vertex attribute
	glVertexAttribPointer(
		1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec3))
	);
	glEnableVertexAttribArray(1);

	// Vertex normal attribute
	glVertexAttribPointer(
		2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(sizeof(vec3) + sizeof(vec2))
	);

	// Unbind vertex arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return FERN_ERROR_OK;
}

void DestroyModel(Model* model) {
	glDeleteVertexArrays(1, &model->VAO);
	glDeleteBuffers(1, &model->VBO);
	glDeleteBuffers(1, &model->EBO);
}

void RenderModel(Model *model) {
	UseMaterial(model->material);
	glBindVertexArray(model->VAO);
	glDrawElements(GL_TRIANGLES, model->index_count, GL_UNSIGNED_INT, 0);
}
