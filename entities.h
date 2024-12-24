#ifndef ENTITY_H
#define ENTITY_H

#include "model.h"

struct EntityCharacteristics {
	float moveSpeed;
	bool followPlayer;
};

class Entity {
public:
	Entity(Model model, EntityCharacteristics characteristics)
	{

	}

	~Entity()
	{

	}

private:


};

#endif // !ENTITIES_H
