#include "libmodeler/mod_internal.h"

static void mod_index_buffer_zero(mod_index_buffer_t* index_buffer)
{
	if (NULL == index_buffer)
	{
		return;
	}

	index_buffer->m_index_buffer = NULL;
	index_buffer->m_index_count = 0;
}

static int32_t _mod_index_buffer_initialize(mod_index_buffer_t* index_buffer, const int32_t* indices, size_t count, int32_t flags)
{
	if (NULL == index_buffer || NULL == indices || count <= 0)
	{
		return -1;
	}

	index_buffer->m_index_buffer = al_create_index_buffer(sizeof(int32_t), indices, (int32_t)count, flags);

	if (NULL == index_buffer->m_index_buffer)
	{
		index_buffer->m_index_count = 0;
		return -1;
	}

	index_buffer->m_index_count = count;

	return 0;
}

mod_index_buffer_t* mod_index_buffer_create(const int32_t* indices, size_t count, int32_t flags)
{
	if (NULL == indices || count <= 0)
	{
		return NULL;
	}

	mod_index_buffer_t* index_buffer = (mod_index_buffer_t*)ogle_malloc(sizeof(mod_index_buffer_t));
	if (NULL == index_buffer)
	{
		return NULL;
	}

	mod_index_buffer_zero(index_buffer);

	if (_mod_index_buffer_initialize(index_buffer, indices, count, flags) != 0)
	{
		mod_index_buffer_destroy(index_buffer);
		return NULL;
	}

	return index_buffer;
}

void mod_index_buffer_destroy(mod_index_buffer_t* index_buffer)
{
	if (NULL == index_buffer)
	{
		return;
	}

	if (NULL == index_buffer->m_index_buffer)
	{
		al_destroy_index_buffer(index_buffer->m_index_buffer);
	}

	ogle_free(index_buffer);
}

int32_t* mod_index_buffer_lock(mod_index_buffer_t* index_buffer, size_t offset, size_t length, int flags)
{
	if (NULL == index_buffer || NULL == index_buffer->m_index_buffer)
	{
		return NULL;
	}

	return (int32_t*)al_lock_index_buffer(index_buffer->m_index_buffer, (int32_t)offset, (int32_t)length, flags);
}

void mod_index_buffer_unlock(mod_index_buffer_t* index_buffer)
{
	if (NULL == index_buffer || NULL == index_buffer->m_index_buffer)
	{
		return;
	}

	al_unlock_index_buffer(index_buffer->m_index_buffer);
}

size_t mod_index_buffer_get_count(const mod_index_buffer_t* index_buffer)
{
	if (NULL == index_buffer)
	{
		return 0;
	}

	return index_buffer->m_index_count;
}
