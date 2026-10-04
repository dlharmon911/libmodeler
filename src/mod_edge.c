#include "libmodeler/mod_internal.h"

mod_edge_t mod_edge_create(int32_t i1, int32_t i2)
{
	if (i1 > i2)
	{
		return (mod_edge_t) { i2, i1 };
	}

	return (mod_edge_t) { i1, i2 };
}

bool mod_edge_equal(mod_edge_t e1, mod_edge_t e2)
{
	return (e1.m_index[0] == e2.m_index[0] && e1.m_index[1] == e2.m_index[1]);
}
