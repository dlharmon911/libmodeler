#include "libmodeler/mod_internal.h"

typedef struct mod_tube_context_t
{
	mod_model_t* m_face;
	mod_model_t* m_tube;
} mod_tube_context_t;

static void mod_tube_context_zero(mod_tube_context_t* context)
{
	if (NULL == context)
	{
		return;
	}

	context->m_face = NULL;
	context->m_tube = NULL;
}

static void _mod_tube_context_destroy(mod_tube_context_t* context)
{
	if (NULL == context)
	{
		return;
	}
	
    if (context->m_face)
	{
		mod_model_destroy(context->m_face);
		context->m_face = NULL;
	}

    if (context->m_tube)
	{
		mod_model_destroy(context->m_tube);
		context->m_tube = NULL;
	}
}


static int32_t _mod_tube_generate(mod_tube_context_t* context, float radius, float width, size_t sides)
{
    mod_quad_t quad = { 0 };
    o_vector3_t center = { 0.0f, 0.0f, 0.0f };
    float angle = OGLE_MATH_PI_HALF;
    mod_transform_t meta_data =
    {
        .m_translation = { 0.0f, 0.0f, 0.0f },
        .m_rotation = { 0.0f, 0.0f, 0.0f },
        .m_scale = { 1.0f, 1.0f, 1.0f },
        .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }
    };
    bool merge = true;
    float step = OGLE_MATH_TAU / (float)sides;

    if (sides < 3 || sides > 32)
    {
        return -1;
    }

    if (NULL == context)
    {
		return -1;
    }

    context->m_face = mod_model_create_empty();
    if (NULL == context->m_face)
    {
        return -1;
    }

    context->m_tube = mod_model_create_empty();
    if (NULL == context->m_tube)
    {
        return -1;
    }

	o_vector3_t vec1 = (o_vector3_t){ width * -0.5f, radius * sinf(step * 2.0f), radius * cosf(step * 2.0f) };
	o_vector3_t vec2 = (o_vector3_t){ width * -0.5f, radius * sinf(step * 1.0f), radius * cosf(step * 1.0f) };
    o_vector3_t vec3 = (o_vector3_t){ width * 0.5f, radius * sinf(step * 1.0f), radius * cosf(step * 1.0f) };
    o_vector3_t vec4 = (o_vector3_t){ width * 0.5f, radius * sinf(step * 2.0f), radius * cosf(step * 2.0f) };

	o_vertex_t v1 = { .m_position = vec1, .m_normal = center, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
    o_vertex_t v2 = { .m_position = vec2, .m_normal = center, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
    o_vertex_t v3 = { .m_position = vec3, .m_normal = center, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
    o_vertex_t v4 = { .m_position = vec4, .m_normal = center, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };

    mod_quad_init(&quad, &v1, &v2, &v3, &v4);

    if (!mod_model_add_quad(context->m_face, &quad, false))
    {
        return -1;
    }

    for (size_t i = 1; i <= sides; ++i)
    {
        float angle = ((float)i / (float)sides);

        meta_data.m_rotation = (o_vector3_t){ angle, 0.0f, 0.0f };
        mod_model_add_model_with_meta_data(context->m_tube, context->m_face, &meta_data, merge);
    }

    mod_model_recalculate_normals(context->m_tube);

    return 0;
}


mod_model_t* mod_tube_generate(float radius, float width, size_t sides, bool merge_vertices)
{
    mod_model_t* model = NULL;
    mod_tube_context_t context =
    {
        .m_face = NULL,
        .m_tube = NULL
    };

	mod_tube_context_zero(&context);

	if (_mod_tube_generate(&context, radius, width, sides) != 0)
	{
        model = NULL;
	}
    else
    {
        model = context.m_tube;
        context.m_tube = NULL;
    }

	_mod_tube_context_destroy(&context);

	return model;
}
