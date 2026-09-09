#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <libogle.h>
#include <libmodeler.h>

// Define inline GLSL vertex shader
const char* vertex_shader_source =
"#version 120\n" // OpenGL ES 2.0 / WebGL compatibility
"attribute vec4 al_pos;\n"
"attribute vec4 al_color;\n"
"uniform mat4 al_projview_matrix;\n"
"varying vec4 v_color;\n"
"void main()\n"
"{\n"
"   v_color = al_color;\n"
"   gl_Position = al_projview_matrix * al_pos;\n"
"}\n";

// Define inline GLSL fragment shader
const char* fragment_shader_source =
"#version 120\n" // OpenGL ES 2.0 / WebGL compatibility
"varying vec4 v_color;\n"
"void main()\n"
"{\n"
"   gl_FragColor = v_color;\n"
"}\n";

mod_mesh_t* create_mesh(ALLEGRO_VERTEX_DECL* vertex_decl)
{
    mod_mesh_t* mesh = NULL;
    mod_model_t* model = NULL;
    mod_model_t* face = NULL;
    mod_quad_t quad = { 0 };
    o_vector3_t center = { 0.0f, 0.0f, 0.0f };
    o_vector3_t corner = { -0.5f, 0.5f, 0.0f };
	const o_vector3_t X_ROTATION = { 1.0f, 0.0f, 0.0f };
	const o_vector3_t Y_ROTATION = { 0.0f, 1.0f, 0.0f };
	const o_vector3_t Z_ROTATION = { 0.0f, 0.0f, 1.0f };
    float angle = (float)ALLEGRO_PI * 0.5f; 

    if (NULL == vertex_decl)
    {
		goto model_error;
    }

	face = mod_model_create_empty();
	if (NULL == face)
    {
        goto model_error;
    }

	model = mod_model_create_empty();
	if (NULL == model)
    {
        goto model_error;
    }

	mod_quad_init_p(&quad, center, corner);

	if (!mod_model_add_quad(face, &quad, false))
	{
		goto model_error;
	}

	mod_model_recolor_f(face, 1.0f, 0.0f, 0.0f, 1.0f);
	mod_model_translate_f(face, 0.0f, 0.0f, 0.5f);
	if (!mod_model_add_model(model, face, false))
	{
		goto model_error;
	}
    mod_model_translate_f(face, 0.0f, 0.0f, -0.5f);

    mod_model_recolor_f(face, 0.0f, 1.0f, 0.0f, 1.0f);
	mod_model_rotate(face, Y_ROTATION, angle);
    mod_model_translate_f(face, -0.5f, 0.0f, 0.0f);
    if (!mod_model_add_model(model, face, false))
    {
        goto model_error;
    }
    mod_model_translate_f(face, 0.5f, 0.0f, 0.0f);

    mod_model_recolor_f(face, 0.0f, 0.0f, 1.0f, 1.0f);
    mod_model_rotate(face, Y_ROTATION, angle);
    mod_model_translate_f(face, 0.0f, 0.0f, -0.5f);
    if (!mod_model_add_model(model, face, false))
    {
        goto model_error;
    }
    mod_model_translate_f(face, 0.0f, 0.0f, 0.5f);

    mod_model_recolor_f(face, 0.0f, 1.0f, 1.0f, 1.0f);
    mod_model_rotate(face, Y_ROTATION, angle);
    mod_model_translate_f(face, 0.5f, 0.0f, 0.0f);
    if (!mod_model_add_model(model, face, false))
    {
        goto model_error;
    }
    mod_model_translate_f(face, -0.5f, 0.0f, 0.0f);

    mod_model_recolor_f(face, 1.0f, 0.5f, 0.0f, 1.0f);
    mod_model_rotate(face, Z_ROTATION, angle);
    mod_model_translate_f(face, 0.0f, 0.5f, 0.0f);
    if (!mod_model_add_model(model, face, false))
    {
        goto model_error;
    }
    mod_model_translate_f(face, 0.0f, -0.5f, 0.0f);

    mod_model_recolor_f(face, 1.0f, 1.0f, 0.0f, 1.0f);
    mod_model_rotate(face, Z_ROTATION, angle* 2.0f);
    mod_model_translate_f(face, 0.0f, -0.5f, 0.0f);
    if (!mod_model_add_model(model, face, false))
    {
        goto model_error;
    }
    mod_model_translate_f(face, 0.0f, 0.5f, 0.0f);

	mod_model_recalculate_normals(model);


	mesh = mod_mesh_create(vertex_decl, model, ALLEGRO_PRIM_BUFFER_STATIC);
	
    if (!mesh)
	{
		goto model_error;
	}

model_error:

    if (model)
    {
        mod_model_destroy(model);
    }

    if (face)
    {
		mod_model_destroy(face);
	}

    return mesh;
}

int main(int argc, char** argv)
{
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_SHADER* shader = NULL;
	o_vertex_decl_t* vertex_decl = NULL;
	mod_mesh_t* mesh = NULL;
    bool key_down = false;
    int ret = 0; // Return value for main

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

    vertex_decl = ogle_vertex_decl_create();
    if (!vertex_decl)
    {
        fprintf(stderr, "failed to create vertex declaration!\n");
        ret = -1;
        goto cleanup;
    }

	mesh = create_mesh(vertex_decl);
	if (!mesh)
	{
		fprintf(stderr, "failed to create mesh!\n");
		ret = -1;
		goto cleanup;
	}

    // Create shader program from inline sources
    shader = al_create_shader(ALLEGRO_SHADER_GLSL);
    if (!shader)
    {
        fprintf(stderr, "failed to create shader object!\n");
        ret = -1;
        goto cleanup;
    }

    if (!al_attach_shader_source(shader, ALLEGRO_VERTEX_SHADER, vertex_shader_source))
    {
        fprintf(stderr, "failed to attach vertex shader source: %s\n", al_get_shader_log(shader));
        ret = -1;
        goto cleanup;
    }

    if (!al_attach_shader_source(shader, ALLEGRO_PIXEL_SHADER, fragment_shader_source))
    {
        fprintf(stderr, "failed to attach fragment shader source: %s\n", al_get_shader_log(shader));
        ret = -1;
        goto cleanup;
    }

    if (!al_build_shader(shader))
    {
        fprintf(stderr, "failed to build shader: %s\n", al_get_shader_log(shader));
        ret = -1;
        goto cleanup;
    }


    // Activate our custom shader for the backbuffer
    al_use_shader(shader);

    // Enable depth testing
    al_set_render_state(ALLEGRO_DEPTH_TEST, 1);

    bool redraw = true;
    bool do_exit = false;
    float angle = 0.0f;

    al_start_timer(timer);

    while (!do_exit)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (key_down)
            {
                angle += 0.02f; // Rotate the cube
            }
            redraw = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            do_exit = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                key_down = true;
            }
            else
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    do_exit = true;
                }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                key_down = false;
            }
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

            // Set up projection matrix
            ALLEGRO_TRANSFORM projection;
            al_identity_transform(&projection);
            float aspect_ratio = (float)al_get_display_height(display) / al_get_display_width(display);
            al_perspective_transform(&projection, -1, aspect_ratio, 1, 1, -aspect_ratio, 1000);
            al_use_projection_transform(&projection);

            // Set up modelview matrix
            ALLEGRO_TRANSFORM model;
            al_identity_transform(&model);
            al_rotate_transform_3d(&model, 1.0f, 0.0f, 0.0f, angle * 0.5f); // Rotate around X
            al_rotate_transform_3d(&model, 0.0f, 1.0f, 0.0f, angle);        // Rotate around Y
            al_rotate_transform_3d(&model, 0.0f, 0.0f, 1.0f, angle * 0.3f); // Rotate around Z
            al_translate_transform_3d(&model, 0.0f, 0.0f, -3.0f); // Translate

            ALLEGRO_TRANSFORM camera;
            al_build_camera_transform(&camera,
                0.0f, 0.0f, 0.0f, // Camera pos, move side to side
                0.0f, 0.0f, -3.0f, // Look at
                0.0f, 1.0f, 0.0f // Up
            );

            ALLEGRO_TRANSFORM modelview;
            al_identity_transform(&modelview);
            al_compose_transform(&modelview, &model);
            al_compose_transform(&modelview, &camera);
            al_use_transform(&modelview);

            // Draw the indexed primitive (cube)
			mod_mesh_render(mesh, NULL);

            // Flip display
            al_flip_display();
        }
    }

cleanup:
    // Cleanup resources in reverse order of allocation
    if (shader)
        al_destroy_shader(shader);
    if (mesh)
        mod_mesh_destroy(mesh);
    if (vertex_decl)
        ogle_vertex_decl_destroy(vertex_decl);
    if (timer)
        al_destroy_timer(timer);
    if (event_queue)
        al_destroy_event_queue(event_queue);
    if (display)
        al_destroy_display(display);

    return ret;
}
