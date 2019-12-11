#include "HeroComponent.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Component, HeroComponent)

void HeroComponent::initialize(Odyssey::GameObject* parent)
{
	onEnable();
	mGameObject = parent;
	mGameObject->addComponent<Odyssey::Transform>();

	mHP = 100;
	mMana = 50;
	skillList[0] = Skills(10, 5);
}

void HeroComponent::update(double deltaTime)
{

}

void HeroComponent::TakeTurn()
{

}

/*
 * Function:  BasicAttack()
 * --------------------
 * Uses basic attack (skillList[0])
 *
 * Currently a public function but will be moved
 * to a private one
 *
 * returns: void
 */
void HeroComponent::BasicAttack(EnemyComponent& target)
{
	skillList[0].Use(target);
}

/*
 * Function:  GetMana()
 * --------------------
 * Gets the HP of the enemy
 *
 * returns: A float representing current HP
 */
float HeroComponent::GetHP()
{
	return mHP;
}

/*
 * Function:  SetHP(float HP)
 * --------------------
 * Sets the HP of the enemy to the passed in float
 *
 * returns: nothing
 */
void HeroComponent::SetHP(float HP)
{
	mHP = HP;
}

/*
 * Function:  GetMana()
 * --------------------
 * Gets the mana of the enemy
 *
 * returns: A float representing mana
 */
float HeroComponent::GetMana()
{
	return mMana;
}

/*
 * Function:  SetMana(float Mana)
 * --------------------
 * Sets the Mana of the enemy to the passed in float
 *
 * returns: nothing
 */
void HeroComponent::SetMana(float Mana)
{
	mMana = Mana;
}