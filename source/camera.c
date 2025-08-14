#include "../include/fern.h"

frnError_t CreateCamera(Camera *camera, ProjectionType ptype, Renderer* renderer) {
	glm_mat4_identity(camera->view_matrix);
	glm_mat4_identity(camera->projection_matrix);

	f32 near = 0.1f;
	f32 far = 100.0f;

	if(ptype == PROJECTIONTYPE_ORTHOGONAL) {
		f32 window_width = (f32)renderer->width;
		f32 window_height = (f32)renderer->height;

		glm_ortho(
			-window_width/2.f,
			window_width/2.f,
			-window_height/2.f,
			window_height/2.f,
			.1f, 100.f,
			camera->projection_matrix
		);

	}
	else if(ptype == PROJECTIONTYPE_PERSPECTIVE) {
		f32 fov = M_PI_4;
		glm_perspective(
			fov,
			(f32)renderer->width/(f32)renderer->height,
			near, far,
			camera->projection_matrix
		);
	}
	else { return error_msg(FERN_UNSUPPORTED_FORMAT, "Invalid projectiontype"); }

	return FERN_ERROR_OK;
}
