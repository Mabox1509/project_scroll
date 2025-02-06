#ifndef _CAMERA_H
#define _CAMERA_H
//[INCLUDES]
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//[CLASS]
class Camera
{
public:
	//[VARIABLES]
		glm::vec3 position;
		glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		float fov;
		float near_plane;
		float far_plane;

		bool is_orthographic;
		float orthographic_size;


	//[FUNCTIONS]
		Camera(glm::vec3 _position, float _fov, float _near, float _far);

		void Rotate(glm::vec3 _angle);
		glm::mat4 Matrix(glm::mat4 _object, float _aspect);
};




#endif // !_CAMERA_H



