#include "libmodeler/mod_internal.h"

int32_t mod_edge_map_find(const mod_edge_map_t* edge_array, mod_edge_t edge)
{
	if (NULL == edge_array)
	{
		return -1;
	}

	size_t size = ogle_darray_size(edge_array);
	
	for (size_t i = 0; i < size; ++i)
	{
		if (mod_edge_equal(edge, edge_array[i].m_edge))
		{
			return (int32_t)i;
		}
	}
	
	return -1;
}

bool mod_edge_map_add(mod_edge_map_t** edge_array, mod_edge_t edge, int32_t index)
{
	if (NULL == edge_array)
	{
		return false;
	}

	mod_edge_map_t map_index =
	{
		edge, index
	};

	if (NULL == *edge_array)
	{
		*edge_array = ogle_darray_create(sizeof(mod_edge_map_t));
		
		if (NULL == *edge_array)
		{
			return false;
		}
	}

	return ogle_darray_push_back(edge_array, &map_index);
}
