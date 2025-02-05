//[INCLUDES]
#include "resources.h"
#include <sstream>
#include <cerrno>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "log.h"
#include "compression.h"
 


//[VARIABLES]
std::unordered_map<std::string, Resources::file_info_t> raw_files;

std::map<std::string, std::weak_ptr<Resources::Texture>> textures;
std::map<std::string, std::weak_ptr<Resources::Audio>> audios;
std::map<std::string, std::weak_ptr<Resources::Shader>> shaders;




//[FUNCTIONS]
std::vector<char> GetFile(std::string _filename)
{
	std::ifstream macroFile(RESOURCES_PATH, std::ios::binary);
	if (!macroFile.is_open())
	{
		Log::Error("Cannot open resources file...");
		exit(1);
	}

	auto _entry = raw_files.find(_filename);
	if(_entry == raw_files.end())
	{
		Log::Error("Invalid asset path");
		exit(1);
	}

	Resources::file_info_t _info = _entry->second;
	std::vector<char> fileData(_info.size);

	macroFile.seekg(_info.position, std::ios::beg);
	macroFile.read(fileData.data(), _info.size);
	if (!macroFile) {
		std::cerr << "Error reading the file data" << std::endl;
		exit(1);
	}

	macroFile.close();
	return fileData;
}



//[IMPLEMENTATION]
void Resources::Init() 
{
	Log::Message("Loading resources...");

	std::ifstream macroFile(RESOURCES_PATH, std::ios::binary);
	if (!macroFile.is_open())
	{
		Log::Error("Cannot open resources file...");
		exit(1);
	}

	unsigned int numEntries = 0;
	macroFile.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries));
	unsigned char buffer[256];

	for (unsigned int i = 0; i < numEntries; ++i)
	{
		// Leer la ruta del archivo
		macroFile.getline(reinterpret_cast<char*>(buffer), sizeof(buffer), '\0');
		std::string filePath(reinterpret_cast<char*>(buffer));

		// Leer el tamaño del archivo
		size_t fileSize;
		macroFile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

		// Guardar la posición actual del archivo en el mapa
		file_info_t info;
		info.position = macroFile.tellg();
		info.size = fileSize;
		raw_files[filePath] = info;

		// Saltar el contenido del archivo
		macroFile.seekg(fileSize, std::ios::cur);

		Log::Message(filePath);
	}

	macroFile.close();

	std::cout << "\n\n";
}


std::shared_ptr<Resources::Texture> Resources::LoadTexture(std::string _name)
{
	auto _it = textures.find(_name);
	if (_it != textures.end())
	{
		if (!_it->second.expired())
		{
			return _it->second.lock();
		}
	}


	//LOAD DATA
	std::string _path = "Textures/" + _name + ".txc";
	auto _compress = GetFile(_path);
	auto _decompress = Compression::Decompress(_compress);

	// READ HEADER VALUES
	unsigned int _w, _h;
	memcpy(&_w, _decompress.data(), sizeof(int));
	memcpy(&_h, _decompress.data() + sizeof(int), sizeof(int));

	char _format = _decompress[8];
	bool _repeat = _decompress[10];
	bool _interpolation = _decompress[11];
	
	//DUMP DATA
	auto _texture = std::make_shared<Texture>(_w, _h, GL_TEXTURE0, _interpolation ? GL_LINEAR : GL_NEAREST, _repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	for (int _y = 0;_y < _h;_y++)
	{
		for (int _x = 0; _x < _w; _x++)
		{
			int _seek = 12 + (((_y * _w) + _x) * _format);

			switch (_format)
			{
			case 4:
				_texture->SetPixel(_x, _y, _decompress[_seek], _decompress[_seek + 1], _decompress[_seek + 2], _decompress[_seek + 3]);
				break;
				
			case 3:
				_texture->SetPixel(_x, _y, _decompress[_seek], _decompress[_seek + 1], _decompress[_seek + 2], 255);
				break;

			case 2:
				_texture->SetPixel(_x, _y, _decompress[_seek], _decompress[_seek], _decompress[_seek], _decompress[_seek+1]);
				break;

			case 1:
				_texture->SetPixel(_x, _y, _decompress[_seek], _decompress[_seek], _decompress[_seek], 255);
				break;
			}
		}
	}

	_texture->Apply();


	textures[_name] = _texture;
	return _texture;
}
std::shared_ptr<Resources::Shader> Resources::LoadShader(std::string _name)
{
	auto _it = shaders.find(_name);
	if(_it != shaders.end())
	{
		if(!_it->second.expired())
		{
			return _it->second.lock();
		}
	}


	std::string _path = "Shaders/" + _name;
	std::vector<char> vertexData = GetFile(_path + "/vertex.bnc");
	std::vector<char> fragmentData = GetFile(_path + "/fragment.bnc");

	// Convierte std::vector<char> a std::string
	std::string _vertex(vertexData.begin(), vertexData.end());
	std::string _fragment(fragmentData.begin(), fragmentData.end());


	auto _shader = std::make_shared<Shader>(_vertex.c_str(), _fragment.c_str());

	shaders[_name] = _shader;

	return _shader;
}