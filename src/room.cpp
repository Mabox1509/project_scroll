//[INCLUDES]
#include "../dogine.h"

#include "../inc/room.h"


//[VARIABLES]
std::unique_ptr<Room> Room::loaded;


//[IMPLEMENTATION]
void Room::Load(std::string _name)
{


}

Room::Room()
{
	main_camera = new Dogine::Camera(glm::vec3(0, 0, 5), 45.0f, 0.1f, 100);
}
Room::~Room(){}

void Room::Update(double _dt)
{

}
void Room::Draw(double _dt)
{

}