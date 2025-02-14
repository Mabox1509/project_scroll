//[INCLUDES]
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "dogine.h"


//[VARIABLES]
double timer;

std::shared_ptr<Dogine::Texture> texture;
std::shared_ptr<Dogine::Shader> shader;
std::shared_ptr<Dogine::Shader> screen_shader;
std::shared_ptr<Dogine::Mesh> mesh;

Dogine::Mesh* screen_mesh = nullptr;

Dogine::Camera cam(glm::vec3(0, 0, 5), 45.0f, 0.1f, 100.0f);

float position[3];
float brightness;


//[FUNCTIONS]
void Start()
{
    texture = Dogine::LoadTexture("TexDither");
    shader = Dogine::LoadShader("default");
    screen_shader = Dogine::LoadShader("screen");
    mesh = Dogine::LoadMesh("primitives/sphere");

    screen_mesh = new Dogine::Mesh();
    screen_mesh->vertices.resize(4);
    screen_mesh->vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    screen_mesh->vertices[1] = glm::vec3( 1.0f, -1.0f, 0.0f);
    screen_mesh->vertices[2] = glm::vec3(-1.0f,  1.0f, 0.0f);
    screen_mesh->vertices[3] = glm::vec3( 1.0f,  1.0f, 0.0f);
    
    screen_mesh->uvs.resize(4);
    screen_mesh->uvs[0] = glm::vec2(0.0f, 0.0f);
    screen_mesh->uvs[1] = glm::vec2(1.0f, 0.0f);
    screen_mesh->uvs[2] = glm::vec2(0.0f, 1.0f);
    screen_mesh->uvs[3] = glm::vec2(1.0f, 1.0f);

    screen_mesh->triangles.resize(6);
    screen_mesh->triangles[0] = 2;
    screen_mesh->triangles[1] = 1;
    screen_mesh->triangles[2] = 0;

    screen_mesh->triangles[3] = 2;
    screen_mesh->triangles[4] = 3;
    screen_mesh->triangles[5] = 1;
    screen_mesh->Apply();


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(Dogine::GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    timer = 0;


    Dogine::application_surface = new Dogine::Surface(320, 180);
}
void Update(double _delta_time) 
{
    

}
void Draw(double _delta_time, int _w, int _h)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    timer += _delta_time;
    shader->Activate();
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position[0],position[1] + sin(timer), position[2]));
    model = glm::rotate(model, (float)glm::radians(timer*65), glm::vec3(0.0f, 1.0f, 0.0f));

    shader->SetMatrix4Fv("cam_matrix", 1, GL_FALSE, glm::value_ptr(cam.Matrix((float)_w / (float)_h)));
    shader->SetMatrix4Fv("model_matrix", 1, GL_FALSE, glm::value_ptr(model));

    texture->Bind(2);
    glUniform1i(glGetUniformLocation(shader->shader_id, "tex0"), 2);

    mesh->Bind();
    glDrawElements(GL_TRIANGLES, mesh->GetTriangles(), GL_UNSIGNED_INT, 0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void PostDraw(double _delta_time, int _w, int _h, GLuint _output)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    screen_shader->Activate();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _output);
    texture->Bind(2);

    screen_shader->Set1I("tex0", 0);
    screen_shader->Set1I("tex_dither", 2);

    screen_shader->Set2F("resolution", Dogine::application_surface->GetWidth(), Dogine::application_surface->GetHeight());
    screen_shader->Set1F("color_depth", 1.0f / 7.0f);
    //glUniform1i(glGetUniformLocation(screen_shader->shader_id, "tex0"), 0);

     
    screen_mesh->Bind();
    glDrawElements(GL_TRIANGLES, screen_mesh->GetTriangles(), GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
 
    


    //IMGUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("ImGui");



    ImGui::Text("FPS: %d", (int)(1.0f / _delta_time));
    ImGui::Text("Time: %f", timer);
    ImGui::InputFloat3("Position", position);
    
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



//[APPLICATION ENTRY]
int main() 
{
    Dogine::on_start = Start;
    Dogine::on_update = Update;
    Dogine::on_draw = Draw;
    Dogine::on_postdraw = PostDraw;

    Dogine::Init(1280, 720, "Hola mundo");
    return 0;
}

