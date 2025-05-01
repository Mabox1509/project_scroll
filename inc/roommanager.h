#ifndef _ROOM_MANAGER_H
#define _ROOM_MANAGER_H
//[INCLUDES]
#include <list>
#include <memory>
#include <string>

#include "gameobject.h"

//[NAMESPACE]
namespace RoomManager
{
	//[VARIABLES]
	extern std::list<std::shared_ptr<GameObject>> instances;
	extern std::list<std::shared_ptr<GameObject>> ghosts;

	//[FUNCTIONS]
	void Load(std::string _room);


	void Update(double _dt);

	void ClearGhosts();
}


#endif // !_ROOM_MANAGER_H
