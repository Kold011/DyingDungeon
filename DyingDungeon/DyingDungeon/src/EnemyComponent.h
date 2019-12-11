#pragma once
#include "Character.h"

class EnemyComponent : public Character
{
	CLASS_DECLARATION(EnemyComponent)

public:
	virtual void initialize(Odyssey::GameObject* parent);
	virtual void update(double deltaTime);
	struct Move 
	{
		Character* target;
		Skills* skill;
	};
	//Attack Functions
	Move findBestMove(std::vector<std::shared_ptr<Odyssey::GameObject>> targets, Skills* skills);
	void TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> targets);
	

	
	/////Get and Set Functions/////
	

	/////End of Get and Set Functions/////

private:

};

