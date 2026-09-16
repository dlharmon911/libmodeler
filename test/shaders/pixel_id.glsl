#version 330 core

in float v_meta;

void main()
{   
    gl_FragColor = vec4(v_meta / 255.0, 0.0, 0.0, 1.0);
}
