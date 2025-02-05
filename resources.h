#ifndef _RESOURCES_H
#define _RESOURCES_H

//[INCLUDES]
#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define RESOURCES_PATH "resources"

//[NAMESPACE]
namespace Resources
{
	typedef struct file_info_t
	{
		size_t position;
		size_t size;
	} file_info_t;

	//[CLASSES]
	class Shader
	{
	public:
		//[VARIABLES]
		GLuint shader_id;

		//[CONSTRUCTOR]
		Shader(const char* _vertex, const char* _fragment);
		~Shader();

		//[FUNCTIONS]
		void Activate();
		void Delete();
	};
	class Texture
	{
		unsigned int* buffer;
		int w, h;

	public:
		GLuint id;

		Texture(int _w, int _h, GLenum _slot, GLuint _filter, GLuint _warp);
		~Texture();

		void TexUnit(Shader& _shader, const char* _uniform, GLuint unit);

		void Bind();
		void Unbind();


		void SetPixel(int _x, int _y, uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a);
		unsigned int GetPixel(int _x, int _y);
		void Apply();

		int GetWidth();
		int GetHeight();
		
	};
	class Audio{};


	//FUNCTIONS
	void Init();

	std::shared_ptr<Texture> LoadTexture(std::string _name);
	std::shared_ptr<Shader> LoadShader(std::string _name);
}

#endif // !_RESOURCES_H