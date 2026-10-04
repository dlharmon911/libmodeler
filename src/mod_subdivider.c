#include "libmodeler/mod_internal.h"

static void _mod_model_subdivide_find_middle(o_vertex_t* middle, const o_vertex_t* vertex1, const o_vertex_t* vertex2)
{
	memcpy(middle, vertex1, sizeof(o_vertex_t));

	middle->m_position.m_x = 0.5f * (vertex1->m_position.m_x + vertex2->m_position.m_x);
	middle->m_position.m_y = 0.5f * (vertex1->m_position.m_y + vertex2->m_position.m_y);
	middle->m_position.m_z = 0.5f * (vertex1->m_position.m_z + vertex2->m_position.m_z);

	middle->m_uv.m_x = 0.5f * (vertex1->m_uv.m_x + vertex2->m_uv.m_x);
	middle->m_uv.m_y = 0.5f * (vertex1->m_uv.m_y + vertex2->m_uv.m_y);

	middle->m_color.r = 0.5f * (vertex1->m_color.r + vertex2->m_color.r);
	middle->m_color.g = 0.5f * (vertex1->m_color.g + vertex2->m_color.g);
	middle->m_color.b = 0.5f * (vertex1->m_color.b + vertex2->m_color.b);
	middle->m_color.a = 0.5f * (vertex1->m_color.a + vertex2->m_color.a);
}

static int32_t _mod_model_subdivide_add_vertex(mod_model_t* out_model, const o_vertex_t* vertex)
{
	if (NULL == out_model || NULL == vertex)
	{
		return -1;
	}

	int32_t size = (int32_t)ogle_darray_size(out_model->m_vertices);

	if (!ogle_darray_push_back(&out_model->m_vertices, vertex))
	{
		return -1;
	}

	return size;
}

static int32_t _mod_model_subdivide_add_indices(mod_model_t* model, int32_t i0, int32_t i1, int32_t i2)
{
	if (!ogle_darray_push_back(&model->m_indices, &i0) ||
		!ogle_darray_push_back(&model->m_indices, &i1) ||
		!ogle_darray_push_back(&model->m_indices, &i2))
	{
		return -1;
	}

	return 0;
}

static int32_t _mod_model_subdivide_edges(const mod_model_t* model, mod_model_t* out_model)
{
	size_t index_count = 0;
	size_t triangle_count = 0;

	if (NULL == model || NULL == out_model)
	{
		return -1;
	}

	index_count = ogle_darray_size(model->m_indices);
	triangle_count = index_count / 3;
	int32_t f[6] = { 0 };
	o_vertex_t vertex[6] = { 0 };

	for (size_t t = 0; t < triangle_count; ++t)
	{
		for (size_t i = 0; i < 3; ++i)
		{
			f[i] = model->m_indices[t * 3 + i];
			memcpy(vertex + i, model->m_vertices + f[i], sizeof(o_vertex_t));
		}

		_mod_model_subdivide_find_middle(vertex + 3, vertex + 0, vertex + 1);
		_mod_model_subdivide_find_middle(vertex + 4, vertex + 1, vertex + 2);
		_mod_model_subdivide_find_middle(vertex + 5, vertex + 2, vertex + 0);

		f[3] = _mod_model_subdivide_add_vertex(out_model, vertex + 3);
		f[4] = _mod_model_subdivide_add_vertex(out_model, vertex + 4);
		f[5] = _mod_model_subdivide_add_vertex(out_model, vertex + 5);

		if (f[3] < 0 || f[4] < 0 || f[5] < 0)
		{
			return -1;
		}
		
		_mod_model_subdivide_add_indices(out_model, f[0], f[3], f[5]);
		_mod_model_subdivide_add_indices(out_model, f[3], f[1], f[4]);
		_mod_model_subdivide_add_indices(out_model, f[4], f[2], f[5]);
		_mod_model_subdivide_add_indices(out_model, f[3], f[4], f[5]);
	}

	return 0;
}

static int32_t _mod_model_subdivide(const mod_model_t* model, mod_model_t** out_model)
{
	if (NULL == model || NULL == out_model)
	{
		return -1;
	}

	*out_model = mod_model_clone(model);

	if (NULL == *out_model)
	{
		return -1;
	}

	ogle_darray_clear((*out_model)->m_indices);

	if (_mod_model_subdivide_edges(model, *out_model) < 0)
	{
		mod_model_destroy(*out_model);
		return -1;
	}

	mod_model_recalculate_normals(*out_model);

	return 0;
}

int32_t mod_model_subdivide(const mod_model_t* model, mod_model_t** out_model, int32_t levels)
{
	mod_model_t* new_model = NULL;

	if (NULL == model || NULL == out_model || levels < 0 || levels > 5)
	{
		return -1;
	}

	if (_mod_model_subdivide(model, out_model) < 0)
	{
		if (NULL != *out_model)
		{
			mod_model_destroy(*out_model);
		}

		return -1;
	}

	mod_model_normalize_vertices(*out_model, 0.5f);

	for (int32_t i = 1; i < levels; ++i)
	{
		new_model = *out_model;

		if (_mod_model_subdivide(new_model, out_model) < 0)
		{
			if (NULL != new_model)
			{
				mod_model_destroy(new_model);
			}

			if (NULL != *out_model)
			{
				mod_model_destroy(*out_model);
			}
		}

		mod_model_normalize_vertices(*out_model, 0.5f);

		mod_model_destroy(new_model);

		new_model = *out_model;
	}



	return 0;
}

void mod_model_normalize_vertices(mod_model_t* model, float radius)
{
	if (NULL == model || radius < 0.0001)
	{
		return;
	}

	o_vertex_t* vertices = model->m_vertices;
	size_t count = ogle_darray_size(vertices);

	for (size_t i = 0; i < count; ++i)
	{
		float vlength = ogle_vector3_length(vertices[i].m_position);

		if (vlength != radius)
		{
			float mulf = radius / vlength;

			vertices[i].m_position = ogle_vector3_mul_ff(vertices[i].m_position, mulf);
		}
	}
}
