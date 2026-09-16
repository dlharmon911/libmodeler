#include "libmodeler/mod_internal.h"

typedef struct mod_cuboid_context_t
{
	mod_model_t* m_face;
	mod_model_t* m_cuboid;
} mod_cuboid_context_t;

static void mod_cuboid_context_zero(mod_cuboid_context_t* context)
{
	if (NULL == context)
	{
		return;
	}

	context->m_face = NULL;
	context->m_cuboid = NULL;
}

static void _mod_cuboid_context_destroy(mod_cuboid_context_t* context)
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

    if (context->m_cuboid)
	{
		mod_model_destroy(context->m_cuboid);
		context->m_cuboid = NULL;
	}
}


static int32_t _mod_cuboid_generate(mod_cuboid_context_t* context, float width, float height, float depth, bool merge_vertices)
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

    if (NULL == context)
    {
		return -1;
    }

	meta_data.m_scale = ogle_vector3_mul_f(meta_data.m_scale, width, height, depth);

    context->m_face = mod_model_create_empty();
    if (NULL == context->m_face)
    {
        return -1;
    }

    context->m_cuboid = mod_model_create_empty();
    if (NULL == context->m_cuboid)
    {
        return -1;
    }

    mod_quad_init_p(&quad, 1.0f, 1.0f);

    if (!mod_model_add_quad(context->m_face, &quad, false))
    {
        return -1;
    }

    meta_data.m_translation = (o_vector3_t){ 0.0f, 0.0f, 0.5f };
    mod_model_add_model_with_meta_data(context->m_cuboid, context->m_face, &meta_data, merge_vertices);

    meta_data.m_translation = (o_vector3_t){ 0.0f, 0.0f, -0.5f };
    meta_data.m_rotation = (o_vector3_t){ 0.0f, 0.5f, 0.0f };
    mod_model_add_model_with_meta_data(context->m_cuboid, context->m_face, &meta_data, merge_vertices);

    meta_data.m_translation = (o_vector3_t){ 0.5f , 0.0f, 0.0f };
    meta_data.m_rotation = (o_vector3_t){ 0.0f, 0.75f, 0.0f };
    mod_model_add_model_with_meta_data(context->m_cuboid, context->m_face, &meta_data, merge_vertices);

    meta_data.m_translation = (o_vector3_t){ -0.5f , 0.0f, 0.0f };
    meta_data.m_rotation = (o_vector3_t){ 0.0f, 0.25f, 0.0f };
    mod_model_add_model_with_meta_data(context->m_cuboid, context->m_face, &meta_data, merge_vertices);

    meta_data.m_translation = (o_vector3_t){ 0.0f, 0.5f , 0.0f };
    meta_data.m_rotation = (o_vector3_t){ 0.75f, 0.0f, 0.0f };
    mod_model_add_model_with_meta_data(context->m_cuboid, context->m_face, &meta_data, merge_vertices);

    meta_data.m_translation = (o_vector3_t){ 0.0f, -0.5f , 0.0f };
    meta_data.m_rotation = (o_vector3_t){ 0.25f, 0.0f, 0.0f };
    mod_model_add_model_with_meta_data(context->m_cuboid, context->m_face, &meta_data, merge_vertices);

    mod_model_recalculate_normals(context->m_cuboid);

    return 0;
}


mod_model_t* mod_cuboid_generate(float width, float height, float depth, bool merge_vertices)
{
    mod_model_t* model = NULL;
    mod_cuboid_context_t context =
    {
        .m_face = NULL,
        .m_cuboid = NULL
    };

	mod_cuboid_context_zero(&context);

	if (_mod_cuboid_generate(&context, width, height, depth, merge_vertices) != 0)
	{
        model = NULL;
	}
    else
    {
        model = context.m_cuboid;
        context.m_cuboid = NULL;
    }

	_mod_cuboid_context_destroy(&context);

	return model;
}
