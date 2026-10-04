#include "libmodeler/mod_internal.h"

void mod_mesh_zero(mod_mesh_t* mesh)
{
	if (NULL == mesh)
	{
		return;
	}

	mesh->m_vertex_buffer = NULL;
	mesh->m_index_buffer = NULL;
}

static int32_t _mod_mesh_initialize(mod_mesh_t* mesh, ALLEGRO_VERTEX_DECL* decl, const o_vertex_t* vertices, size_t vertex_count, const int32_t* indices, size_t index_count, int32_t flags)
{
	if (NULL == mesh || NULL == decl || NULL == vertices || NULL == indices || vertex_count == 0 || index_count == 0)
	{
		return -1;
	}

	mesh->m_vertex_buffer = mod_vertex_buffer_create(decl, vertices, vertex_count, flags);
	if (NULL == mesh->m_vertex_buffer)
	{
		return -1;
	}

	mesh->m_index_buffer = mod_index_buffer_create(indices, index_count, flags);
	if (NULL == mesh->m_index_buffer)
	{
		return -1;
	}

	return 0;
}

mod_mesh_t* mod_mesh_create(ALLEGRO_VERTEX_DECL* decl, const mod_model_t* model, int32_t flags)
{
	if (NULL == decl || NULL == model)
	{
		return NULL;
	}

	const o_vertex_t* vertices = mod_model_get_vertices_const(model);
	size_t vertex_count = mod_model_get_vertex_count(model);
	const int32_t* indices = mod_model_get_indices_const(model);
	size_t index_count = mod_model_get_index_count(model);

	return mod_mesh_create_v(decl, vertices, vertex_count, indices, index_count, flags);
}

mod_mesh_t* mod_mesh_create_v(ALLEGRO_VERTEX_DECL* decl, const o_vertex_t* vertices, size_t vertex_count, const int32_t* indices, size_t index_count, int32_t flags)
{
	if (NULL == decl || NULL == vertices || NULL == indices || vertex_count == 0 || index_count == 0)
	{
		return NULL;
	}

	mod_mesh_t* mesh = (mod_mesh_t*)ogle_malloc(sizeof(mod_mesh_t));

	if (NULL == mesh)
	{
		return NULL;
	}

	mod_mesh_zero(mesh);

	if (_mod_mesh_initialize(mesh, decl, vertices, vertex_count, indices, index_count, flags) != 0)
	{
		mod_mesh_destroy(mesh);
		return NULL;
	}

	return mesh;
}

void mod_mesh_destroy(mod_mesh_t* mesh)
{
	if (NULL == mesh)
	{
		return;
	}

	mod_vertex_buffer_destroy(mesh->m_vertex_buffer);
	mod_index_buffer_destroy(mesh->m_index_buffer);
	ogle_free(mesh);
}

mod_vertex_buffer_t* mod_mesh_get_vertex_buffer(mod_mesh_t* mesh)
{
	if (NULL == mesh)
	{
		return NULL;
	}
	return mesh->m_vertex_buffer;
}

const mod_vertex_buffer_t* mod_mesh_get_vertex_buffer_const(const mod_mesh_t* mesh)
{
	if (NULL == mesh)
	{
		return NULL;
	}
	return mesh->m_vertex_buffer;
}

mod_index_buffer_t* mod_mesh_get_index_buffer(mod_mesh_t* mesh)
{
	if (NULL == mesh)
	{
		return NULL;
	}
	return mesh->m_index_buffer;
}

const mod_index_buffer_t* mod_mesh_get_index_buffer_const(const mod_mesh_t* mesh)
{
	if (NULL == mesh)
	{
		return NULL;
	}

	return mesh->m_index_buffer;
}

void mod_mesh_render(const mod_mesh_t* mesh, o_texture_t* texture)
{
	if (NULL == mesh)
	{
		return;
	}

	al_draw_indexed_buffer(
		mesh->m_vertex_buffer->m_vertex_buffer,
		(ALLEGRO_BITMAP*)texture,
		mesh->m_index_buffer->m_index_buffer,
		0, (int32_t)mesh->m_index_buffer->m_index_count,
		ALLEGRO_PRIM_TRIANGLE_LIST);
}
