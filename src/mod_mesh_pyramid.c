#include "libmodeler/mod_internal.h"

typedef struct mod_pyramid_context_t
{
	mod_model_t* m_base;
	mod_model_t* m_face;
	mod_model_t* m_pyramid;
} mod_pyramid_context_t;

static void mod_pyramid_context_zero(mod_pyramid_context_t* context)
{
	if (NULL == context)
	{
		return;
	}
    
	context->m_base = NULL;
	context->m_face = NULL;
	context->m_pyramid = NULL;
}

static void _mod_pyramid_context_destroy(mod_pyramid_context_t* context)
{
	if (NULL == context)
	{
		return;
	}

	if (context->m_base)
	{
		mod_model_destroy(context->m_base);
		context->m_base = NULL;
	}

    if (context->m_face)
	{
		mod_model_destroy(context->m_face);
		context->m_face = NULL;
	}

    if (context->m_pyramid)
	{
		mod_model_destroy(context->m_pyramid);
		context->m_pyramid = NULL;
	}
}


static int32_t _mod_pyramid_generate2(mod_pyramid_context_t* context, float radius, float height, size_t sides, bool withbase, bool merge_vertices)
{
	mod_triangle_t base_triangle = { 0 };
	mod_triangle_t face_triangle = { 0 };
	mod_transform_t meta_data =
	{
		.m_translation = { 0.0f, 0.0f, 0.0f },
		.m_rotation = { 0.0f, 0.0f, 0.0f },
		.m_scale = { 1.0f, 1.0f, 1.0f },
		.m_color = { 1.0f, 1.0f, 1.0f, 1.0f }
	};
	float step = OGLE_MATH_TAU / (float)sides;

	if (sides < 3 || sides > 32)
	{
		return -1;
	}

    if (NULL == context)
    {
		return -1;
    }

	context->m_base = mod_model_create_empty();
	if (NULL == context->m_base)
	{
		return -1;
	}

    context->m_face = mod_model_create_empty();
    if (NULL == context->m_face)
    {
        return -1;
    }

    context->m_pyramid = mod_model_create_empty();
    if (NULL == context->m_pyramid)
    {
        return -1;
    }

	o_vector3_t vec1 = { 0.0f, height / 2.0f, 0.0f };
	o_vector3_t vec2 = { 0.0f, height / -2.0f, 0.0f };
	o_vector3_t vec3 = { radius * cosf(step * 1.0f), height / -2.0f, radius * sinf(step * 1.0f) };
	o_vector3_t vec4 = { radius * cosf(step * 2.0f), height / -2.0f, radius * sinf(step * 2.0f) };

	o_vertex_t v1 = { .m_position = vec1, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
	o_vertex_t v2 = { .m_position = vec2, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
	o_vertex_t v3 = { .m_position = vec3, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
	o_vertex_t v4 = { .m_position = vec4, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };

	mod_triangle_init(&face_triangle, &v1, &v4, &v3);
	mod_triangle_init(&base_triangle, &v2, &v3, &v4);

    if (!mod_model_add_triangle(context->m_base, &base_triangle, merge_vertices))
    {
        return -1;
    }

	if (!mod_model_add_triangle(context->m_face, &face_triangle, merge_vertices))
	{
		return -1;
	}

	for (size_t i = 1; i <= sides; ++i)
	{
		float angle = ((float)i / (float)sides);

		meta_data.m_rotation = (o_vector3_t){ 0.0f, angle, 0.0f };
		mod_model_add_model_with_meta_data(context->m_pyramid, context->m_face, &meta_data, merge_vertices);
		if (withbase)
		{
			mod_model_add_model_with_meta_data(context->m_pyramid, context->m_base, &meta_data, merge_vertices);
		}
	}

    mod_model_recalculate_normals(context->m_pyramid);

    return 0;
}

static mod_model_t* _mod_pyramid_generate(float radius, float height, size_t sides, bool withbase, bool merge_vertices)
{
	mod_model_t* model = NULL;
	mod_pyramid_context_t context =
	{
		.m_face = NULL,
		.m_pyramid = NULL
	};

	mod_pyramid_context_zero(&context);

	if (_mod_pyramid_generate2(&context, radius, height, sides, withbase, merge_vertices) != 0)
	{
		model = NULL;
	}
	else
	{
		model = context.m_pyramid;
		context.m_pyramid = NULL;
	}

	_mod_pyramid_context_destroy(&context);

	return model;
}

mod_model_t* mod_pyramid_generate(float radius, float height, size_t sides, bool merge_vertices)
{
	return _mod_pyramid_generate(radius, height, sides, true, merge_vertices);
}

mod_model_t* mod_pyramid_generate_baseless(float radius, float height, size_t sides, bool merge_vertices)
{
	return _mod_pyramid_generate(radius, height, sides, false, merge_vertices);
}
