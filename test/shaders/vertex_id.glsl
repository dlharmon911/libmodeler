#version 330 core

in vec4 al_pos;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

void main()
{
   gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * al_pos;
}
