#include "Attack.h"
#include "Character.h"

Attack::Attack(std::string skillName, std::string animationId, float mpCost, float damage, StatusEffect* debuff, bool AOE)
{
	mTypeId = SKILLTYPE::ATTACK;
	mName = skillName;
	mAnimationId = animationId;
	mMpCost = mpCost;
	mDamage = damage;
	mDebuff = debuff;
	mIsAOE = AOE;
	mAdditionalEffect = nullptr;
}

Attack::Attack(std::string skillName, std::string animationId, float mpCost, float damage, StatusEffect* debuff, bool AOE, Skills* additionalEffect)
{
	mTypeId = SKILLTYPE::ATTACK;
	mName = skillName;
	mAnimationId = animationId;
	mMpCost = mpCost;
	mDamage = damage;
	mDebuff = debuff;
	mIsAOE = AOE;
	mAdditionalEffect = additionalEffect;
}

Attack::~Attack()
{
	if (mDebuff != nullptr)
	{
		delete mDebuff;
		mDebuff = nullptr;
	}
	if (mAdditionalEffect != nullptr)
	{
		delete mAdditionalEffect;
		mAdditionalEffect = nullptr;
	}
}

void Attack::Use(Character& caster, Character& target)
{
	caster.DepleteMana(mMpCost);
	float totalDps = 0.0f;
	totalDps = mDamage + (mDamage * caster.GetAtk());
	std::cout << caster.GetName() << " used " << mName << " on " << target.GetName() << " for ";
	target.TakeDamage(totalDps);
	if (mDebuff != nullptr)
	{
		if (mDebuff->GetRecipient() == nullptr)
			mDebuff->Apply(target);
		else
			mDebuff->Apply(*mDebuff->GetRecipient());
	}
	if (mAdditionalEffect != nullptr)
	{
		mAdditionalEffect->Use(caster, caster);
	}
}
float Attack::GetDamage()
{
	return mDamage;
}
StatusEffect* Attack::GetStatusEffect()
{
	return mDebuff;
}

void Attack::SetStatusEffect(StatusEffect* se)
{
	mDebuff = se;
}


