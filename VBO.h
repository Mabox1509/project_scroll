#ifndef _VBO_H
#define _VBO_H

//[INCLUDES]
#include <glad/glad.h>

//[CLASS]
class VBO {

	public:
		GLuint id;
		VBO(GLfloat* _vertices, GLsizeiptr _size);

		void Bind();
		void Unbind();
		void Delete();
};


#endif // !_VBO_H