//[INCLUDES]
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "log.h"
#include "resources.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"



//[VARIABLES]
const unsigned int width = 1280;
const unsigned int height = 720;
float aspect;

//R y G c B p R
//     COOORD               //COLORS            //UVS
GLfloat vertices[] =
{
    // Base de la pir�mide
    -0.5f,  0.0f,  0.5f,    0.85f, 0.75f, 0.0f,   0.0f,  0.0f,
    -0.5f,  0.0f, -0.5f,    1.0f, 0.75f, 0.0f,    2.5f,  0.0f,
     0.5f,  0.0f, -0.5f,    0.85f, 0.75f, 0.0f,   0.0f,  0.0f,
     0.5f,  0.0f,  0.5f,    0.5f, 0.65f, 0.0f,    2.5f,  0.0f,

     0.0f,  0.8f,  0.0f,    1.0f, 1.0f, 0.5f,    1.25f,  2.5f
};

GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};


GLFWwindow* window = NULL;
std::shared_ptr<Resources::Shader> shader = nullptr;
std::shared_ptr<Resources::Texture> texture = nullptr;


//[FUNCTIONS]
void InitializeOpenGL()
{
    Log::Message("Loading GLFW...");
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    Log::Message("Creating window...");
    window = glfwCreateWindow(width, height, "Hola que tal", NULL, NULL);
    if (window == NULL)
    {
        Log::Error("Failed to create GLFW window");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();

}



//[APPLICATION ENTRY]
int main()
{
    aspect = (float)(width) / (float)(height);

    InitializeOpenGL();
    Resources::Init();


    glViewport(0, 0, width, height);
    shader = Resources::LoadShader("default");
    texture = Resources::LoadTexture("test");
   

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    


    GLuint tex0Uni = glGetUniformLocation(shader->shader_id, "tex0");
    shader->Activate();
    glUniform1i(tex0Uni, 0);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();


    double delta_time = 0;

    double timer = 0;
    bool isOrtho = false;
    float orthoSize = 0.5;

    bool press = false;

    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader->Activate();

        double crntTime = glfwGetTime();
        delta_time = crntTime - prevTime;
        prevTime = crntTime;


        rotation += (24 * delta_time);
        timer += delta_time;

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            if (!press) 
            {
                isOrtho = !isOrtho;
                press = true;

                if (isOrtho)
                    Log::Message("Ortografica");
                else
                    Log::Message("Perspectiva");
            }
            
        }
        else
        {
            press = false;
        }


        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(0, 0.15 + (sin(timer) * 0.1f), 0));
        model = glm::scale(model, glm::vec3(1, 2, 1));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
       
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        if (isOrtho)
        {
            proj = glm::ortho(-aspect * orthoSize, aspect * orthoSize, -orthoSize, orthoSize, 0.1f, 100.0f);
        }
        else
        {
            proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        }


        int modelLoc = glGetUniformLocation(shader->shader_id, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shader->shader_id, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(shader->shader_id, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));


        texture->Bind();
        
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);


        //TAKE CARE OF GLFW EVENTS
        glfwPollEvents();
    }


    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    //glDeleteTextures(1, &texture);

    shader.reset();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

