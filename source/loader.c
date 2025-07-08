#include "../include/fern.h"
#include <stdio.h>
#include <string.h>

fern_error_t read_file_all(char **contents, u32* size, const char* path) {
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

fern_error_t load_obj(model_t *model, const char *path) {
	FILE* f = fopen(path, "rb");

	mesh_t mesh;

	vec3* positions; u32 position_count;
	vec2* uvs; u32 uv_count;
	vec3* normals; u32 normal_count;

	if(f) {
		// 1. pass - get number of entries
		while(1){
			char linetype[3];
			s32 status = fscanf(f, "%s", linetype);
			if(status == EOF) { break; }

			if(strcmp(linetype, "v") == 0) {
				
			}
			else if(strcmp(linetype, "vt") == 0) {

			}
			else if(strcmp(linetype, "vn") == 0) {
	
			}
		}
	}
	else {
		return error_msg(FERN_FILE_NOT_FOUND, "File: %s\n", path);
	}

	fclose(f);
	
	return FERN_ERROR_OK;
}
