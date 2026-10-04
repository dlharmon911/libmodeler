#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <libogle.h>
#include <libmodeler.h>

static const o_light_t g_light =
{
    .m_position = { 0.0f, 0.0f, 10.0f },
    .m_ambient = { 0.6f, 0.6f, 0.6f },
    .m_diffuse = { 1.0f, 1.0f, 1.0f },
    .m_specular = { 0.4f, 0.4f, 0.4f }
};

static const o_camera_t g_camera =
{
    .m_position = { 0.0f, 0.0f, 1.5f },
    .m_lookat = { 0.0f, 0.0f, 0.0f },
    .m_up = { 0.0f, 1.0f, 0.0f }
};

static ALLEGRO_SHADER* load_shader_from_file(const char* vertex_shader_path, const char* fragment_shader_path)
{
	ALLEGRO_SHADER* shader = al_create_shader(ALLEGRO_SHADER_GLSL);
	if (!shader)
	{
		fprintf(stderr, "failed to create shader object!\n");
		return NULL;
	}
	if (!al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, vertex_shader_path))
	{
		fprintf(stderr, "failed to attach vertex shader source: %s\n", al_get_shader_log(shader));
		al_destroy_shader(shader);
		return NULL;
	}
	if (!al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, fragment_shader_path))
	{
		fprintf(stderr, "failed to attach fragment shader source: %s\n", al_get_shader_log(shader));
		al_destroy_shader(shader);
		return NULL;
	}
	if (!al_build_shader(shader))
	{
		fprintf(stderr, "failed to build shader: %s\n", al_get_shader_log(shader));
		al_destroy_shader(shader);
		return NULL;
	}
	return shader;
}

#define TILE_WIDTH 32
#define TILE_HEIGHT 40

static void set_tile(int32_t index, int32_t bitmap_width, int32_t bitmap_height, o_vertex_t* vertices)
{
    o_vector2_t tl = { 0.0f, 0.0f };
	o_vector2_t br = { 0.0f, 0.0f };

	if (index < 0 || index > 42)
	{
		index = 0;
	}

    int32_t tiles_per_row = bitmap_width / TILE_WIDTH;
    int32_t row = index / tiles_per_row;
    int32_t col = index % tiles_per_row;

    tl.m_x = (float)(col * TILE_WIDTH) / (float)bitmap_width;
    tl.m_y = 1.0f - (float)((row + 1) * TILE_HEIGHT) / (float)bitmap_height;
    br.m_x = (float)((col + 1) * TILE_WIDTH) / (float)bitmap_width;
    br.m_y = 1.0f - (float)(row * TILE_HEIGHT) / (float)bitmap_height;

	vertices[0].m_uv = (o_vector2_t){ tl.m_x, tl.m_y }; // Top-left
	vertices[1].m_uv = (o_vector2_t){ br.m_x, tl.m_y }; // Top-right
	vertices[2].m_uv = (o_vector2_t){ br.m_x, br.m_y }; // Bottom-right
	vertices[3].m_uv = (o_vector2_t){ tl.m_x, br.m_y }; // Bottom-left
}

void update_id(mod_model_t* model, int32_t id)
{
	if (NULL == model || id < 0 || id > 143)
	{
		return;
	}

	size_t vertex_count = mod_model_get_vertex_count(model);
	if (0 == vertex_count)
	{
		return;
	}

	for (size_t i = 0; i < vertex_count; ++i)
	{
		o_vertex_t* vertex = mod_model_get_vertex(model, i);
		if (vertex)
		{
			int32_t vid = (int32_t)vertex->m_meta;

            if (id == vid)
            {
                vertex->m_meta = -vertex->m_meta;
            }
		}
	}
}

int main(int argc, char** argv)
{
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_SHADER* shader = NULL;
    ALLEGRO_SHADER* shader_id = NULL;
	o_vertex_decl_t* vertex_decl = NULL;
	ALLEGRO_BITMAP* texture = NULL;
	mod_model_t* mod_model = NULL;
    bool key_down[3] = { false, false, false };
    int ret = 0; // Return value for main
    bool redraw = true;
    bool do_exit = false;
    float angle[3] = { 0.0f, 0.0f, 0.0f };
    bool reverse = false;
    mod_model_t* big_model = NULL;
	int32_t mouse_id = -1;

    // Initialize Allegro
    if (!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        ret = -1;
        goto cleanup;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "failed to initialize keyboard!\n");
        ret = -1;
        goto cleanup;
    }

	if (!al_install_mouse())
	{
		fprintf(stderr, "failed to initialize mouse!\n");
		ret = -1;
		goto cleanup;
	}

    // Initialize images add-on
    if (!al_init_image_addon())
    {
        fprintf(stderr, "failed to initialize images addon!\n");
        ret = -1;
        goto cleanup;
    }

    // Initialize primitives add-on
    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "failed to initialize primitives addon!\n");
        ret = -1;
        goto cleanup;
    }

    // Set OpenGL version for Allegro
    al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST); // Enable depth buffer
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL);

    // Create display
    display = al_create_display(800, 600);
    if (!display)
    {
        fprintf(stderr, "failed to create display!\n");
        ret = -1;
        goto cleanup;
    }

    // Create event queue
    event_queue = al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr, "failed to create event_queue!\n");
        ret = -1;
        goto cleanup;
    }

    // Create timer
    timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer)
    {
        fprintf(stderr, "failed to create timer!\n");
        ret = -1;
        goto cleanup;
    }

    // Register event sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

	texture = al_load_bitmap("textures/tiles.png");
	if (!texture)
	{
		fprintf(stderr, "failed to load texture!\n");
		ret = -1;
		goto cleanup;
	}
    al_convert_mask_to_alpha(texture, al_map_rgb(255, 0, 255));

    vertex_decl = ogle_vertex_decl_create();
    if (!vertex_decl)
    {
        fprintf(stderr, "failed to create vertex declaration!\n");
        ret = -1;
        goto cleanup;
    }

	float radius = 0.5f;
	float height = radius * sqrtf(2.0f);
    mod_model = mod_tile_generate(0.9f, 1.2f, 0.25f, 0.025f, true);
	//mod_model = mod_pyramid_generate(radius, height, 3, false);
	if (!mod_model)
	{
		fprintf(stderr, "failed to create model!\n");
		ret = -1;
		goto cleanup;
	}

    // Create shader program from inline sources
    shader = load_shader_from_file("shaders/vertex_material.glsl", "shaders/pixel_material.glsl");
    if (!shader)
    {
        fprintf(stderr, "failed to load shader!\n");
        ret = -1;
        goto cleanup;
    }

    // Create shader program from inline sources
	shader_id = load_shader_from_file("shaders/vertex_material.glsl", "shaders/pixel_id.glsl");
	if (!shader_id)
	{
		fprintf(stderr, "failed to load shader!\n");
		ret = -1;
		goto cleanup;
	}

    // Enable depth testing
    al_set_render_state(ALLEGRO_DEPTH_TEST, 1);

    al_start_timer(timer);

    big_model = mod_model_create_empty();
	if (!big_model)
	{
		fprintf(stderr, "failed to create big model!\n");
		ret = -1;
		goto cleanup;
	}

	mod_transform_t model_transform = 
    {
		.m_translation = { 0.0f, 0.0f, 0.0f },
		.m_scale = { 1.0f, 1.0f, 1.0f },
		.m_rotation = { 0.0f, 0.0f, 0.0f },
		.m_color = { 1.0f, 1.0f, 1.0f, 1.0f }
    };

    o_vertex_t* vertices = mod_model_get_vertices(mod_model);
	for (int32_t i = 0; i < 144; ++i)
	{
        int32_t tile_index = 1 + (rand() % 42);
        set_tile(tile_index, al_get_bitmap_width(texture), al_get_bitmap_height(texture), vertices);

        model_transform.m_translation.m_x = (float)(i % 12) - 6.0f;
        model_transform.m_translation.m_y = ((float)(i / 12) - 6.0f) * 1.2f;
		model_transform.m_translation.m_z = 0.0f; // Z position
		mod_model_set_id(mod_model, i);

		if (!mod_model_add_model_with_meta_data(big_model, mod_model, &model_transform, false))
		{
			fprintf(stderr, "failed to add model to big model!\n");
			ret = -1;
			goto cleanup;
		}
	}

    while (!do_exit)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
			float amount = 0.01f; // Rotation speed

			if (reverse)
			{
                amount = -0.01f;
			}

			for (int i = 0; i < 3; ++i)
			{
				if (key_down[i])
				{
					angle[i] += amount; // Rotate the cube
				}
			}

            redraw = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            do_exit = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_LSHIFT)
            {
				reverse = true;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_Z)
            {
                key_down[2] = true;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_X)
            {
                key_down[0] = true;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_Y)
            {
                key_down[1] = true;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    do_exit = true;
                }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_LSHIFT)
            {
                reverse = false;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_Z)
            {
                key_down[2] = false;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_X)
            {
                key_down[0] = false;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_Y)
            {
                key_down[1] = false;
            }
        }
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(0, 0, 0)); // Clear background to black
            al_clear_depth_buffer(1.); // Clear depth to infinitely far away

            o_vector2_t resolution =
            {
                (float)al_get_display_width(display),
                (float)al_get_display_height(display)
            };
            float aspect_ratio = resolution.m_y / resolution.m_x;
            o_vector2_t top_left = { -1.0f, aspect_ratio };
            o_vector2_t bottom_right = { 1.0f, -aspect_ratio };

            o_transform_t projection;
            o_transform_t view;
            o_transform_t model;

            // build projection matrix
            ogle_transform_identity(&projection);
            ogle_transform_perspective(&projection, top_left, bottom_right, 1.0f, 100.0f);

            // build view matrix
            ogle_transform_camera_build(&view, &g_camera);

            // build model matrix
            ogle_transform_identity(&model);
            ogle_transform_compose(&model, &view);

            al_use_shader(shader_id);

            const o_material_t* material = ogle_material_get(OGLE_MATERIAL_IVORY);
            ogle_material_set_shader("u_material", material);

            ogle_light_set_shader("u_light", &g_light);
            ogle_camera_set_shader("u_camera", &g_camera);
            ogle_transform_set_shader("u_projection_matrix", &projection);
            ogle_transform_set_shader("u_view_matrix", &view);

            o_vector3_t scale = { 1.0f, 1.0f, 1.0f };
            o_vector3_t position = { 0.0f, 0.0f, -8.0f };

            o_transform_t transform;
            ogle_transform_identity(&transform);
            ogle_transform_scale_3d(&transform, scale);
            ogle_transform_rotate_3d(&transform, (o_vector3_t) { 1.0f, 0.0f, 0.0f }, angle[0]);
            ogle_transform_rotate_3d(&transform, (o_vector3_t) { 0.0f, 1.0f, 0.0f }, angle[1]);
            ogle_transform_rotate_3d(&transform, (o_vector3_t) { 0.0f, 0.0f, 1.0f }, angle[2]);
            ogle_transform_translate_3d(&transform, position);
            ogle_transform_compose(&transform, &model);
            ogle_transform_set_shader("u_model_matrix", &transform);

            // Draw the indexed primitive (cube)
            mod_model_render(vertex_decl, big_model, texture);

            al_use_shader(NULL);

			ALLEGRO_BITMAP* target = al_get_target_bitmap();
            ALLEGRO_COLOR pixel = al_get_pixel(target, (int32_t)ev.mouse.x, (int32_t)ev.mouse.y);
            mouse_id = (int32_t)(pixel.r * 255.0f);

            update_id(big_model, mouse_id);
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            al_acknowledge_resize(display);
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(0, 0, 128)); // Clear background to blue
            al_clear_depth_buffer(1.); // Clear depth to infinitely far away

            o_vector2_t resolution =
            {
                (float)al_get_display_width(display),
                (float)al_get_display_height(display)
            };
            float aspect_ratio = resolution.m_y / resolution.m_x;
            o_vector2_t top_left = { -1.0f, aspect_ratio };
            o_vector2_t bottom_right = { 1.0f, -aspect_ratio };

			o_transform_t projection;
			o_transform_t view;
			o_transform_t model;

            // build projection matrix
            ogle_transform_identity(&projection);
            ogle_transform_perspective(&projection, top_left, bottom_right, 1.0f, 100.0f);

            // build view matrix
            ogle_transform_camera_build(&view, &g_camera);

            // build model matrix
            ogle_transform_identity(&model);
            ogle_transform_compose(&model, &view);

            al_use_shader(shader);

            const o_material_t* material = ogle_material_get(OGLE_MATERIAL_IVORY);
            ogle_material_set_shader("u_material", material);

            ogle_light_set_shader("u_light", &g_light);
            ogle_camera_set_shader("u_camera", &g_camera);
            ogle_transform_set_shader("u_projection_matrix", &projection);
            ogle_transform_set_shader("u_view_matrix", &view);


            o_vector3_t scale = { 1.0f, 1.0f, 1.0f };
			o_vector3_t position = { 0.0f, 0.0f, -8.0f };

            o_transform_t transform;
            ogle_transform_identity(&transform);
            ogle_transform_scale_3d(&transform, scale);
            ogle_transform_rotate_3d(&transform, (o_vector3_t) { 1.0f, 0.0f, 0.0f }, angle[0]);
            ogle_transform_rotate_3d(&transform, (o_vector3_t) { 0.0f, 1.0f, 0.0f }, angle[1]);
            ogle_transform_rotate_3d(&transform, (o_vector3_t) { 0.0f, 0.0f, 1.0f }, angle[2]);
            ogle_transform_translate_3d(&transform, position);
            ogle_transform_compose(&transform, &model);
            ogle_transform_set_shader("u_model_matrix", &transform);

            // Draw the indexed primitive (cube)
            mod_model_render(vertex_decl, big_model, texture);

            al_use_shader(NULL);

            // Flip display
            al_flip_display();
        }
    }

cleanup:

	if (big_model)
		mod_model_destroy(big_model);

    // Cleanup resources in reverse order of allocation
    if (shader_id)
        al_destroy_shader(shader_id);
    if (shader)
        al_destroy_shader(shader);
    if (mod_model)
        mod_model_destroy(mod_model);
    if (vertex_decl)
        ogle_vertex_decl_destroy(vertex_decl);
	if (texture)
		al_destroy_bitmap(texture);
    if (timer)
        al_destroy_timer(timer);
    if (event_queue)
        al_destroy_event_queue(event_queue);
    if (display)
        al_destroy_display(display);

    return ret;
}
