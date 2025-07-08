#include "../include/fern.h"

static void set_mat4(shader_t* shader, const char* name, mat4 mat) {
	glProgramUniformMatrix4fv(
		*shader,
		glGetUniformLocation(*shader, name),
		1, GL_FALSE,
		mat[0]
	);
}

fern_error_t use_material(material_t *material) {
	glUseProgram(material->shader);

	if(
		material->property_count == 0 ||
		material->properties == NULL
	) {
		return error_msg(FERN_INCORRECT_MATERIAL, NULL);
	}

	for(int i = 0; i < material->property_count; i++) {
		material_property_t property = material->properties[i];
		switch (property.type) {
			case MATERIAL_PROPERTY_TYPE_MAT4:
				set_mat4(
					&material->shader,
					property.name,
					property.valueptr
				);
				break;

			default:
				return error_msg(
					FERN_UNKNOWN_MATERIAL_TYPE,
					"%s has unknown type\n", property.name
				);
		}
	}

	return FERN_ERROR_OK;
}
