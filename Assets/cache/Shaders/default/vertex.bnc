#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

//out vec3 color;
out vec2 texCoord;


//TRANSFORMS OUTS
out vec3 local_position;
out vec3 world_position;

out vec3 local_normal;
out vec3 world_normal;


out vec3 ligth;


//UNIFORMS
uniform vec3 position;

uniform mat4 cam_matrix;
uniform mat4 model_matrix;




void main()
{
    gl_Position = cam_matrix * model_matrix * vec4(aPos, 1.0f);
    
    texCoord = aTex;
    
    //TRANSFORMS
    local_position = aPos;
    world_position = vec3(model_matrix * vec4(aPos, 1.0f));

    local_normal = normalize(aNormal);
    world_normal = normalize(vec3(model_matrix * vec4(aNormal, 1.0f)) - position);
    

    //LIGTH
    vec3 _ligth = normalize(vec3(1.0f, 1.0f, 0.0f));
    float _l = max(dot(world_normal, _ligth), 0.0f);

    ligth = max(vec3(_l, _l, _l), vec3(0.15f, 0.1f, 0.1f));
}