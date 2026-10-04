#include "libmodeler/mod_internal.h"

void mod_triangle_init(mod_triangle_t* triangle, const o_vertex_t* v1, const o_vertex_t* v2, const o_vertex_t* v3)
{
	if (NULL == triangle || NULL == v1 || NULL == v2 || NULL == v3)
	{
		return;
	}

	if (v1 == v2 || v1 == v3 || v2 == v3)
	{
		return;
	}

	memcpy(&triangle->m_vertex[0], v1, sizeof(o_vertex_t));
	memcpy(&triangle->m_vertex[1], v2, sizeof(o_vertex_t));
	memcpy(&triangle->m_vertex[2], v3, sizeof(o_vertex_t));
}

void mod_triangle_init_v(mod_triangle_t* triangle, o_vector3_t v1, o_vector3_t v2, o_vector3_t v3)
{
	if (NULL == triangle)
	{
		return;
	}
	
	o_vertex_t vertex1 = { v1, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} };
	o_vertex_t vertex2 = { v2, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} };
	o_vertex_t vertex3 = { v3, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} };

	mod_triangle_init(triangle, &vertex1, &vertex2, &vertex3);
}

void mod_triangle_recolor(mod_triangle_t* triangle, o_color_t color)
{
	if (NULL == triangle)
	{
		return;
	}

	triangle->m_vertex[0].m_color = color;
	triangle->m_vertex[1].m_color = color;
	triangle->m_vertex[2].m_color = color;
}
