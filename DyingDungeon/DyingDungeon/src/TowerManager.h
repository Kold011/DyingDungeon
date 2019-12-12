#pragma once
#include "Component.h"
#include "Buffs.h"
#include <vector>
#include <time.h>

class TowerManager : public Odyssey::Component
{
	CLASS_DECLARATION(TowerManager);
	
public: // Constructors
	TowerManager() = default;

public: // Functions

	// Necessary
	virtual void initialize(Odyssey::GameObject* _parent);
	virtual void update(double deltaTime);

	//Getters
	std::vector<std::shared_ptr<Odyssey::GameObject>> GetEnemyTeam() { return mEnemyTeam; } // This will allow the battle instance to get the enemy team genreated by the Overlord

	std::shared_ptr<float> GetStatitics() { return mStatitics; } // This will return the statics that the Overlord will be using to construct a new enemy team

	//Setters
	void SetEnemyTeam(std::vector<std::shared_ptr<Odyssey::GameObject>> _newEnemyTeam) { mEnemyTeam = _newEnemyTeam; } // This will set the enemy team to the new enemy team that is generated by the Overlord

	void SetStatitics(std::shared_ptr<float> _newStatitics) { mStatitics = _newStatitics; } // This will set a list of new statitics to the mStatitics varibale that will be used in the Overlord

private: // Varibales

	// Vectors
	std::vector<std::shared_ptr<Odyssey::GameObject>> mAllCharacters; // allCharacters will hold all of the game objects that are in the current battle
	std::vector<std::shared_ptr<Odyssey::GameObject>> mPlayerTeam; // playerTeam will hold the gameObjects associated with the Player's team
	std::vector<std::shared_ptr<Odyssey::GameObject>> mEnemyTeam; // enemyTeam will hold the gameObjects associated with the Overlord's team
	std::vector<Buffs> mTowerBuffs; //This will hold the list of all of the buffs that the player has selected and put onto the Player's team
	// Queues


	// GameObjects


	// Ints


	// Floats
	std::shared_ptr<float> mStatitics;

	// Bools


private: // Functions
};

