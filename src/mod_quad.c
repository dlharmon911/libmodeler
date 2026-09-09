#include "libmodeler/mod_internal.h"

void mod_quad_init(mod_quad_t* quad, const o_vertex_t* v1, const o_vertex_t* v2, const o_vertex_t* v3, const o_vertex_t* v4)
{
	if (NULL == quad || NULL == v1 || NULL == v2 || NULL == v3 || NULL == v4)
	{
		return;
	}

	memcpy(&quad->v1, v1, sizeof(o_vertex_t));
	memcpy(&quad->v2, v2, sizeof(o_vertex_t));
	memcpy(&quad->v3, v3, sizeof(o_vertex_t));
	memcpy(&quad->v4, v4, sizeof(o_vertex_t));
}

void mod_quad_init_v(mod_quad_t* quad, o_vector3_t v1, o_vector3_t v2, o_vector3_t v3, o_vector3_t v4)
{
	if (NULL == quad)
	{
		return;
	}

	o_vertex_t vertex1 = { v1, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} };
	o_vertex_t vertex2 = { v2, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} };
	o_vertex_t vertex3 = { v3, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} };
	o_vertex_t vertex4 = { v4, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} };

	mod_quad_init(quad, &vertex1, &vertex2, &vertex3, &vertex4);
}

void mod_quad_init_p(mod_quad_t* quad, o_vector3_t center, o_vector3_t corner)
{
	if (NULL == quad)
	{
		return;
	}
	
	o_vector3_t half_size = { fabsf(corner.m_x - center.m_x), fabsf(corner.m_y - center.m_y), fabsf(corner.m_z - center.m_z) };

	o_vector3_t v1 = { center.m_x - half_size.m_x, center.m_y - half_size.m_y, center.m_z - half_size.m_z };
	o_vector3_t v2 = { center.m_x + half_size.m_x, center.m_y - half_size.m_y, center.m_z - half_size.m_z };
	o_vector3_t v3 = { center.m_x + half_size.m_x, center.m_y + half_size.m_y, center.m_z - half_size.m_z };
	o_vector3_t v4 = { center.m_x - half_size.m_x, center.m_y + half_size.m_y, center.m_z - half_size.m_z };
	
	mod_quad_init_v(quad, v1, v2, v3, v4);
}

void mod_quad_recolor(mod_quad_t* quad, o_color_t color)
{
	if (NULL == quad)
	{
		return;
	}

	quad->v1.m_color = color;
	quad->v2.m_color = color;
	quad->v3.m_color = color;
	quad->v4.m_color = color;
}