//[INCLUDES]
#include "../inc/gameobject.h"

#include "../inc/room.h"


//[IMPLEMENTATION]
GameObject::GameObject(glm::vec3 _pos)
{
	position = _pos;
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);

	memcpy(tag, "Default\0", 8);
	enable = true;
	visible = true;
	persistent = false;



	//TODO: Set room id
	id = 0;
}
GameObject::~GameObject(){}

void GameObject::Interpret(std::vector<double> _args) {}

void GameObject::Start(){}
void GameObject::Update(double _dt){}
void GameObject::LateUpdate(double _dt){}
void GameObject::Draw(glm::mat4 _camera){}



int GameObject::GetID() { return id; }

glm::mat4 GameObject::GetMatrix()
{
	glm::mat4 _matrix = glm::mat4(1.0f);


	_matrix = glm::translate(_matrix, position);
	_matrix = glm::rotate(_matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	_matrix = glm::rotate(_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	_matrix = glm::rotate(_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	_matrix = glm::scale(_matrix, scale);

	return _matrix;
}
