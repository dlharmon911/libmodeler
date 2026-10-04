#include "libmodeler/mod_internal.h"

typedef struct mod_tile_context_t
{
	mod_model_t* m_top;
	mod_model_t* m_base;
	mod_model_t* m_side;
	mod_model_t* m_tile;
	o_vector3_t m_points[4];
} mod_tile_context_t;

static void mod_tile_context_zero(mod_tile_context_t* context)
{
	if (NULL == context)
	{
		return;
	}
    
	context->m_top = NULL;
	context->m_side = NULL;
	context->m_tile = NULL;
}

static void _mod_tile_context_destroy(mod_tile_context_t* context)
{
	if (NULL == context)
	{
		return;
	}

	if (context->m_top)
	{
		mod_model_destroy(context->m_top);
		context->m_top = NULL;
	}

    if (context->m_side)
	{
		mod_model_destroy(context->m_side);
		context->m_side = NULL;
	}

    if (context->m_tile)
	{
		mod_model_destroy(context->m_tile);
		context->m_tile = NULL;
	}
}

static int32_t _mod_tile_generate_top(mod_tile_context_t* context, float width, float height, float depth, float radius, bool merge_vertices)
{
	mod_quad_t quad = { 0 };

	float x = (width * 0.5f) - radius;
	float y = (height * 0.5f) - radius;
	float z = depth * 0.5f;

	o_vector3_t vec1 = (o_vector3_t){ -x, -y, z };
	o_vector3_t vec2 = (o_vector3_t){ x, -y, z };
	o_vector3_t vec3 = (o_vector3_t){ x, y, z };
	o_vector3_t vec4 = (o_vector3_t){ -x, y, z };

	o_vertex_t v1 = { .m_position = vec1, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
	o_vertex_t v2 = { .m_position = vec2, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 1.0f, 0.0f } };
	o_vertex_t v3 = { .m_position = vec3, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 1.0f, 1.0f } };
	o_vertex_t v4 = { .m_position = vec4, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 1.0f } };

	mod_quad_init(&quad, &v1, &v2, &v3, &v4);

	if (!mod_model_add_quad(context->m_top, &quad, merge_vertices))
	{
		return -1;
	}
	
	return 0;
}

static int32_t _mod_tile_generate_side(mod_tile_context_t* context, float width, float height, float depth, float radius, bool merge_vertices)
{
	mod_quad_t quad = { 0 };
	mod_triangle_t tri = { 0 };

	float w1 = (width * 0.5f) - radius;
	float w2 = (width * 0.5f);
	float h1 = (height * 0.5f) - radius;
	float h2 = (height * 0.5f);
	float x = w1;
	float y = h1;
	float z1 = depth * 0.5f;
	float z2 = depth * 0.45f;
	float z3 = depth * 0.0f;

	o_vector3_t vec1 = (o_vector3_t){ -x, -y, z1 };
	o_vector3_t vec2 = (o_vector3_t){ -x, y, z1 };
	o_vector3_t vec3 = (o_vector3_t){ vec1.m_x - radius, vec1.m_y, z2 };
	o_vector3_t vec4 = (o_vector3_t){ vec2.m_x - radius, vec2.m_y, z2 };
	o_vector3_t vec5 = (o_vector3_t){ vec1.m_x - radius, vec1.m_y, z3 };
	o_vector3_t vec6 = (o_vector3_t){ vec2.m_x - radius, vec2.m_y, z3 };

	o_vertex_t v1 = { .m_position = vec1, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
	o_vertex_t v2 = { .m_position = vec2, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 1.0f, 0.0f } };
	o_vertex_t v3 = { .m_position = vec3, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
	o_vertex_t v4 = { .m_position = vec4, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 1.0f, 0.0f } };
	o_vertex_t v5 = { .m_position = vec5, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 0.0f, 0.0f } };
	o_vertex_t v6 = { .m_position = vec6, .m_color = { 1.0f, 1.0f, 1.0f, 1.0f }, .m_uv = { 1.0f, 0.0f } };

	mod_quad_init(&quad, &v1, &v2, &v4, &v3);

	if (!mod_model_add_quad(context->m_side, &quad, merge_vertices))
	{
		return -1;
	}

	mod_quad_init(&quad, &v3, &v4, &v6, &v5);

	if (!mod_model_add_quad(context->m_side, &quad, merge_vertices))
	{
		return -1;
	}

	float angle = 0.5f;
	float step = 0.25f / (float)8;
	for (int i = 1; i <= 8; ++i)
	{
		angle -= step;

		v4.m_position = v3.m_position;
		v6.m_position = v5.m_position;

		v3.m_position.m_x = v5.m_position.m_x = vec1.m_x + radius * cosf(angle * OGLE_MATH_TAU);
		v3.m_position.m_y = v5.m_position.m_y = vec1.m_y - radius * sinf(angle * OGLE_MATH_TAU);

		mod_quad_init(&quad, &v3, &v4, &v6, &v5);

		if (!mod_model_add_quad(context->m_side, &quad, merge_vertices))
		{
			return -1;
		}

		mod_triangle_init(&tri, &v1, &v4, &v3);

		if (!mod_model_add_triangle(context->m_side, &tri, merge_vertices))
		{
			return -1;
		}
	}

	return 0;
}

static int32_t _mod_tile_generate(mod_tile_context_t* context, float width, float height, float depth, float radius, bool merge_vertices)
{
	mod_transform_t meta_data =
	{
		.m_translation = { 0.0f, 0.0f, 0.0f },
		.m_rotation = { 0.0f, 0.0f, 0.0f },
		.m_scale = { 1.0f, 1.0f, 1.0f },
		.m_color = { 1.0f, 1.0f, 1.0f, 1.0f }
	};
	size_t sides = 8;
	float step = OGLE_MATH_TAU / (float)sides;

    if (NULL == context)
    {
		return -1;
    }

	context->m_top = mod_model_create_empty();
	if (NULL == context->m_top)
	{
		return -1;
	}

    context->m_side = mod_model_create_empty();
    if (NULL == context->m_side)
    {
        return -1;
    }

	context->m_base = mod_model_create_empty();
	if (NULL == context->m_base)
	{
		return -1;
	}

    context->m_tile = mod_model_create_empty();
    if (NULL == context->m_tile)
    {
        return -1;
    }

	if (_mod_tile_generate_top(context, width, height, depth, radius, merge_vertices) != 0)
	{
		return -1;
	}

	if (!mod_model_add_model(context->m_tile, context->m_top, merge_vertices))
	{
		return -1;
	}

	if (_mod_tile_generate_side(context, width, height, depth, radius, merge_vertices) != 0)
	{
		return -1;
	}

	if (!mod_model_add_model(context->m_base, context->m_side, merge_vertices))
	{
		return -1;
	}

	mod_model_rotate(context->m_side, MOD_Z_ROTATION, 0.25f);
	if (!mod_model_add_model(context->m_base, context->m_side, merge_vertices))
	{
		return -1;
	}

	mod_model_rotate(context->m_side, MOD_Z_ROTATION, 0.25f);
	if (!mod_model_add_model(context->m_base, context->m_side, merge_vertices))
	{
		return -1;
	}

	mod_model_rotate(context->m_side, MOD_Z_ROTATION, 0.25f);
	if (!mod_model_add_model(context->m_base, context->m_side, merge_vertices))
	{
		return -1;
	}

	if (!mod_model_add_model(context->m_tile, context->m_base, merge_vertices))
	{
		return -1;
	}

	mod_model_rotate(context->m_base, MOD_Y_ROTATION, 0.5f);
	if (!mod_model_add_model(context->m_tile, context->m_base, merge_vertices))
	{
		return -1;
	}

	size_t vertex_count = mod_model_get_vertex_count(context->m_tile);
	for (size_t i = 0; i < vertex_count; ++i)
	{
		o_vertex_t* vertex = mod_model_get_vertex(context->m_tile, i);
		if (vertex)
		{
			vertex->m_uv = (o_vector2_t){ -100.0f, -100.0f };
		}
	}

    mod_model_recalculate_normals(context->m_tile);

    return 0;
}

mod_model_t* mod_tile_generate(float width, float height, float depth, float radius, bool merge_vertices)
{
	mod_model_t* model = NULL;
	mod_tile_context_t context =
	{
		.m_top = NULL,
		.m_side = NULL,
		.m_tile = NULL
	};

	mod_tile_context_zero(&context);

	if (radius > 0.45f)
	{
		radius = 0.45f;
	}

	if (_mod_tile_generate(&context, 1.0f, 1.0f, 1.0f, radius, merge_vertices) != 0)
	{
		model = NULL;
	}
	else
	{
		model = context.m_tile;
		context.m_tile = NULL;
		mod_model_scale_f(model, width, height, depth);
	}

	_mod_tile_context_destroy(&context);

	return model;
}

