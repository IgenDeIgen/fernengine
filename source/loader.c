#include "../include/fern.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

frnError_t read_file_all(char **contents, u32* size, const char* path) {
	FILE* f = fopen(path, "rb");

	char* filecontents;
	u32 filesize = 0;
	if(f) {
		fseek(f, 0, SEEK_END);
		filesize = ftell(f);
		fseek(f, 0, SEEK_SET);
		
		filecontents = (char*)malloc((filesize) + 1);
		if(!filecontents) {
			fclose(f);
			return error_msg(FERN_ALLOC_FAILED, NULL); // Failed to allocate 
		}
		
		size_t read_size = fread(filecontents, 1, filesize, f);
		if(read_size != filesize) {
			free(filecontents);
			fclose(f);
			return error_msg(
				FERN_FILE_READ_FAILED,
				"File: %s\n", path
			); // File read error
		}

		filecontents[filesize] = '\0';
		
		*contents = filecontents;
		*size = filesize;
	}
	else {
		return error_msg(FERN_FILE_NOT_FOUND, "File: %s\n", path);
	}

	fclose(f);
	return FERN_ERROR_OK;
}

frnError_t LoadBMP(Texture* texture, TextureParams params, const char *path) {
	FILE* f = fopen(path, "r");

	if(f) {
		struct {
			u32 filesize;
			u16 reserved0, reserved1;
			u32 offset;
		} fileheader;

		if(fgetc(f) != 'B' || fgetc(f) != 'M') {
			fclose(f);
			return error_msg(FERN_UNSUPPORTED_FORMAT, "File: %s\n", path);
		}

		fread(&fileheader, sizeof(fileheader), 1, f);

		fseek(f, 14, SEEK_SET);

		struct {
			u32 headersize;
			s32 width, height;
			u16 nplanes;
			u16 bpp;
			u32 compression;
			u32 datasize;
			s32 hsize, vsize;
			u32 ncolors;
			u32 nimpcolors;
		} infoheader;
		
		fread(&infoheader, sizeof(infoheader), 1, f);

		if(infoheader.compression != 0) {
			fclose(f);
			return error_msg(FERN_UNSUPPORTED_FORMAT, "ONLY UNCOMPRESSED BMP IS SUPPORTED\n");
		}

		u8* data = (u8*)malloc(sizeof(u8) * infoheader.datasize);

		if(data == NULL) {
			fclose(f);
			return error_msg(FERN_ALLOC_FAILED, NULL);
		}

		fseek(f, fileheader.offset, SEEK_SET);
		u64 bytes_read = fread(data, sizeof(u8), infoheader.datasize, f);
		
		if(bytes_read < infoheader.datasize) {
			free(data);
			fclose(f);
			return error_msg(FERN_FILE_READ_FAILED, "Failed to read image data\n Image: %s\n", path);
		}

		fclose(f);

		fprintf(stdout, "Successfully loaded: %s\n", path);

		CreateTexture(
			texture,
			params,
			data,
			(u32)infoheader.width, (u32)infoheader.height,
			GL_BGR
		);

		free(data);
	}
	else { return error_msg(FERN_FILE_NOT_FOUND, "File: %s\n", path); }

	return FERN_ERROR_OK;
}

static inline void debug_log_obj_data(vec3* pos, u32 pos_c, vec2* uvs, u32 uv_c, vec3* normals, u32 normal_c) {
	printf("Obj data\n");

	printf("Positions[%d]:\n", pos_c);
	for(u32 i = 0; i < pos_c; i++) {
		printf("\t%f %f %f [%d]\n", pos[i][0], pos[i][1], pos[i][2], i+1);
	}
	printf("UVs[%d]:\n", uv_c);
	for(u32 i = 0; i < uv_c; i++) {
		printf("\t%f %f [%d]\n", uvs[i][0], uvs[i][1], i+1);
	}
	printf("Normals[%d]:\n", normal_c);
	for(u32 i = 0; i < normal_c; i++) {
		printf("\t%f %f %f [%d]\n", normals[i][0], normals[i][1], normals[i][2], i+1);
	}
}

frnError_t LoadOBJ(Model *model, const char *path) {
	FILE* f = fopen(path, "rb");

	Mesh mesh;

	vec3* positions = NULL; u32 position_count = 0;
	vec2* uvs = NULL; u32 uv_count = 0;
	vec3* normals = NULL; u32 normal_count = 0;

	u32* indices = NULL; u32 index_count = 0;

	Vertex* vertices = NULL; u32 vertex_count = 0;

	u32 i = 0;
	u32 j = 0;
	u32 k = 0;

	if(f) {
		u32 scans = 0;
		while(1){
			char linetype[3];
			s32 status = fscanf(f, "%3s", linetype);

			if(status == EOF) { 
				scans++;
				//printf("Scans: %d\n", scans);	
				if(positions == NULL && position_count > 0) {
					if(scans > 1) {
						fclose(f);
						return error_msg(FERN_FILE_READ_FAILED, NULL);
					}
					positions = (vec3*)malloc(sizeof(vec3) * position_count);
				}

				if(uvs == NULL && uv_count > 0) {
					if(scans > 1) {
						fclose(f);
						return error_msg(FERN_FILE_READ_FAILED, NULL);
					}
					uvs = (vec2*)malloc(sizeof(vec2) * uv_count);
				}

				if(normals == NULL && normal_count > 0) {
					if(scans > 1) {
						fclose(f);
						return error_msg(FERN_FILE_READ_FAILED, NULL);
					}
					normals = (vec3*)malloc(sizeof(vec3) * normal_count);
				}

				if(
					(!positions && position_count > 0) ||
					(!normals && normal_count > 0) ||
					(!uvs && uv_count > 0)
				) { fclose(f); return error_msg(FERN_ALLOC_FAILED, NULL); }

				if(scans > 2) { break; }
				else { fseek(f, SEEK_SET, 0); }
			}

			if(strcmp(linetype, "v") == 0) {
				if(positions == NULL) { position_count++; }
				else {
					fscanf(f, "%f %f %f",
						&positions[i][0],
						&positions[i][1],
						&positions[i][2]
					);
					i++;
				}
			}
			else if(strcmp(linetype, "vt") == 0) {
				if(uvs == NULL) { uv_count++; }
				else {
					fscanf(f, "%f %f",
						&uvs[j][0],
						&uvs[j][1]
					);
					j++;
				}
			}
			else if(strcmp(linetype, "vn") == 0) {
				if(normals == NULL) { normal_count++; }
				else {
					fscanf(f, "%f %f %f",
						&normals[k][0],
						&normals[k][1],
						&normals[k][2]
					);
					k++;
				}
			}

			else if(strcmp(linetype, "f") == 0) {
				// TODO: finish this shit...
			}

		}

		//debug_log_obj_data(positions, position_count, uvs, uv_count, normals, normal_count);
	}
	else {
		return error_msg(FERN_FILE_NOT_FOUND, "File: %s\n", path);
	}

	free(positions);
	free(uvs);
	free(normals);

	fclose(f);

	printf("loading finished\n");

	return FERN_ERROR_OK;
}
