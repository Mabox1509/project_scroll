#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;


void main()
{
    vec4 _color = texture(tex0, texCoord);
    _color *= vec4(color, 1.0f);


    FragColor = _color;
}