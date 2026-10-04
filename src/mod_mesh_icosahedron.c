#include "libmodeler/mod_internal.h"

#define MOD_ICOSAHEDRON_T   (0.5f * (1.0 + sqrt(5)))

typedef struct mod_icosahedron_context_t
{
	mod_model_t* m_icosahedron;
} mod_icosahedron_context_t;

static void mod_icosahedron_context_zero(mod_icosahedron_context_t* context)
{
	if (NULL == context)
	{
		return;
	}

	context->m_icosahedron = NULL;
}

static void _mod_icosahedron_context_destroy(mod_icosahedron_context_t* context)
{
	if (NULL == context)
	{
		return;
	}

    if (context->m_icosahedron)
	{
		mod_model_destroy(context->m_icosahedron);
		context->m_icosahedron = NULL;
	}
}


static int32_t _mod_icosahedron_generate(mod_icosahedron_context_t* context, float radius, bool merge_vertices)
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
    const o_vector3_t vector[12] =
    {
        { -1.0f, MOD_ICOSAHEDRON_T, 0.0f },
        { 1.0f, MOD_ICOSAHEDRON_T, 0.0f },
        { -1.0f, -MOD_ICOSAHEDRON_T, 0.0f },
        { 1.0f, -MOD_ICOSAHEDRON_T, 0.0f },
        { 0.0f, -1.0f, MOD_ICOSAHEDRON_T },
        { 0.0f, 1.0f, MOD_ICOSAHEDRON_T },
        { 0.0f, -1.0f, -MOD_ICOSAHEDRON_T },
        { 0.0f, 1.0f, -MOD_ICOSAHEDRON_T },
        { MOD_ICOSAHEDRON_T, 0.0f, -1.0f },
        { MOD_ICOSAHEDRON_T, 0.0f, 1.0f },
        { -MOD_ICOSAHEDRON_T, 0.0f, -1.0f },
        { -MOD_ICOSAHEDRON_T, 0.0f, 1.0f }
    };
    const int32_t faces[20][3] =
    {
        { 0, 11, 5 },
        { 0, 5, 1 },
        { 0, 1, 7 },
        { 0, 7, 10 },
        { 0, 10, 11 },
        { 1, 5, 9 },
        { 5, 11, 4 },
        { 11, 10, 2 },
        { 10, 7, 6 },
        { 7, 1, 8 },
        { 3, 9, 4 },
        { 3, 4, 2 },
        { 3, 2, 6 },
        { 3, 6, 8 },
        { 3, 8, 9 },
        { 4, 9, 5 },
        { 2, 4, 11 },
        { 6, 2, 10 },
        { 8, 6, 7 },
        { 9, 8, 1 }
    };

    if (NULL == context)
    {
		return -1;
    }

    context->m_icosahedron = mod_model_create_empty();
    if (NULL == context->m_icosahedron)
    {
        return -1;
    }

    o_vertex_t v1 = { 0 };
    o_vertex_t v2 = { 0 };
    o_vertex_t v3 = { 0 };

	for (size_t i = 0; i < 20; ++i)
	{
		v1.m_position = ogle_vector3_mul_ff(vector[faces[i][0]], radius);
		v2.m_position = ogle_vector3_mul_ff(vector[faces[i][1]], radius);
		v3.m_position = ogle_vector3_mul_ff(vector[faces[i][2]], radius);

		mod_model_add_triangle_v(context->m_icosahedron, &v1, &v2, &v3, merge_vertices);
	}

    mod_model_scale_ff(context->m_icosahedron, radius);

	mod_model_recolor_f(context->m_icosahedron, 1.0f, 1.0f, 1.0f, 1.0f);
    mod_model_recalculate_normals(context->m_icosahedron);

    return 0;
}

mod_model_t* mod_icosahedron_generate(float radius, bool merge_vertices)
{
    mod_model_t* model = NULL;
    mod_icosahedron_context_t context =
    {
        .m_icosahedron = NULL
    };

	mod_icosahedron_context_zero(&context);

	if (_mod_icosahedron_generate(&context, radius, merge_vertices) != 0)
	{
        model = NULL;
	}
    else
    {
        model = context.m_icosahedron;
        context.m_icosahedron = NULL;
    }

	_mod_icosahedron_context_destroy(&context);

	return model;
}
