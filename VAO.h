#ifndef _VAO_H
#define _VAO_H

//[INCLUDES]
#include <glad/glad.h>
#include "VBO.h"

//[CLASS]
class VAO
{
public:
	GLuint id;
	VAO();

	void LinkAttrib(VBO& _VBO, GLuint _layout, GLuint _num_components, GLenum _type, GLsizeiptr _stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};

#endif 

