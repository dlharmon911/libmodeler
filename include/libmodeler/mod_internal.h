#ifndef _HEADER_GUARD_LIBRARY_MODELER_INTERNAL_H_
#define _HEADER_GUARD_LIBRARY_MODELER_INTERNAL_H_

// Struct definitions
#include "libmodeler.h"

struct mod_vertex_buffer_tag_t
{
	ALLEGRO_VERTEX_BUFFER* m_vertex_buffer;
	size_t m_vertex_count;
};

struct mod_index_buffer_tag_t
{
	ALLEGRO_INDEX_BUFFER* m_index_buffer;
	size_t m_index_count;
};

struct mod_mesh_tag_t
{
	mod_vertex_buffer_t* m_vertex_buffer;
	mod_index_buffer_t* m_index_buffer;
};

struct mod_model_tag_t
{
	o_vertex_t* m_vertices;
	int32_t* m_indices;
};

#endif // !_HEADER_GUARD_LIBRARY_MODELER_INTERNAL_H_
