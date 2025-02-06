//[INCLUDES]
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "log.h"
#include "resources.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "camera.h"



//[VARIABLES]
const unsigned int width = 1280;
const unsigned int height = 720;
float aspect;

//R y G c B p R
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2, // Bottom side
    0, 2, 3, // Bottom side
    4, 6, 5, // Left side
    7, 9, 8, // Non-facing side
    10, 12, 11, // Right side
    13, 15, 14 // Facing side
};


GLFWwindow* window = NULL;
std::shared_ptr<Resources::Shader> shader = nullptr;
std::shared_ptr<Resources::Texture> texture = nullptr;

Camera camera(glm::vec3(0.0f, 5.0f, -3.0f), 45.0f, 0.1f, 100.0f);
double delta_time = 0;

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

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    
    camera.Rotate(glm::vec3(-35.0f, 0.0f, 0.0f));
    camera.Rotate(glm::vec3(0.0f, 180.0f, 0.0f));

    GLuint tex0Uni = glGetUniformLocation(shader->shader_id, "tex0");
    shader->Activate();
    glUniform1i(tex0Uni, 0);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();
    float scale[3] = { 1, 1, 1 };

    

    double timer = 0;
    float clear_color[4] = { 0.07f, 0.13f, 0.17f, 1.0f };
    float spd = 1;


    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    while (!glfwWindowShouldClose(window))
    {
        //UPDATE
        float camera_speed = spd * delta_time; // deltaTime es el tiempo entre frames sucesivos

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.position.x -= camera_speed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.position.x += camera_speed;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.position.z += camera_speed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.position.z -= camera_speed;



        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.position.y += camera_speed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.position.y -= camera_speed;




        //DRAW
        glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        
        shader->Activate();

        double crntTime = glfwGetTime();
        delta_time = crntTime - prevTime;
        prevTime = crntTime;
        timer += delta_time;

        

        
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(0, sin(timer) * 0.1f, 0));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale[0], scale[1], scale[2]));
       


        int projLoc = glGetUniformLocation(shader->shader_id, "cam_matrix");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.Matrix(model, aspect)));

        int modLoc = glGetUniformLocation(shader->shader_id, "model_matrix");
        glUniformMatrix4fv(modLoc, 1, GL_FALSE, glm::value_ptr(model));

        texture->Bind();
        
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        

        ImGui::Begin("ImGui");
        
        
        ImGui::SliderFloat("Rotacion", &rotation, 0, 360);
        ImGui::InputFloat3("Scale", scale);
        ImGui::Checkbox("Orthographic", &camera.is_orthographic);
        ImGui::ColorEdit4("Clear color", clear_color);

        ImGui::End();

        ImGui::Begin("Camera Information");  // Crear una ventana de ImGui
        ImGui::Text("(%.1f, %.1f, %.1f)", camera.position.x, camera.position.y, camera.position.z);
        ImGui::SliderFloat("Speed", &spd, 1, 10);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        //TAKE CARE OF GLFW EVENTS
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    //glDeleteTextures(1, &texture);

    shader.reset();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

