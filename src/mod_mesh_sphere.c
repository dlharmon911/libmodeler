#include "libmodeler/mod_internal.h"

mod_model_t* mod_sphere_generate(float radius, int32_t levels, bool merge_vertices)
{
	mod_model_t* model = NULL;
	mod_model_t* sphere = mod_icosahedron_generate(radius, true);
	if (NULL == sphere)
	{
		return NULL;
	}

	model = sphere;
	sphere = NULL;

	if (mod_model_subdivide(model, &sphere, levels) < 0)
	{
		return NULL;
	}

	mod_model_destroy(model);

	return sphere;
}
