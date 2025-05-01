//[INCLUDES]
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/implot.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "dogine.h"
#include "inc/renderer.h"
#include "inc/roommanager.h"



//[VARIABLES]
glm::mat4 ortho_matrix = glm::ortho(0, 320, 0, 180);



//[FUNCTIONS]
void Start()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(Dogine::GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Dogine::application_surface = new Dogine::Surface(320, 180);
    Renderer::Init();
}
void Update(double _delta_time) 
{
    RoomManager::Update(_delta_time);
}
void Draw(double _delta_time, int _w, int _h)
{
    float _aspect = (float)_w / (float)_h;
    auto _cam = Renderer::main_camera.Matrix(_aspect);

    Renderer::DrawLayer(Renderer::LayerID::BG, ortho_matrix);
    Renderer::DrawLayer(Renderer::LayerID::OPAQUE, _cam);
    Renderer::DrawLayer(Renderer::LayerID::TRANS, _cam);
    Renderer::DrawLayer(Renderer::LayerID::HUD, ortho_matrix);
}
void PostDraw(double _delta_time, int _w, int _h, GLuint _output)
{
    Renderer::DrawMain(_output);



    //IMGUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    ImGui::Begin("Profiler");
    ImGui::Text("FPS: %d (%.2f ms)", (int)(1.0 / _delta_time), _delta_time * 1000.0f);
    ImGui::End();

    Renderer::DrawLayer(Renderer::LayerID::WIN, ortho_matrix);


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    RoomManager::ClearGhosts();
}



//[APPLICATION ENTRY]
int main()
{
    Dogine::on_start = Start;
    Dogine::on_update = Update;
    Dogine::on_draw = Draw;
    Dogine::on_postdraw = PostDraw;

    Dogine::Init(1280, 720, "project_scroll");
    return 0;
}


