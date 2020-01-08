#pragma once
#include "Character.h"
#include "InputManager.h"

typedef std::vector<std::shared_ptr<Odyssey::Entity>> EntityList;
class HeroComponent : public Character
{
	CLASS_DECLARATION(HeroComponent)

public:
	HeroComponent() = default;
	HeroComponent(HEROID id);
	~HeroComponent();
	//Attack Functions
	virtual bool TakeTurn(EntityList heros, EntityList enemies);

	//Death Check
	virtual void Die();
	/////Get and Set Functions/////
	/////End of Get and Set Functions/////
private:
	Skills* mCurrentSkill;
	Character* mCurrentTarget;
	// State functions
	void SelctionState(int moveIndex);
	void SelectTarget(EntityList heros, EntityList enemies, int targetIndex);
	void ResetToSelection();
};

