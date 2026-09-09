#include "libmodeler/mod_internal.h"

static void mod_model_zero(mod_model_t* model)
{
	if (NULL == model)
	{
		return;
	}

	model->m_vertices = NULL;
	model->m_indices = NULL;
}

static int32_t _mod_model_initialize(mod_model_t* model)
{
	if (NULL == model)
	{
		return -1;
	}

	model->m_vertices = ogle_darray_create(sizeof(o_vertex_t));
	if (NULL == model->m_vertices)
	{
		return -1;
	}

	model->m_indices = ogle_darray_create(sizeof(int32_t));
	if (NULL == model->m_indices)
	{
		return -1;
	}

	return 0;
}

mod_model_t* mod_model_create_empty()
{
	mod_model_t* model = (mod_model_t*)ogle_malloc(sizeof(mod_model_t));

	if (NULL == model)
	{
		return NULL;
	}

	mod_model_zero(model);

	if (_mod_model_initialize(model) != 0)
	{
		mod_model_destroy(model);
		model = NULL;
	}

	return model;
}

mod_model_t* mod_model_create(const o_vertex_t* vertices, size_t vertex_count, const int32_t* indices, size_t index_count)
{
	mod_model_t* model = mod_model_create_empty();
	
	if (NULL == model)
	{
		return NULL;
	}
	
	if (!mod_model_add_shape(model, vertices, vertex_count, indices, index_count, false))
	{
		mod_model_destroy(model);
		model = NULL;
	}

	return model;
}

void mod_model_destroy(mod_model_t* model)
{
	if (NULL == model)
	{
		return;
	}

	if (NULL != model->m_vertices)
	{
		ogle_darray_destroy(&model->m_vertices);
		model->m_vertices = NULL;
	}

	if (NULL != model->m_indices)
	{
		ogle_darray_destroy(&model->m_indices);
		model->m_indices = NULL;
	}

	ogle_free(model);
}

o_vertex_t* mod_model_get_vertices(mod_model_t* model)
{
	if (NULL == model || NULL == model->m_vertices)
	{
		return NULL;
	}

	return model->m_vertices;
}

const o_vertex_t* mod_model_get_vertices_const(const mod_model_t* model)
{
	if (NULL == model || NULL == model->m_vertices)
	{
		return NULL;
	}

	return model->m_vertices;
}

size_t mod_model_get_vertex_count(const mod_model_t* model)
{
	if (NULL == model || NULL == model->m_vertices)
	{
		return 0;
	}

	return ogle_darray_size(model->m_vertices);
}

int32_t* mod_model_get_indices(mod_model_t* model)
{
	if (NULL == model || NULL == model->m_indices)
	{
		return NULL;
	}

	return model->m_indices;
}

const int32_t* mod_model_get_indices_const(const mod_model_t* model)
{
	if (NULL == model || NULL == model->m_indices)
	{
		return NULL;
	}

	return model->m_indices;
}

size_t mod_model_get_index_count(const mod_model_t* model)
{
	if (NULL == model || NULL == model->m_indices)
	{
		return 0;
	}

	return ogle_darray_size(model->m_indices);
}

bool mod_model_push_vertex(mod_model_t* model, const o_vertex_t* vertex)
{
	if (NULL == model || NULL == model->m_vertices || NULL == vertex)
	{
		return false;
	}

	if (!ogle_darray_push_back(&model->m_vertices, vertex))
	{
		return false;
	}

	return true;
}

bool mod_model_push_vertices(mod_model_t* model, const o_vertex_t* vertices, size_t count)
{
	if (NULL == model || NULL == model->m_vertices || NULL == vertices || count == 0)
	{
		return false;
	}

	for (size_t i = 0; i < count; ++i)
	{
		if (!ogle_darray_push_back(&model->m_vertices, vertices + i))
		{
			return false;
		}
	}

	return true;
}

bool mod_model_pop_vertex(mod_model_t* model)
{
	if (NULL == model || NULL == model->m_vertices)
	{
		return false;
	}

	size_t vertex_count = ogle_darray_size(model->m_vertices);

	if (0 == vertex_count)
	{
		return false;
	}

	if (!ogle_darray_pop_back(&model->m_vertices))
	{
		return false;
	}

	return true;
}

bool mod_model_pop_vertices(mod_model_t* model, size_t count)
{
	if (NULL == model || NULL == model->m_vertices)
	{
		return false;
	}

	size_t vector_count = ogle_darray_size(model->m_vertices);

	if (count > vector_count)
	{
		count = vector_count;
	}

	for (size_t i = 0; i < count; ++i)
	{
		if (!ogle_darray_pop_back(&model->m_vertices))
		{
			return false;
		}
	}

	return true;
}

bool mod_model_push_index(mod_model_t* model, int32_t index)
{
	if (NULL == model || NULL == model->m_indices)
	{
		return false;
	}

	if (!ogle_darray_push_back(&model->m_indices, &index))
	{
		return false;
	}

	return true;
}

bool mod_model_push_indices(mod_model_t* model, const int32_t* indices, size_t count)
{
	if (NULL == model || NULL == model->m_indices || NULL == indices || count == 0)
	{
		return false;
	}
	
	for (size_t i = 0; i < count; ++i)
	{
		if (!ogle_darray_push_back(&model->m_indices, indices + i))
		{
			return false;
		}
	}

	return true;
}

bool mod_model_pop_index(mod_model_t* model)
{
	if (NULL == model || NULL == model->m_indices)
	{
		return false;
	}

	size_t index_count = ogle_darray_size(model->m_indices);

	if (0 == index_count)
	{
		return false;
	}

	if (!ogle_darray_pop_back(&model->m_indices))
	{
		return false;
	}

	return true;
}

bool mod_model_pop_indices(mod_model_t* model, size_t count)
{
	if (NULL == model || NULL == model->m_indices)
	{
		return false;
	}

	size_t index_count = ogle_darray_size(model->m_indices);

	if (count > index_count)
	{
		count = index_count;
	}

	for (size_t i = 0; i < count; ++i)
	{
		if (!ogle_darray_pop_back(&model->m_indices))
		{
			return false;
		}
	}

	return true;
}

static int32_t mod_model_find_vertex(const mod_model_t* model, const o_vertex_t* vertex)
{
	if (NULL == model || NULL == model->m_vertices || NULL == vertex)
	{
		return -1;
	}

	size_t vertex_count = ogle_darray_size(model->m_vertices);

	for (size_t i = 0; i < vertex_count; ++i)
	{
		const o_vertex_t* existing_vertex = &model->m_vertices[i];

		if (ogle_vector3_equal(existing_vertex->m_position, vertex->m_position))
		{
			return (int32_t)i;
		}
	}

	return -1;
}

static int32_t mod_model_add_vertex(mod_model_t* model, const o_vertex_t* vertex, bool merge_vertices)
{
	int32_t index = -1;

	if (NULL == model || NULL == vertex)
	{
		return -1;
	}

	if (merge_vertices)
	{
		index = mod_model_find_vertex(model, vertex);
	}

	if (index < 0)
	{
		index = (int32_t)mod_model_get_vertex_count(model);

		if (!mod_model_push_vertex(model, vertex))
		{
			index = -1;
		}
	}

	return index;
}

bool mod_model_add_triangle(mod_model_t* model, const mod_triangle_t* triangle, bool merge_vertices)
{
	int32_t indices[3] = { -1, -1, -1 };
	if (NULL == model || NULL == triangle)
	{
		return false;
	}

	indices[0] = mod_model_add_vertex(model, &triangle->v1, merge_vertices);
	indices[1] = mod_model_add_vertex(model, &triangle->v2, merge_vertices);
	indices[2] = mod_model_add_vertex(model, &triangle->v3, merge_vertices);

	if (indices[0] < 0 || indices[1] < 0 || indices[2] < 0)
	{
		return false;
	}

	if (!mod_model_push_indices(model, indices, 3))
	{
		return false;
	}

	return true;
}

bool mod_model_add_triangle_v(mod_model_t* model, const o_vertex_t* v1, const o_vertex_t* v2, const o_vertex_t* v3, bool merge_vertices)
{
	if (NULL == model || NULL == v1 || NULL == v2 || NULL == v3)
	{
		return false;
	}
	mod_triangle_t triangle = { 0 };
	triangle.v1 = *v1;
	triangle.v2 = *v2;
	triangle.v3 = *v3;
	return mod_model_add_triangle(model, &triangle, merge_vertices);
}

bool mod_model_add_quad(mod_model_t* model, const mod_quad_t* quad, bool merge_vertices)
{
	if (NULL == model || NULL == quad)
	{
		return false;
	}

	if (!mod_model_add_triangle_v(model, &quad->v1, &quad->v2, &quad->v3, merge_vertices) ||
		!mod_model_add_triangle_v(model, &quad->v1, &quad->v3, &quad->v4, merge_vertices))
	{
		return false;
	}

	return true;
}

bool mod_model_add_quad_v(mod_model_t* model, const o_vertex_t* v1, const o_vertex_t* v2, const o_vertex_t* v3, const o_vertex_t* v4, bool merge_vertices)
{
	if (NULL == model || NULL == v1 || NULL == v2 || NULL == v3 || NULL == v4)
	{
		return false;
	}
	mod_quad_t quad = { 0 };
	quad.v1 = *v1;
	quad.v2 = *v2;
	quad.v3 = *v3;
	quad.v4 = *v4;
	return mod_model_add_quad(model, &quad, merge_vertices);
}

bool mod_model_add_model(mod_model_t* model, const mod_model_t* other_model, bool merge_vertices)
{
	if (NULL == model || NULL == other_model || NULL == other_model->m_vertices || NULL == other_model->m_indices)
	{
		return false;
	}

	const o_vertex_t* vertices = other_model->m_vertices;
	const int32_t* indices = other_model->m_indices;
	size_t vertex_count = mod_model_get_vertex_count(other_model);

	if (0 == vertex_count)
	{
		return false;
	}

	size_t index_count = mod_model_get_index_count(other_model);

	if (0 == index_count)
	{
		return false;
	}

	return mod_model_add_shape(model, vertices, vertex_count, indices, index_count, merge_vertices);
}

bool mod_model_add_shape(mod_model_t* model, const o_vertex_t* vertices, size_t vertex_count, const int32_t* indices, size_t index_count, bool merge_vertices)
{
	if (NULL == model || NULL == vertices || NULL == indices || 0 == vertex_count || 0 == index_count)
	{
		return false;
	}

	for (size_t i = 0; i < index_count; i += 3)
	{
		const o_vertex_t* v1 = (vertices + i);
		const o_vertex_t* v2 = (vertices + i + 1);
		const o_vertex_t* v3 = (vertices + i + 2);

		if (!mod_model_add_triangle_v(model, v1, v2, v3, merge_vertices))
		{
			return false;
		}
	}

	return true;
}

void mod_model_rotate(mod_model_t* model, o_vector3_t rotation, float angle)
{
	mod_model_rotate_f(model, rotation.m_x, rotation.m_y, rotation.m_z, angle);
}

void mod_model_rotate_f(mod_model_t* model, float x, float y, float z, float angle)
{
	if (NULL == model || NULL == model->m_vertices)
	{
		return;
	}

	size_t vertex_count = mod_model_get_vertex_count(model);

	for (size_t i = 0; i < vertex_count; ++i)
	{
		o_vertex_t* vector = (model->m_vertices + i);
		o_vector3_t pos = vector->m_position;

		pos = ogle_vector3_rotate_yz(pos, x * angle);
		pos = ogle_vector3_rotate_xz(pos, y * angle);
		pos = ogle_vector3_rotate_xy(pos, z * angle);

		vector->m_position = pos;
	}
}

void mod_model_scale(mod_model_t* model, o_vector3_t scale)
{
	mod_model_scale_f(model, scale.m_x, scale.m_y, scale.m_z);
}

void mod_model_scale_f(mod_model_t* model, float x, float y, float z)
{
	if (NULL == model || NULL == model->m_vertices)
	{
		return;
	}

	size_t vertex_count = mod_model_get_vertex_count(model);

	for (size_t i = 0; i < vertex_count; ++i)
	{
		o_vertex_t* vector = (model->m_vertices + i);
		o_vector3_t pos = vector->m_position;

		pos = ogle_vector3_mul_f(pos, x, y, z);

		vector->m_position = pos;
	}
}

void mod_model_translate(mod_model_t* model, o_vector3_t translation)
{
	mod_model_translate_f(model, translation.m_x, translation.m_y, translation.m_z);
}

void mod_model_translate_f(mod_model_t* model, float x, float y, float z)
{
	if (NULL == model || NULL == model->m_vertices)
	{
		return;
	}

	size_t vertex_count = mod_model_get_vertex_count(model);

	for (size_t i = 0; i < vertex_count; ++i)
	{
		o_vertex_t* vector = (model->m_vertices + i);
		o_vector3_t pos = vector->m_position;

		pos = ogle_vector3_add_f(pos, x, y, z);

		vector->m_position = pos;
	}
}

void mod_model_recolor(mod_model_t* model, o_color_t color)
{
	mod_model_recolor_f(model, color.r, color.g, color.b, color.a);
}

void mod_model_recolor_f(mod_model_t* model, float r, float g, float b, float a)
{
	if (NULL == model || NULL == model->m_vertices)
	{
		return;
	}

	size_t vertex_count = mod_model_get_vertex_count(model);

	for (size_t i = 0; i < vertex_count; ++i)
	{
		o_vertex_t* vector = (model->m_vertices + i);
		vector->m_color.r = r;
		vector->m_color.g = g;
		vector->m_color.b = b;
		vector->m_color.a = a;
	}
}

static o_vector3_t mod_model_calculate_normal(const mod_model_t* model, size_t index)
{
	o_vector3_t result = { 0.0f, 0.0f, 0.0f };
	size_t count = 0;
	size_t triangle_count = mod_model_get_index_count(model) / 3;

	for (size_t i = 0; i < triangle_count; ++i)
	{
		size_t idx = i * 3;
		
		int32_t index1 = model->m_indices[idx];
		int32_t index2 = model->m_indices[idx + 1];
		int32_t index3 = model->m_indices[idx + 2];

		if (index1 == index || index2 == index || index3 == index)
		{
			o_vector3_t v1 = model->m_vertices[index1].m_position;
			o_vector3_t v2 = model->m_vertices[index2].m_position;
			o_vector3_t v3 = model->m_vertices[index3].m_position;
			o_vector3_t edge1 = ogle_vector3_sub(v2, v1);
			o_vector3_t edge2 = ogle_vector3_sub(v3, v1);
			o_vector3_t normal = ogle_vector3_cross(edge1, edge2);
			result = ogle_vector3_add(result, normal);
			count++;
		}
	}

	if (count > 0)
	{
		result = ogle_vector3_div_ff(result, (float)count);
		result = ogle_vector3_normalize(result);
	}

	return result;
}

void mod_model_recalculate_normals(mod_model_t* model)
{
	if (NULL == model || NULL == model->m_vertices || NULL == model->m_indices)
	{
		return;
	}

	size_t vertex_count = mod_model_get_vertex_count(model);
	size_t index_count = mod_model_get_index_count(model);

	for (size_t i = 0; i < vertex_count; ++i)
	{
		o_vector3_t normal = mod_model_calculate_normal(model, i);
		model->m_vertices[i].m_normal = normal;
	}
}

