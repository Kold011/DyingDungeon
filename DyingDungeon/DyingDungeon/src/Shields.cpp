#include "Shields.h"
#include "Character.h"

Shields::Shields(float amountOfAffect, int duration, Character* target)
{
	mTypeId = EFFECTTYPE::Shield;
	mAmountOfEffect = amountOfAffect;
	mDuration = duration;
	mRecipient = target;
	mAffectedStatId = STATS::None;
}

Shields::~Shields()
{
	mRecipient = nullptr;
}

void Shields::Apply(Character& target)
{
	std::shared_ptr<StatusEffect> newStatusEffect = nullptr;
	newStatusEffect = std::make_shared<Shields>(mAmountOfEffect, mDuration, &target);
	target.AddStatusEffect(newStatusEffect);
	return;
}

void Shields::Remove()
{
	return;
}

void Shields::Use()
{
	return;
}
