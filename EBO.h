#ifndef _EBO_H
#define _EBO_H

//[INCLUDES]
#include <glad/glad.h>

//[CLASS]
class EBO
{
	public:
		GLuint id;
		EBO(GLuint* _indices, GLsizeiptr _size);

		void Bind();
		void Unbind();
		void Delete();
};

#endif 

