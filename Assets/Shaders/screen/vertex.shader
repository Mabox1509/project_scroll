#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

//out vec3 color;
out vec2 texCoord;


//UNIFORMS



void main()
{
    gl_Position = vec4(aPos, 1.0f);
    
    texCoord = aTex;
    

}