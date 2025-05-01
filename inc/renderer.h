#ifndef _RENDERER_H
#define _RENDERER_H

//[INCLUDES]
#include <map>
#include <glm/glm.hpp>


#include "gameobject.h"
#include "../dogine.h"


namespace Renderer
{
	//[TYPES]
	enum class LayerID : uint8_t {
		BG,       // background / skybox
		OPAQUE,   // opaque objects
		TRANS,    // transparent objects
		HUD,      // HUD inside custom framebuffer
		WIN       // UI drawn directly to the window
	};

	class Layer
	{
	private:
		bool blending;       // enable alpha blending
		bool ztest;          // enable depth testing
		bool zwriting;       // enable writing to depth buffer
		bool copy_buffer;    // copy framebuffer before drawing this layer
		bool fix_depth;      // shift depth if conflict exists
		bool face_culling; // enable or disable back face culling

		std::map<int, GameObject*> queue;  // depth-sorted render queue

	public:
		// Constructor
		Layer(bool _blending, bool _ztest, bool _zwriting, bool _copy, bool _fix_depth, bool _face_culling);
		Layer();

		// Methods
		void Enqueue(GameObject* _obj, int _depth);
		void Draw(glm::mat4 _view, uint8_t _layer);
	};



	//[VARIABLES]
	extern std::map<LayerID, Layer> layers;
	extern Dogine::Camera main_camera;
	extern int color_depth;



	//[FUNCTIONS]
	void Init();

	void Enqueue(GameObject* _obj, int _depth, LayerID _layer);
	void DrawLayer(LayerID _layer, glm::mat4 _view);

	void DrawMain(GLuint _out);



	//DRAW FUNCTIONS
	void Draw(Dogine::Mesh* mesh, const glm::vec3& _position, const glm::quat& _rotation, const glm::vec3& _scale);

}

#endif // !_RENDERER_H
