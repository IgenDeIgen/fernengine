#include "../include/fern.h"
#include <stdarg.h>
#include <stdio.h>

const char* get_error_str(frnError_t error) {
	switch(error) {
		case FERN_ERROR_OK:
			return "No error";
    	case FERN_GLFW_INITIALIZATION_FAILED:
    		return "Failed to initalize GLFW";
		case FERN_WINDOW_CREATION_FAILED:
			return "Failed to create window";
		case FERN_GLAD_INITIALIZATION_FAILED:
			return "Failed to initialize GLAD";
        case FERN_FILE_NOT_FOUND:
			return "File not found";
        case FERN_FILE_READ_FAILED:
			return "File read failed";
        case FERN_ALLOC_FAILED:
			return "Memory allocation failed";
		case FERN_SHADER_COMPILATION_FAILED:
			return "Shader compile error";
		case FERN_LINKING_SHADERS_FAILED:
			return "Linking shaders failed";
		case FERN_UNKNOWN_MATERIAL_TYPE:
			return "Unknown material type";
		case FERN_INCORRECT_MATERIAL:
			return "Incorrect material";
		case FERN_UNSUPPORTED_FORMAT:
			return "Invalid format";
   }
}

frnError_t error_msg(frnError_t type, const char* msg_format, ...) {
#ifdef FERN_ERROR_LOG
	fprintf(stderr, "ERROR: %s\n", get_error_str(type));
	if(msg_format == NULL) { return type; }
	va_list argptr;
	va_start(argptr, msg_format);
	vfprintf(stderr, msg_format, argptr);
	va_end(argptr);
#endif
	return type;
}
