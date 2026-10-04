#include "libmodeler/mod_internal.h"

static void mod_vertex_buffer_zero(mod_vertex_buffer_t* vertex_buffer)
{
	if (NULL == vertex_buffer)
	{
		return;
	}

	vertex_buffer->m_vertex_buffer = NULL;
	vertex_buffer->m_vertex_count = 0;
}

static int32_t _mod_vertex_buffer_initialize(mod_vertex_buffer_t* vertex_buffer, ALLEGRO_VERTEX_DECL* decl, const o_vertex_t* vertices, size_t count, int32_t flags)
{
	if (NULL == vertex_buffer || NULL == decl || NULL == vertices || count <= 0)
	{
		return -1;
	}

	vertex_buffer->m_vertex_buffer = al_create_vertex_buffer(decl, vertices, (int32_t)count, flags);

	if (NULL == vertex_buffer->m_vertex_buffer)
	{
		vertex_buffer->m_vertex_count = 0;
		return -1;
	}

	vertex_buffer->m_vertex_count = count;
	
	return 0;
}


mod_vertex_buffer_t* mod_vertex_buffer_create(ALLEGRO_VERTEX_DECL* decl, const o_vertex_t* vertices, size_t count, int32_t flags)
{
	if (NULL == decl || NULL == vertices || count <= 0)
	{
		return NULL;
	}

	mod_vertex_buffer_t* vertex_buffer = (mod_vertex_buffer_t*)ogle_malloc(sizeof(mod_vertex_buffer_t));
	if (NULL == vertex_buffer)
	{
		return NULL;
	}

	mod_vertex_buffer_zero(vertex_buffer);

	if (_mod_vertex_buffer_initialize(vertex_buffer, decl, vertices, count, flags) != 0)
	{
		mod_vertex_buffer_destroy(vertex_buffer);
		return NULL;
	}

	return vertex_buffer;
}

void mod_vertex_buffer_destroy(mod_vertex_buffer_t* vertex_buffer)
{
	if (NULL == vertex_buffer)
	{
		return;
	}

	if (NULL != vertex_buffer->m_vertex_buffer)
	{
		al_destroy_vertex_buffer(vertex_buffer->m_vertex_buffer);
	}

	ogle_free(vertex_buffer);
}

o_vertex_t* mod_vertex_buffer_lock(mod_vertex_buffer_t* vertex_buffer, size_t offset, size_t length, int flags)
{
	if (NULL == vertex_buffer || NULL == vertex_buffer->m_vertex_buffer)
	{
		return NULL;
	}

	return (o_vertex_t*)al_lock_vertex_buffer(vertex_buffer->m_vertex_buffer, (int32_t)offset, (int32_t)length, flags);
}

void mod_vertex_buffer_unlock(mod_vertex_buffer_t* vertex_buffer)
{
	if (NULL == vertex_buffer || NULL == vertex_buffer->m_vertex_buffer)
	{
		return;
	}

	al_unlock_vertex_buffer(vertex_buffer->m_vertex_buffer);
}

size_t mod_vertex_buffer_get_count(const mod_vertex_buffer_t* vertex_buffer)
{
	if (NULL == vertex_buffer)
	{
		return 0;
	}

	return vertex_buffer->m_vertex_count;
}
