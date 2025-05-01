//[INCLUDES]
#include "../inc/renderer.h"
#define GLM_ENABLE_EXPERIMENTAL

#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>




//[VARIABLES]
std::shared_ptr<Dogine::Texture> texture_dither;
std::shared_ptr<Dogine::Shader> screen_shader;
Dogine::Mesh* screen_mesh = nullptr;

glm::mat4 active_view;

//[NAMESPACES]
namespace Renderer
{
#pragma region Layer
	Layer::Layer(bool _blending, bool _ztest, bool _zwriting, bool _copy, bool _fix_depth, bool _face_culling)
		: blending(_blending), ztest(_ztest), zwriting(_zwriting), copy_buffer(_copy), fix_depth(_fix_depth), face_culling(_face_culling)
	{

	}
	Layer::Layer(){}

	// Add object to layer queue
	void Layer::Enqueue(GameObject* _obj, int _depth)
	{
		if(queue.find(_depth) != queue.end())
		{
			if(fix_depth)
			{
				Enqueue(_obj, _depth + 1);
			}
			else 
			{
				if(rand() % 2 == 0)
				{
					queue[_depth] = _obj;
				}
			}
		}

		queue[_depth] = _obj;
	}

	// Draw layer content
	void Layer::Draw(glm::mat4 _view, uint8_t _layer)
	{
		if (queue.size() <= 0)
			return;


		//SET RENDER PARAM
		if (blending)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		if (ztest)
		{
			glEnable(GL_DEPTH_TEST);  // Enable Z-test
			if (zwriting)
				glDepthMask(GL_TRUE);  // Allow writing to Z-buffer
			else
				glDepthMask(GL_FALSE); // Disallow writing to Z-buffer
		}
		else
		{
			glDisable(GL_DEPTH_TEST); // Disable Z-test
		}

		if (face_culling)
			glEnable(GL_CULL_FACE);  // Enable culling of back faces
		else
			glDisable(GL_CULL_FACE); // Disable culling




		//DRAW OBJECTS
		for (auto& pair : queue)
		{
			GameObject* obj = pair.second;
			if (obj->IsDestroyed())
				continue;

			obj->Draw(_view, _layer, pair.first);
		}


		queue.clear();
	}
#pragma endregion

	



	//[VARIABLES]
	std::map<LayerID, Layer> layers;
	Dogine::Camera main_camera(glm::vec3(0, 0, 5), 45.0f, 0.1f, 100.0f);
	int color_depth = 15;

	//[FUNCTIONS]
	void Init()
	{
		layers[LayerID::BG] = Layer(true, false, false, false, true, false); // Background
		layers[LayerID::OPAQUE] = Layer(false, true, true, false, true, true);  // Opaque
		layers[LayerID::TRANS] = Layer(true, true, false, true, true, true);  // Transparent
		layers[LayerID::HUD] = Layer(true, true, false, true, false, false); // HUD (custom framebuffer)
		layers[LayerID::WIN] = Layer(true, false, false, false, true, false); // UI (drawn to window)

		texture_dither = Dogine::LoadTexture("TexDither");
		screen_shader = Dogine::LoadShader("screen");

		screen_mesh = new Dogine::Mesh();
		screen_mesh->vertices.resize(4);
		screen_mesh->vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
		screen_mesh->vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
		screen_mesh->vertices[2] = glm::vec3(-1.0f, 1.0f, 0.0f);
		screen_mesh->vertices[3] = glm::vec3(1.0f, 1.0f, 0.0f);

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
	}



	void Enqueue(GameObject* _obj, int _depth, LayerID _layer)
	{
		auto it = layers.find(_layer);
		if (it == layers.end())
		{
			Log::Error("Cannot enqueue object: render layer does not exist.");
			return;
		}

		it->second.Enqueue(_obj, _depth);
	}
	void DrawLayer(LayerID _layer, glm::mat4 _view)
	{
		auto it = layers.find(_layer);
		if (it == layers.end())
		{
			Log::Error("Attempting to render an invalid layer.");
			return;
		}

		active_view = _view;

		it->second.Draw(_view, (uint8_t)_layer);
	}


	void DrawMain(GLuint _out)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		screen_shader->Activate();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _out);
		texture_dither->Bind(2);

		screen_shader->Set1I("tex0", 0);
		screen_shader->Set1I("tex_dither", 2);

		screen_shader->Set2F("resolution", Dogine::application_surface->GetWidth(), Dogine::application_surface->GetHeight());
		screen_shader->Set1F("color_depth", 1.0f / color_depth);
		screen_shader->Set4F("color_tint", 1, 1, 1, 1);
		//glUniform1i(glGetUniformLocation(screen_shader->shader_id, "tex0"), 0);


		screen_mesh->Bind();
		glDrawElements(GL_TRIANGLES, screen_mesh->GetTriangles(), GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}



	void Draw(Dogine::Mesh* mesh, const glm::vec3& _position, const glm::quat& _rotation, const glm::vec3& _scale)
	{
		glm::mat4 _matrix = glm::translate(glm::mat4(1.0f), _position);
		_matrix *= glm::toMat4(_rotation);
		_matrix = glm::scale(_matrix, _scale);

		auto _shader = Dogine::Shader::GetActive();

		_shader->SetMatrix4Fv("u_ViewProjection", 1, GL_FALSE, glm::value_ptr(active_view));
		_shader->SetMatrix4Fv("u_Model", 1, GL_FALSE, glm::value_ptr(_matrix));


		mesh->Bind();
		glDrawElements(GL_TRIANGLES, mesh->GetTriangles(), GL_UNSIGNED_INT, 0);
	}
}
