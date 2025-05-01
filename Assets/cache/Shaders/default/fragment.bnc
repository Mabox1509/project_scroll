#version 330 core
out vec4 FragColor;

/*in vec3 color;*/
in vec2 texCoord;


//TRANSFORMS IN
in vec3 local_position;
in vec3 world_position;

in vec3 local_normal;
in vec3 world_normal;

in vec3 ligth;

uniform sampler2D tex0;


void main()
{
    vec3 _norm = local_normal;
    _norm += 1.0;
    _norm /= 2.0;

    FragColor = vec4(_norm, 1.0f);
}