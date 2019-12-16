#pragma once
#include "Component.h"
#include "Skills.h"
#include <vector>

enum class HEROID { Paladin = 0 };
enum class ENEMYID { Skeleton = 0 };

class Character : public Odyssey::Component
{
	CLASS_DECLARATION(Character)

public:
	Character() = default;
public:
	virtual void initialize();
	virtual void update(double deltaTime);

	//Attack Functions
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> _allCharacters);
	virtual void Die();

	void TakeDamage(float dmg);
	void DepleteMana(float manaCost);

	/////Get and Set Functions/////
	// HP Functions
	float GetHP();
	void SetHP(float m_HP);
	float GetMaxHP();
	// Mana Functions
	float GetMana();
	void SetMana(float Mana);
	// Attack Functions
	float GetAtk();
	// Defense Functions
	float GetDef();
	// Dead Status Functions
	bool IsDead();
	void SetDead(bool deadStatus);
	// EXP Functions
	void AddExp(float exp);
	// IsHero Functions
	bool IsHero();
	void SetHero(bool heroStat);
	// Name Functions
	std::string GetName();
	void SetName(std::string newName);
	// Skill Functions
	void SetSkills(Skills newSkillList);
	Skills* GetSkills();
	// Status Effect Functions
	void AddStatusEffect(Buffs* newEffect);
	void ManageStatusEffects();
	/////End of Get and Set Functions/////
protected:
	bool mHero;
	bool mDead;
	//Stats
	float mCurrentHP;
	float mCurrentMana;
	float mBaseMaxHP;
	float mBaseMaxMana;
	float mAttack;
	float mDefense;
	float mEXP;
	std::string mName;
	Skills mSkillList[4];
	std::vector<Buffs*> mStatusEffects;
private:

};

