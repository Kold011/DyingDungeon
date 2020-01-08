#pragma once
#include "Skills.h"
#include "Entity.h"

class Character;

class AIMoves
{
	struct Move
	{
		Character* target;
		std::shared_ptr<Skills> skill;
		float score = -10000;
	};

	public:
		AIMoves();
		AIMoves(int _enemyID, Character* _caster);
		~AIMoves() = default;
		bool FindMove(SKILLTYPE ovverride, std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

	private:
		//General Functions
		bool FindBestMove();
		float ScoreMove(std::shared_ptr<Skills> skill, Character* target);

		//Skeleton
		void SkeletonDeterminePriority();
		
		//UngaAttack
		void UngaAttackDeterminePriority();

		//Team Lists
		std::vector<std::shared_ptr<Odyssey::Entity>> mPlayerTeam;
		std::vector<std::shared_ptr<Odyssey::Entity>> mEnemyTeam;
		
		//Member Variables
		std::vector<std::shared_ptr<Skills>> mSkillList;
		std::vector<Odyssey::Entity> mBestTarget;
		Character* caster;

		std::shared_ptr<Move> mBestMove;
		int mEnemyID;
		int mCurrMoveCheck;
		SKILLTYPE mPriorityMove;


		//////////////////////////GET FUNCTIONS//////////////////////////////////
	public:
		std::shared_ptr<Move> GetMove();
};