#include "../include/fern.h"
#include <stdlib.h>

frnError_t AddMaterialProperty(
	Material* material, MaterialPropertyType type,
    const char *name, void *ptr,
    u32 dynamic
) {

	MaterialProperty property = {
		.type = type,
		.name = name,
		.valueptr = ptr
	};

	material->property_count += 1;

	if(dynamic) {
		material->dynamic_property_count += 1;

		if(material->dynamic_property_count == 1) {
			material->dynamic_properties = (MaterialProperty*)malloc(sizeof(MaterialProperty));
		}
		else {
			material->dynamic_properties = (MaterialProperty*)realloc(material->dynamic_properties, sizeof(MaterialProperty) * material->dynamic_property_count);
		}

		if(material->dynamic_properties == NULL) {
			return error_msg(FERN_ALLOC_FAILED, "Failed to allocate dynamic material properties!");
		}

		material->dynamic_properties[material->dynamic_property_count - 1] = property;
	}
	else {
		u32 static_property_count = material->property_count - material->dynamic_property_count;
		
		if(static_property_count == 1) {
			material->static_properties = (MaterialProperty*)malloc(sizeof(MaterialProperty));
		}
		else {
			material->static_properties = (MaterialProperty*)realloc(material->static_properties, sizeof(MaterialProperty) * static_property_count);
		}
		
		if(material->static_properties == NULL) {
			return error_msg(FERN_ALLOC_FAILED, "Failed to allocate static material properties!");
		}

		material->static_properties[static_property_count - 1] = property;
	}

	for (int i = 0; i < material->dynamic_property_count; i++) {
		MaterialProperty property = material->dynamic_properties[i];
		frnError_t status = LoadMaterialProperty(property, material->shader);
		if (status != FERN_ERROR_OK) {
		  return status;
		}
	}
	
	for (int i = 0; i < material->property_count - material->dynamic_property_count; i++) {
		MaterialProperty property = material->static_properties[i];
		frnError_t status = LoadMaterialProperty(property, material->shader);
		if (status != FERN_ERROR_OK) {
		  return status;
		}
	}

	return FERN_ERROR_OK;
}

void DestroyMaterial(Material *material) {
	free(material->static_properties);
	free(material->dynamic_properties);
	material->property_count = 0;
	material->dynamic_property_count = 0;
}

frnError_t UseMaterial(Material *material) {
	glUseProgram(material->shader);

	if (material->dynamic_property_count == 0 && material->dynamic_properties != NULL) {
		return error_msg(FERN_INCORRECT_MATERIAL, NULL);
	}

	for (int i = 0; i < material->dynamic_property_count; i++) {
		MaterialProperty property = material->dynamic_properties[i];
	    frnError_t status = LoadMaterialProperty(property, material->shader);
		if (status != FERN_ERROR_OK) {
	      return status;
		}
	}

	return FERN_ERROR_OK;
}

static inline void set_texture(Shader* shader, const char* name, Texture* texture);
static inline void set_mat4(Shader* shader, const char* name, mat4 mat);
static inline void set_int(Shader* shader, const char* name, int value);

static inline void set_texture(Shader* shader, const char* name, Texture* texture) {
	UseTexture(texture, GL_TEXTURE0);
	set_int(shader, name, 0);
}

static inline void set_mat4(Shader* shader, const char* name, mat4 mat) {
	glProgramUniformMatrix4fv(
		*shader,
		glGetUniformLocation(*shader, name),
		1, GL_FALSE,
		mat[0]
	);
}

static inline void set_int(Shader* shader, const char* name, int value) {
	glProgramUniform1i(
		*shader,
		glGetUniformLocation(*shader, name),
		value
	);
}

frnError_t LoadMaterialProperty(MaterialProperty property, Shader shader) {
	switch (property.type) {
		case MATERIAL_PROPERTY_TYPE_MAT4:
		{
			set_mat4(&shader, property.name, property.valueptr);
			break;
		}
		case MATERIAL_PROPERTY_TYPE_INT:
		{
			set_int(&shader, property.name, *((int*)property.valueptr));
			break;
		}
		case MATERIAL_PROPERTY_TYPE_TEXTURE2D:
		{
			set_texture(&shader, property.name, property.valueptr);
			break;
		}
		case MATERIAL_PROPERTY_TYPE_CAMERA:
		{
			set_mat4(&shader, "view", ((Camera*)property.valueptr)->view_matrix);
			set_mat4(&shader, "projection", ((Camera*)property.valueptr)->projection_matrix);
			break;
		}
		default:
			return error_msg(
				FERN_UNKNOWN_MATERIAL_TYPE,
				"%s has unknown type\n", property.name
			);
	}

	return FERN_ERROR_OK;
}
