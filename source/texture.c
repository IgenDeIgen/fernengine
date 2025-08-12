#include "../include/fern.h"

frnError_t CreateTexture(Texture *texture, TextureParams params, u8 *data,
                         u32 width, u32 height, int format) {
	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrap_t);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.filter_mag);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	texture->width = width;
	texture->height = height;

	return FERN_ERROR_OK;
}

void UseTexture(Texture *texture, s32 slot) {
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, texture->id);
}
