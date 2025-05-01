#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
//[INCLUDES]
#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

//[CLASS]
class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	//[VARIABLES]
	bool enable;
	bool persistent;


	std::string tag;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;



	GameObject(glm::vec3 _pos);
	~GameObject();

	virtual void Interpret(std::vector<double> _args);

	virtual void Start();

	virtual void Update(double _dt);
	virtual void LateUpdate(double _dt);

	virtual void Draw(glm::mat4 _camera, uint8_t _layer, int _depth);


	void Destroy();	
	bool IsDestroyed();


private:
	bool destroyed;

	
};
#endif // !_GAMEOBJECT_H
