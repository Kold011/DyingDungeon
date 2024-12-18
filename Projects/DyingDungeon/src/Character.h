#pragma once
#include "Component.h"
#include "Animator.h"
#include "Rectangle2D.h"
#include "Text2D.h"
#include "Skills.h"
#include "GameUIManager.h"
#include <vector>

// Character State identification enum
enum class STATE { NONE = 0, STUNNED, SELECTMOVE, SELECTTARGET, CONFIRM, INPROGRESS, FINISHED, DEAD };

// #defines for doing the stats math
#define BASEATK 40.0f
#define BASEDEF 40.0f
#define BASESPD 40.0f

class Character : public Odyssey::Component
{
	CLASS_DECLARATION(Character)

public:
	Character();

	// Struct to hold all data needed for animations
	struct AnimationImportData
	{
		std::string mAnimationNickName = "NULLNAME";
		std::string mAnimationPath = "NULLPATH";
		bool mIsLooping = true;
	};
public:
	//Virtual functions from Component base class
	virtual void initialize();
	virtual void update(double deltaTime);

	// Function that gets called by battle instance to allow the character to take their turn
	virtual bool TakeTurn(std::vector<Odyssey::Entity*> playerTeam, std::vector<Odyssey::Entity*> enemyTeam);

	// Function that gets called to set the character state to dead, along with all other necessary variables
	virtual void Die();

	// Returns the vector of shared pointer to the characters skills
	virtual std::vector<std::shared_ptr<Skills>> GetSkills();

	// Reduces the characters current health
	void TakeDamage(float dmg);

	// Increase the characters current health
	void ReceiveHealing(float healing);

	// Depletes the characters current mana
	void DepleteMana(float manaCost);

	// Returns the characters current heath
	float GetHP();

	// Sets the characters current heath
	void SetHP(float m_HP);

	// Returns the characters max heath
	float GetMaxHP();

	// Returns the characters current mana
	float GetMana();

	// Sets the characters current mana
	void SetMana(float Mana);

	// Returns the characters max mana
	float GetMaxMana();

	// Returns the characters current Attack stat
	float GetAtk();

	// Returns the characters base attack stat
	float GetBaseAtk();

	// Returns the attack mod stat
	float GetAtkMod();

	// Increase the characters current Attack stat
	void IncreaseAtk(float statIncrease);

	// Decrease the characters current Attack stat
	void DecreaseAtk(float statDecrease);

	// Returns the characters current Defense stat
	float GetDef();

	// Returns the characters base Defense stat
	float GetBaseDef();

	// Returns the defense mod stat
	float GetDefMod();

	// Increase the characters current Defense stat
	void IncreaseDef(float statIncrease);

	// Decrease the characters current Defense stat
	void DecreaseDef(float statDecrease);

	// Return the characters current speed stat
	float GetSpeed();

	// Return the characters base speed stat
	float GetBaseSpeed();

	// Returns the Speed mod stat
	float GetSpdMod();

	// Increase the characters current speed stat
	void IncreaseSpd(float statIncrease);

	// Decrease the characters current speed stat
	void DecreaseSpd(float statDecrease);

	// Returns the characters current exp
	float GetExp();

	// Increases the characters current exp
	void AddExp(float exp);

	// returns true if the character is a hero, false if its a enemy
	bool IsHero();

	// Set if the character is a hero or not
	void SetHero(bool heroStat);

	// Returns the characters name
	std::wstring GetName();

	// Sets the characters name 
	void SetName(std::wstring newName);

	// Gets the characters theme color
	DirectX::XMFLOAT3 GetThemeColor();

	// Resets the characters
	void ResetMe();

	// Returns the pointer to the character that has provoked this character 
	Character* GetProvoked();

	// Sets the pointer to a character who has provoked this character
	void SetProvoked(Character* provoker);

	// Returns the current state of the character
	STATE GetState();

	// Sets the current state of the character
	void SetState(STATE newState);

	// Sets the impact indicator entity used for targeting 
	void SetImpactIndicator(Odyssey::Entity* _impactIndicatorModel) { mImpactIndicator = _impactIndicatorModel; }

	// Gets the impact indicator entity used for targeting
	Odyssey::Entity* GetInpactIndicator() { return mImpactIndicator; }

	// Adds a status effect to the character and sorts it putting it in the correct vector
	bool AddStatusEffect(std::shared_ptr<StatusEffect> newEffect, Character* caster);

	// Manages the passed in status effect, appling effects and removing expired ones
	void ManageStatusEffects(std::vector<std::shared_ptr<StatusEffect>>& effectList);

	// Manage status effects I casted
	void ManageCastedEffects();

	// Add a status effect I cast to the Casted list
	void AddCastedEffect(std::shared_ptr<StatusEffect> newCastedEffect);

	// Manages all status effects, appling effects and removing expired ones
	bool ManageTOREffects();

	// Removes all status effects from characters
	void ClearStatusEffects();

	// Remove all harmful status effects
	void ClearBadStatusEffects();

	// Returns if there is a shield or not on the character
	bool IsShielded();

	// Sets the Particle system pointer to a "Hit effect"
	void SetBloodPrefab(Odyssey::Entity* newBloodEffectPrefab);

	// Returns the Particle system pointer to a "Hit effect"
	Odyssey::Entity* GetBloodPrefab();

	// Spawn Blood Effect At Character's Position
	void SpawnBloodEffect();

	// Turns all active particle effects to inactive
	void StopParticleEffects();

	// Returns the character portrait file path
	std::wstring GetPortraitPath();

	// Returns the vector of animation import data containing the animation paths, names, and bools
	std::vector<AnimationImportData> GetAnimationPaths();

	// Returns the current level of the character
	unsigned int GetLevel();

	// Returns the sub-name of the character
	std::wstring GetSubName();

	// Returns the description of the character
	std::wstring GetDescription();

	// Returns the models file path name
	std::string GetModel();

	// Sets the description of the character
	void SetDescription(std::wstring newDescription);

	// Set the characters Hud index
	void SetHudIndex(unsigned int newIndex);

	// Returns the character hud index
	unsigned int GetHudIndex();

	// Adds a soundClip to the map
	void AddSoundClip(std::string soundKey, std::string newSoundName);

	// Returns a string used for a sound clip
	std::string GetSoundClipName(std::string soundKey);

protected:
	// Bool to tell if character is a hero or enemy
	bool mHero;

	// Bool for if im affected by a bleed
	bool mIsBleeding;
	int mBleedTimer;

	// Bool for telling if im regenreating or not
	bool mIsRegenerating;
	int mRegenTimer;

	// Timer for how long im stunned
	int mStunTimer;

	// A pointer to a character that get sets whenever the character is provoked  
	Character* mProvoked;
	int mProvokedTimer;

	// how much a character has of temp health
	float mShielding;
	int mShieldTimer;

	// Map of strings for sound effects
	std::map<std::string, std::string> mSoundClips;

	// Character hud index
	unsigned int mHudIndex;

	// Characters current level
	unsigned int mCurrentLevel;

	// Float for the current HP of the character
	float mCurrentHP;

	// Float for the current MP of the character
	float mCurrentMana;

	// Float for the Max HP of the character
	float mBaseMaxHP;

	// Float for the Max MP of the character
	float mBaseMaxMana;

	// Float for the current Attack of the character
	float mAttack;

	// Float for the base Attack of the character
	float mBaseAttack;

	// Float for the current Defense of the character
	float mDefense;

	// Float for the base Defense of the character
	float mBaseDefense;

	// Float for the current Speed of the character
	float mSpeed;

	// Float for the base Speed of the character
	float mBaseSpeed;

	// Float for the current exp of the character
	float mEXP;

	// The name for the character
	std::wstring mName;

	// The sub-name for the character
	std::wstring mSubName;

	// The theme color for the character
	DirectX::XMFLOAT3 mThemeColor;

	// Name of file path for character portrait
	std::wstring mPortrait;

	// Name of Model used for character
	std::string mModel;

	// Character Description
	std::wstring mDescription;

	// Vector of animation import data used for character
	std::vector<AnimationImportData> mAnimations;
	
	// Vector of shared pointers that point to the skills the character has
	std::vector<std::shared_ptr<Skills>> mSkillList;

	// Vector of shared pointers that point to the Debuffs the character has
	//std::vector<std::shared_ptr<StatusEffect>> mDebuffs;

	// Vector of shared pointers that point to the Buffs the character has
	//std::vector<std::shared_ptr<StatusEffect>> mBuffs;

	// Vector of shared pointers that point to the Bleeds the character has
	//std::vector<std::shared_ptr<StatusEffect>> mBleeds;

	// Vector of shared pointers that point to the Regens the character has
	//std::vector<std::shared_ptr<StatusEffect>> mRegens;

	// Vector of shared pointers that point to the Sheilds the character has
	//std::vector<StatusEffect*> mSheilds;

	// Vector of raw pointer to effects affecting me
	std::vector<StatusEffect*> mSE;

	// Vector of pointers for effects I casted 
	std::vector<std::shared_ptr<StatusEffect>> mCastedEffects;

	// Pointer to the animator the character uses
	Odyssey::Animator* mAnimator;

	// The current state of the character used in the hero/enemy components state machine 
	STATE mCurrentState;

	// Pointer to the particle system used for "getting hit" effects
	Odyssey::Entity* mBloodEffectPrefab;

	public:
	// entity used for targeting 
	Odyssey::Entity* mImpactIndicator;
};

