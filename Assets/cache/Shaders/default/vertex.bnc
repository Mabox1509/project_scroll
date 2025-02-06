#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 texCoord;

out vec3 normal;
out vec3 crntPos;


uniform mat4 cam_matrix;
uniform mat4 model_matrix;


void main()
{
    crntPos = vec3(model_matrix * vec4(aPos, 1.0f));

    gl_Position = cam_matrix * vec4(aPos, 1.0f);
    
    color = aColor;
    texCoord = aTex;

    normal = normalize(vec3(model_matrix * vec4(aNormal, 1.0f)));
}