#ifndef _ROOM_H
#define _ROOM_H
//[INCLUDES]
#include <list>
#include <string>
#include <memory>
#include <unordered_map>

#include "../dogine.h"
#include "gameobject.h"


//[CLASS]
class Room
{
public:
	//[GLOBALS]
	static std::unique_ptr<Room> loaded;
	static void Load(std::string _name);



	//[INSTANCE]
	std::unordered_map<int, GameObject*> objects_map;
	Dogine::Camera* main_camera;

	Room();
	~Room();


	void Update(double _dt);
	void Draw(double _dt);

private:
	std::list<std::unique_ptr<GameObject>> objects_list;

};



#endif // !_ROOM_H

