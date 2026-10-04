#ifndef _HEADER_GUARD_LIBRARY_MODELER_H_
#define _HEADER_GUARD_LIBRARY_MODELER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <libogle.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MOD_QUAD_VERTEX_COUNT 4
#define MOD_TRIANGLE_VERTEX_COUNT 3
#define MOD_EDGE_INDEX_COUNT 2

static const o_vector3_t MOD_X_ROTATION = { 1.0f, 0.0f, 0.0f };
static const o_vector3_t MOD_Y_ROTATION = { 0.0f, 1.0f, 0.0f };
static const o_vector3_t MOD_Z_ROTATION = { 0.0f, 0.0f, 1.0f };

// Forward declarations

typedef struct mod_vertex_buffer_tag_t mod_vertex_buffer_t;
typedef struct ALLEGRO_VERTEX_ELEMENT mod_vertex_element_t;
typedef struct mod_index_buffer_tag_t mod_index_buffer_t;
typedef struct mod_mesh_tag_t mod_mesh_t;
typedef struct mod_model_tag_t mod_model_t;
typedef struct mod_triangle_tag_t mod_triangle_t;
typedef struct mod_quad_tag_t mod_quad_t;
typedef struct mod_edge_tag_t mod_edge_t;
typedef struct mod_transform_tag_t mod_transform_t;

struct mod_triangle_tag_t
{
	o_vertex_t m_vertex[MOD_TRIANGLE_VERTEX_COUNT];
};

struct mod_quad_tag_t
{
	o_vertex_t m_vertex[MOD_QUAD_VERTEX_COUNT];
};

struct mod_edge_tag_t
{
	int32_t m_index[MOD_EDGE_INDEX_COUNT];
};

struct mod_transform_tag_t
{
	o_vector3_t m_translation;
	o_vector3_t m_rotation;
	o_vector3_t m_scale;
	o_color_t m_color;
};

// Function declarations

void mod_triangle_init(mod_triangle_t* triangle, const o_vertex_t* v1, const o_vertex_t* v2, const o_vertex_t* v3);
void mod_triangle_init_v(mod_triangle_t* triangle, o_vector3_t v1, o_vector3_t v2, o_vector3_t v3);
void mod_triangle_recolor(mod_triangle_t* triangle, o_color_t color);

void mod_quad_init(mod_quad_t* quad, const o_vertex_t* v1, const o_vertex_t* v2, const o_vertex_t* v3, const o_vertex_t* v4);
void mod_quad_init_v(mod_quad_t* quad, o_vector3_t v1, o_vector3_t v2, o_vector3_t v3, o_vector3_t v4);
void mod_quad_init_p(mod_quad_t* quad, float width, float height);
void mod_quad_recolor(mod_quad_t* quad, o_color_t color);

mod_vertex_buffer_t* mod_vertex_buffer_create(ALLEGRO_VERTEX_DECL* decl, const o_vertex_t* vertices, size_t count, int32_t flags);
void mod_vertex_buffer_destroy(mod_vertex_buffer_t* vertex_buffer);
o_vertex_t* mod_vertex_buffer_lock(mod_vertex_buffer_t* vertex_buffer, size_t offset, size_t length, int flags);
void mod_vertex_buffer_unlock(mod_vertex_buffer_t* vertex_buffer);
size_t mod_vertex_buffer_get_count(const mod_vertex_buffer_t* vertex_buffer);

mod_index_buffer_t* mod_index_buffer_create(const int32_t* indices, size_t count, int32_t flags);
void mod_index_buffer_destroy(mod_index_buffer_t* index_buffer);
int32_t* mod_index_buffer_lock(mod_index_buffer_t* index_buffer, size_t offset, size_t length, int flags);
void mod_index_buffer_unlock(mod_index_buffer_t* index_buffer);
size_t mod_index_buffer_get_count(const mod_index_buffer_t* index_buffer);

mod_edge_t mod_edge_create(int32_t i1, int32_t i2);
bool mod_edge_equal(mod_edge_t e1, mod_edge_t e2);

mod_model_t* mod_model_create_empty();
mod_model_t* mod_model_create(const o_vertex_t* vertices, size_t vertex_count, const int32_t* indices, size_t index_count);
mod_model_t* mod_model_clone(const mod_model_t* model);
mod_model_t* mod_model_clone_with_meta_data(const mod_model_t* model, const mod_transform_t* meta_data);
void mod_model_destroy(mod_model_t* model);
void mod_model_apply_meta_data(mod_model_t* model, const mod_transform_t* meta_data);
o_vertex_t* mod_model_get_vertex(mod_model_t* model, size_t index);
const o_vertex_t* mod_model_get_vertex_const(const mod_model_t* model, size_t index);
o_vertex_t* mod_model_get_vertices(mod_model_t* model);
const o_vertex_t* mod_model_get_vertices_const(const mod_model_t* model);
size_t mod_model_get_vertex_count(const mod_model_t* model);
int32_t* mod_model_get_indices(mod_model_t* model);
const int32_t* mod_model_get_indices_const(const mod_model_t* model);
int32_t mod_model_get_index(mod_model_t* model, size_t index);
size_t mod_model_get_index_count(const mod_model_t* model);
bool mod_model_push_vertex(mod_model_t* model, const o_vertex_t* vertex);
bool mod_model_push_vertices(mod_model_t* model, const o_vertex_t* vertices, size_t count);
bool mod_model_pop_vertex(mod_model_t* model);
bool mod_model_pop_vertices(mod_model_t* model, size_t count);
bool mod_model_push_index(mod_model_t* model, int32_t index);
bool mod_model_push_indices(mod_model_t* model, const int32_t* indices, size_t count);
bool mod_model_pop_index(mod_model_t* model);
bool mod_model_pop_indices(mod_model_t* model, size_t count);
bool mod_model_add_triangle(mod_model_t* model, const mod_triangle_t* triangle, bool merge_vertices);
bool mod_model_add_triangle_v(mod_model_t* model, const o_vertex_t* v1, const o_vertex_t* v2, const o_vertex_t* v3, bool merge_vertices);
bool mod_model_add_quad(mod_model_t* model, const mod_quad_t* quad, bool merge_vertices);
bool mod_model_add_quad_v(mod_model_t* model, const o_vertex_t* v1, const o_vertex_t* v2, const o_vertex_t* v3, const o_vertex_t* v4, bool merge_vertices);
bool mod_model_add_model(mod_model_t* model, const mod_model_t* other_model, bool merge_vertices);
bool mod_model_add_model_with_meta_data(mod_model_t* model, const mod_model_t* other_model, const mod_transform_t* meta_data, bool merge_vertices);
bool mod_model_add_shape(mod_model_t* model, const o_vertex_t* vertices, size_t vertex_count, const int32_t* indices, size_t index_count, bool merge_vertices);
void mod_model_rotate(mod_model_t* model, o_vector3_t rotation, float angle);
void mod_model_rotate_f(mod_model_t* model, float x, float y, float z, float angle);
void mod_model_scale(mod_model_t* model, o_vector3_t scale);
void mod_model_scale_f(mod_model_t* model, float x, float y, float z);
void mod_model_scale_ff(mod_model_t* model, float scale);
void mod_model_translate(mod_model_t* model, o_vector3_t translation);
void mod_model_translate_f(mod_model_t* model, float x, float y, float z);
void mod_model_recolor(mod_model_t* model, o_color_t color);
void mod_model_recolor_f(mod_model_t* model, float r, float g, float b, float a);
o_vector3_t mod_model_get_center(const mod_model_t* model);
void mod_model_recalculate_normals(mod_model_t* model);
void mod_model_render(ALLEGRO_VERTEX_DECL* decl, const mod_model_t* model, o_texture_t* texture);
void mod_model_set_id(mod_model_t* model, int32_t id);
int32_t mod_model_subdivide(const mod_model_t* model, mod_model_t** out_model, int32_t levels);
void mod_model_normalize_vertices(mod_model_t* model, float radius);

mod_mesh_t* mod_mesh_create(ALLEGRO_VERTEX_DECL* decl, const mod_model_t* model, int32_t flags);
mod_mesh_t* mod_mesh_create_v(ALLEGRO_VERTEX_DECL* decl, const o_vertex_t* vertices, size_t vertex_count, const int32_t* indices, size_t index_count, int32_t flags);
void mod_mesh_destroy(mod_mesh_t* mesh);
mod_vertex_buffer_t* mod_mesh_get_vertex_buffer(mod_mesh_t* mesh);
const mod_vertex_buffer_t* mod_mesh_get_vertex_buffer_const(const mod_mesh_t* mesh);
mod_index_buffer_t* mod_mesh_get_index_buffer(mod_mesh_t* mesh);
const mod_index_buffer_t* mod_mesh_get_index_buffer_const(const mod_mesh_t* mesh);
void mod_mesh_render(const mod_mesh_t* mesh, o_texture_t* texture);

mod_model_t* mod_cube_generate(float size, bool merge_vertices);
mod_model_t* mod_cuboid_generate(float width, float height, float depth, bool merge_vertices);
mod_model_t* mod_pyramid_generate(float radius, float height, size_t sides, bool merge_vertices);
mod_model_t* mod_pyramid_generate_baseless(float radius, float height, size_t sides, bool merge_vertices);
mod_model_t* mod_tube_generate(float radius, float width, size_t sides, bool merge_vertices);
mod_model_t* mod_icosahedron_generate(float radius, bool merge_vertices);
mod_model_t* mod_sphere_generate(float radius, int32_t levels, bool merge_vertices);
mod_model_t* mod_tile_generate(float width, float height, float depth, float radius, bool merge_vertices);

#ifdef __cplusplus
   }
#endif

#endif // !_HEADER_GUARD_LIBRARY_MODELER_H_
