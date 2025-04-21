#include "../include/fern.h"

#include <stdio.h>
#include <stdlib.h>

s32 load_filesrc(char* file_path, filesrc_t* filesrc) {
	
	FILE* f = fopen(file_path, "rb");
	
	if(f) {
		fseek(f, 0, SEEK_END);
		filesrc->size = ftell(f);
		fseek(f, 0, SEEK_SET);
		
		filesrc->buffer = (char*)malloc(filesrc->size + 1);
		if(!filesrc->buffer) {
			fclose(f);
			fprintf(stderr, "Error: failed to allocate filesrc buffer for: %s\n", file_path);
			return FILESRC_ALLOC_FAILED; // Failed to allocate buffer
		}
		
		size_t read_size = fread(filesrc->buffer, 1, filesrc->size, f);
		if(read_size != filesrc->size) {
			free(filesrc->buffer);
			fclose(f);
			fprintf(stderr, "Error: failed to read file: %s\n", file_path);
			return FILESRC_READ_ERROR; // File read error
		}

		filesrc->buffer[filesrc->size] = '\0';
	}
	else {
		fprintf(stderr, "Error: file doesn't exist: %s\n", file_path);
		return FILESRC_FILE_NOT_EXISTS; // File doesn't exist
	}

	fclose(f);
	return FILESRC_OK;
}

void destroy_filesrc(filesrc_t* filesrc) {
	filesrc->size = 0;
	free(filesrc->buffer);
}
