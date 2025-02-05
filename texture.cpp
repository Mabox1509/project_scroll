//[INCLUDES]
#include "resources.h"
#include <cerrno>

#include "log.h"

//[IMPLEMENTATION]
Resources::Texture::Texture(int _w, int _h, GLenum _slot, GLuint _filter = GL_LINEAR, GLuint _warp = GL_REPEAT)
{
	//ALLOC RAM BUFFER
	size_t _size = (_w * _h) * sizeof(int);

	buffer = (unsigned int*)malloc(_size);
	memset(buffer, 0, _size);
	w = _w;
	h = _h;


	//ALLOC VRAM
    glGenTextures(1, &id);
    glActiveTexture(_slot);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    Log::Message("New texture with id %d", id);
}

Resources::Texture::~Texture()
{
    glDeleteTextures(1, &id);
    free(buffer);
    Log::Message("Texture with id %d deleted", id);
}


void Resources::Texture::TexUnit(Shader& _shader, const char* _uniform, GLuint unit)
{
    GLuint tex_uni = glGetUniformLocation(_shader.shader_id, _uniform);
    _shader.Activate();

    glUniform1i(tex_uni, unit);
}


void Resources::Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}
void Resources::Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Resources::Texture::SetPixel(int _x, int _y, uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
{
    int _seek = (((h-1)-_y) * w) + _x;

    // Combinar los canales en un solo int (suponiendo un formato RGBA)
    int color = (_a << 24) | (_b << 16) | (_g << 8) | _r;

    // Asignar el valor combinado al buffer
    buffer[_seek] = color;
}
unsigned int Resources::Texture::GetPixel(int _x, int _y)
{
    int _seek = (_y * w) + _x;

    return buffer[_seek];
}
void Resources::Texture::Apply()
{
    Bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    Unbind();
}

int Resources::Texture::GetWidth() { return w; }
int Resources::Texture::GetHeight() { return h; }