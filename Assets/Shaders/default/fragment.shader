#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

in vec3 normal;
in vec3 crntPos;

uniform sampler2D tex0;


void main()
{
    vec4 _color = texture(tex0, texCoord);
    _color *= vec4(color, 1.0f);

    vec3 lightDirection = normalize(vec3(1.0, 1.0, 0.0));
    float _dot = dot(normal, lightDirection);

    FragColor = _color * _dot;
}