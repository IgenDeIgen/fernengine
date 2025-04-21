#include "../include/fern.h"

#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>
#include <stdio.h>

void create_texture_from_image(const char *path, texture_t *texture) {
	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	s32 width, height, channel_count;
	stbi_set_flip_vertically_on_load(1);

	unsigned char* data = stbi_load(path, &width, &height, &channel_count, 0);

	if(data) {
		glTexImage2D(GL_TEXTURE_2D,
			   0,
			   GL_RGBA,
			   width, height,
			   0,
			   GL_RGBA,
			   GL_UNSIGNED_BYTE,
			   data);

        glGenerateMipmap(GL_TEXTURE_2D);
	}
	else { fprintf(stderr, "Error: failed to load image file at: %s", path); }

	stbi_image_free(data);

	texture->width = (s32)width;
	texture->height = (s32)height;
}

void destroy_texture(texture_t *texture) {
	glDeleteTextures(1, &texture->id);
}

void use_texture(texture_t *texture) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->id);
}
