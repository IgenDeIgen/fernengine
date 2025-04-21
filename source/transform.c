#include "../include/fern.h"
#include "cglm/affine-pre.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/quat.h"
#include "cglm/vec3.h"

void transform_translate(transform_t *transform, vec3 v) {
	glm_vec3_add(v, transform->position, transform->position);
	transform_apply(transform);
}

void transform_rotate(transform_t *transform, vec3 axis, f32 angle) {
	versor q;
	glm_quatv(q, angle, axis);
	glm_quat_mul(transform->rotation, q, transform->rotation);
	transform_apply(transform);
}

void transform_scale(transform_t *transform, vec3 s) {
	glm_vec3_add(s, transform->scale, transform->scale);
	transform_apply(transform);
}

void transform_scalef(transform_t *transform, f32 s) {
	glm_vec3_scale(transform->scale, s, transform->scale);
	transform_apply(transform);
}

void transform_apply(transform_t *transform) {
	glm_mat4_identity(transform->matrix);

	glm_scale(transform->matrix, transform->scale);

	mat4 rot = GLM_MAT4_IDENTITY_INIT;
	glm_quat_mat4(transform->rotation, rot);
	glm_mat4_mul(rot, transform->matrix, transform->matrix);

	glm_translate(transform->matrix, transform->position);
}
