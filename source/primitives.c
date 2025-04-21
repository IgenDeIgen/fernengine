#include "../include/fern.h"
#include "cglm/types.h"

void create_primitive_rect(f32 width, f32 height,
				 shader_t shader,
				 model_t *out)
{
	vertex_t vertices[4] = {
		{ -0.5f,  0.5f, 0.0f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
		{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f }
	};

	u32 indices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	mesh_t mesh;
	mesh.vertices = vertices;
	mesh.vertex_count = 4;
	mesh.indices = indices;
	mesh.index_count = 6;

	create_model(&mesh, shader, out);
	transform_scale(&out->transform, (vec3){width, height, 0.0f});
}
