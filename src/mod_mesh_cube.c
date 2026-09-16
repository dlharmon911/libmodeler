#include "libmodeler/mod_internal.h"

mod_model_t* mod_cube_generate(float size, bool merge_vertices)
{
	return mod_cuboid_generate(size, size, size, merge_vertices);
}
