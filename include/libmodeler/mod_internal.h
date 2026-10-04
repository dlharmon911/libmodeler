#ifndef _HEADER_GUARD_LIBRARY_MODELER_INTERNAL_H_
#define _HEADER_GUARD_LIBRARY_MODELER_INTERNAL_H_

// Struct definitions
#include "libmodeler.h"

typedef struct mod_edge_map_tag_t mod_edge_map_t;

struct mod_edge_map_tag_t
{
	mod_edge_t m_edge;
	int32_t m_index;
};

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

int32_t mod_edge_map_find(const mod_edge_map_t* edge_array, mod_edge_t edge);
bool mod_edge_map_add(mod_edge_map_t** edge_array, mod_edge_t edge, int32_t index);

#endif // !_HEADER_GUARD_LIBRARY_MODELER_INTERNAL_H_
