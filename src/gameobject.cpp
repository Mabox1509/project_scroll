//[INCLUDES]
#include "../inc/gameobject.h"

#include "../inc/roommanager.h"
#include "../dogine.h"



//[IMPLEMENTATION]
GameObject::GameObject(glm::vec3 _pos)
{
	position = _pos;
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);

	tag = "default";
	enable = true;
	persistent = false;



	//TODO: Set room id
	destroyed = false;
	RoomManager::instances.push_back(shared_from_this());
}
GameObject::~GameObject()
{

}

void GameObject::Interpret(std::vector<double> _args) {}

void GameObject::Start(){}
void GameObject::Update(double _dt){}
void GameObject::LateUpdate(double _dt){}
void GameObject::Draw(glm::mat4 _camera, uint8_t _layer, int _depth){}

void GameObject::Destroy()
{
	destroyed = true;
	RoomManager::ghosts.push_back(shared_from_this());

	auto _self = shared_from_this();
	RoomManager::instances.erase(std::remove(RoomManager::instances.begin(), RoomManager::instances.end(), _self), RoomManager::instances.end());
}


bool GameObject::IsDestroyed() { return destroyed; }