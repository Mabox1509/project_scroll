//[INCLUDES]
#include "../inc/roommanager.h"
#include "../dogine.h"


//[VARIABLES]


//[NAMESPACE]
namespace RoomManager 
{
	//[VARIABLES]
	std::list<std::shared_ptr<GameObject>> instances;
	std::list<std::shared_ptr<GameObject>> ghosts;

	//[FUNCTIONS]
	void Load(std::string _room)
	{
	
	}

	void Update(double _dt)
	{
		if (instances.empty())
			return;

		for (auto& obj : instances)
		{
			if (obj->enable)
			{
				obj->Update(_dt);
			}
		}

	}

	void ClearGhosts()
	{
		if (ghosts.empty())
			return;

		for (auto it = RoomManager::ghosts.begin(); it != RoomManager::ghosts.end();)
		{
			auto& ghost = *it;

			// Verificar si el objeto tiene más referencias
			if (ghost.use_count() > 1)
			{
				Log::Error("Warning: Object at address %p with tag %s  has pending references!", ghost.get(), ghost->tag.c_str());
				Dogine::End(1);
				return;
			}

			it = RoomManager::ghosts.erase(it);
		}

		ghosts.clear();
	}
}