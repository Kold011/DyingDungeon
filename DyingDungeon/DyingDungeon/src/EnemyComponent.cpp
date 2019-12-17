#include "EnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"

CLASS_DEFINITION(Character, EnemyComponent)

void EnemyComponent::initialize(ENEMYID enemyID)
{
	SetHero(false);
	switch (enemyID)
	{
	case ENEMYID::Skeleton:
	{
		mBaseMaxHP = mCurrentHP = 100.0f;
		mBaseMaxMana = mCurrentMana = 100.0f;
		mAttack = 0.15f;
		mDefense = 0.05f;
		// Basic Attack
		mSkillList[0] = Skills(5, 0, Buffs(STATS::NONE, -5, 0, false, nullptr), "Basic Attack");
		// Skill 1 (Bleed)
		mSkillList[1] = Skills(10, 10, Buffs(STATS::HP, 0.15f, 2, true, nullptr), "Skeletal Slash");
		// Skill 2 (Big Damage & Bleed)
		mSkillList[2] = Skills(25, 40, Buffs(STATS::HP, 0.15f, 2, true, nullptr), "Necrotic Infection");
		break;
	}
	default:
		break;
	}
}

EnemyComponent::Move EnemyComponent::findBestMove(std::vector<std::shared_ptr<Odyssey::GameObject>> targets)
{
	Character* target = nullptr;
	for (std::shared_ptr<Odyssey::GameObject> t : targets)
	{
		if(target = t->getComponent<Character>())
		{
			if (target->IsHero() == true && target->IsDead() == false)
			{
				break;
			}
		}
	}

	Move bestMove;
	for (int i = 0; i < 4; i++)
	{
		float score = ScoreMove(mSkillList[i], target);

		if (score > bestMove.score)
		{
			bestMove.skill = &mSkillList[i];
			bestMove.target = target;
			bestMove.score = score;
		}
	}

	return bestMove;
}

float EnemyComponent::ScoreMove(Skills skillOption, Character* target)
{
	float score = skillOption.GetDamage() - skillOption.GetManaCost();
	if (target->GetHP() - skillOption.GetDamage() == 0 && GetMana() >= skillOption.GetManaCost())
		score += 1000;
	if (GetHP() > 60 && skillOption.GetName() == "Necrotic Infection" && GetMana() >= skillOption.GetManaCost())
		score += 25;
	if (GetMana() - skillOption.GetManaCost() <= 10)
		score -= 10;

	return score;
}

bool EnemyComponent::TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> targets)
{
	// Find my best option
	Move bestMove = findBestMove(targets);
	// Use the best move
	bestMove.skill->Use(*mGameObject->getComponent<Character>(), *bestMove.target);
	// If i have any buffs manage them 
	ManageStatusEffects();
	return true;
}

void EnemyComponent::Die()
{
	if (GetHP() <= 0)
	{
		SetDead(true);
		//TODO Uncomment for death animation
		//mGameObject->getComponent<Odyssey::Animator>()->setAnimationClip("Death");
	}
}