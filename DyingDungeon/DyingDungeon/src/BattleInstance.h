#pragma once
#include "MyTypeDefs.h"
#include <queue>
#include <time.h>

class BattleInstance
{

public: // Constructors
	BattleInstance(GameObjectList _playerTeam, GameObjectList _enemyTeam);

	enum BattleInstanceCommands
	{
		DESTORY = -1,
		CONTINUE = 1,
	};

public: // Functions

	int UpdateBattle();

	//Getters


	//Setters


private: // Varibales

	// Vectors
	GameObjectList mAllCharacters; // allCharacters will hold all of the game objects that are in the current battle
	GameObjectList mPlayerTeam; // playerTeam will hold the gameObjects associated with the Player's team
	GameObjectList mEnemyTeam; // enemyTeam will hold the gameObjects associated with the Overlord's team

	// Queues
	GameObjectQueue mBattleQueue; // Battle Queue that will hodl the order in which players can attack

	// GameObjects
	std::shared_ptr<Odyssey::GameObject> mCurrentCharacter; // This will hold the current player who's turn it is

	// Ints


	// Floats


	// Bools


private: // Functions
	void GenerateBattleQueue(); // This will generate the battle queue for the character turn orders
	bool IsTeamAlive(GameObjectList _teamToCheck); // This will check to see if at least one character from the passed in team is alive
};