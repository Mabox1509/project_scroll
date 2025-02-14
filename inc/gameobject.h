#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
//[INCLUDES]
#include <vector>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

//[CLASS]
class GameObject
{
public:
	//[VARIABLES]
	bool enable;
	bool visible;
	bool persistent;

	char tag[8];

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;


	GameObject(glm::vec3 _pos);
	~GameObject();

	virtual void Interpret(std::vector<double> _args);

	virtual void Start();

	virtual void Update(double _dt);
	virtual void LateUpdate(double _dt);

	virtual void Draw(glm::mat4 _camera);


	int GetID();
protected:
	glm::mat4 GetMatrix();


private:
	int id;
	
};
#endif // !_GAMEOBJECT_H
