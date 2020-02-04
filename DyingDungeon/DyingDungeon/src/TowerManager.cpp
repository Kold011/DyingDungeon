#include "TowerManager.h"
#include "Transform.h"
#include "Character.h"
#include "HeroComponent.h"
#include "InputManager.h"
#include "EventManager.h"
#include "StatusEvents.h"
#include "GameUIManager.h"
#include "StatusEvents.h"
#include "UICanvas.h"
#include "RedAudioManager.h"

CLASS_DEFINITION(Component, TowerManager)

TowerManager::~TowerManager()
{
	DestroyBattleInstance();
}

void TowerManager::initialize()
{
	// Reset cheat code bools
	mUsedBossCheatCode = false;

	// The tower will not be paused on start up
	mIsPaused = false;

	// Don't show the boos character
	mBossCharacter->setActive(false);

	// Reset the enemy team to the skely bois
	if (mSkeletonTeam.size() != 0)
	{
		// Clear enemy team
		mEnemyTeam.clear();

		// Set the enemy team to be the skeleton team on start
		for (int i = 0; i < mSkeletonTeam.size(); i++)
		{
			mSkeletonTeam[i]->setActive(true);
			mEnemyTeam.push_back(mSkeletonTeam[i]);
		}
	}

	// Create a Battle when we set up the tower !!THIS WILL BE TEMPORARY!!
	CreateBattleInstance();

	// Set the pause menu button callbacks
	GameUIManager::getInstance().GetResumeButton()->registerCallback("onMouseClick", this, &TowerManager::TogglePauseMenu);
	GameUIManager::getInstance().GetOptionsButton()->registerCallback("onMouseClick", this, &TowerManager::ShowOptionsMenu);
	GameUIManager::getInstance().GetMainMenuButton()->registerCallback("onMouseClick", this, &TowerManager::GoToMainMenu);
}

void TowerManager::update(double deltaTime)
{
	// Always look for the pause input button
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::P))
	{
		// Turn the pause menu either on or off
		TogglePauseMenu();
	}

	// Toggles the combat canvas
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Tab))
	{
		Odyssey::UICanvas* combatCanvas = GameUIManager::getInstance().GetCombatLogCanvas();

		GameUIManager::getInstance().ToggleCanvas(combatCanvas, !combatCanvas->isActive());
	}

	// Go straight to the BOSS when F3 is hit
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::F3))
	{
		// Destroy the current battle instance
		DestroyBattleInstance();
		// Set the game to in rewards 
		mTowerState = IN_REWARDS;
		// Set the tower level to the last level which is the boss
		mCurrentLevel = mNumberOfLevels;
		// Set the cheat code bool
		mUsedBossCheatCode = true;
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::M))
	{
		if (!RedAudioManager::Instance().isMuted())
		{
			RedAudioManager::Instance().Mute();
		}
		else
		{
			RedAudioManager::Instance().Unmute();
		}
	}

	// Don't update unless the game is not paused
	if (!mIsPaused)
	{
		// Update the health popups
		GameUIManager::getInstance().UpdateCharacterHealthPopups(deltaTime);

		// If we are in battle, Update the battle
		if (GetTowerState() == IN_BATTLE)
		{
			int result = mCurrentBattle->UpdateBattle();

			// If the result of the updated battle was DESTROY, destory the current battle instance
			if (result == mCurrentBattle->PLAYER_TEAM_DIED || result == mCurrentBattle->DESTORY)
			{
				// Destroy the battle instance
				DestroyBattleInstance();
				SetTowerState(IN_REWARDS);
			    Odyssey::EventManager::getInstance().publish(new RewardsActiveEvent(mCurrentLevel));
				Rewards->setActive(true);

				//Check to see if the update returned PLAYER_TEAM_DIED
				if (result == mCurrentBattle->PLAYER_TEAM_DIED)
				{
					std::cout << "You FAILED to complete the tower, Get Better\n" << std::endl;
					std::cout << "Your team DIED!!!!!!!\n" << std::endl;

					SetTowerState(NOT_IN_BATTLE);
					// Go to main menu screen
					GoToMainMenu();
				}
				else
				{
					// Update to the next level
					mCurrentLevel = GetCurrentLevel() + 1;
				}
			}
		}
		else if (GetTowerState() == IN_REWARDS)
		{
			if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Enter) || mUsedBossCheatCode)
			{
				// Reset cheat code bool
				mUsedBossCheatCode = false;
				// Create temp xp variable
				float tempXP = 0.0f;

				// Check to see if that was our last level for completing the tower
				if (GetCurrentLevel() > mNumberOfLevels)
				{
					std::cout << "You have completed the tower, Congratulations\n" << std::endl;
					SetTowerState(NOT_IN_BATTLE);

					// Give player some XP
					tempXP = 500.0f;
					// Print how much XP was given to the players
					for (int i = 0; i < mPlayerTeam.size(); i++)
					{
						mPlayerTeam[i]->getComponent<Character>()->AddExp(tempXP);
					}
					std::cout << "Player team characters gained " << tempXP << "XP for completing the tower.\n" << std::endl;
					// Go to main menu screen
					GoToMainMenu();
				}
				else
				{
					// If this is the last level of the tower, spawn the boss
					if (mCurrentLevel == mNumberOfLevels)
					{
						// Turn off the other enemies
						for (int i = 0; i < mEnemyTeam.size(); i++)
						{
							mEnemyTeam[i]->setActive(false);
							mSkeletonTeam.push_back(mEnemyTeam[i]);
						}
						// Clear all enemies from the current enemy list
						mEnemyTeam.clear();

						// Now active the boos and only add the boss to the enemy list
						mBossCharacter->setActive(true);
						mEnemyTeam.push_back(mBossCharacter);
					}

					std::cout << "The current level is " << mCurrentLevel << "\n" << std::endl;

					// Give player some XP
					tempXP = 100.0f;
					// Print how much XP was given to the player
					for (int i = 0; i < mPlayerTeam.size(); i++)
					{
						Character* currCharacter = mPlayerTeam[i]->getComponent<Character>();
						currCharacter->AddExp(tempXP);

						// If the player made the finishing kill set him back to NONE state
						if (currCharacter->GetState() == STATE::FINISHED)
						{
							currCharacter->SetState(STATE::NONE);
						}
					}
					std::cout << "Player team characters gained " << tempXP << "XP for completing the level.\n" << std::endl;

					// Make a new battle to continue the tower
					CreateBattleInstance();
				}

				// Turn off the rewads screen
				Rewards->setActive(false);
			}
		}
		else if (GetTowerState() == NOT_IN_BATTLE)
		{

		}
	}
}

void TowerManager::SetUpTowerManager(EntityList _playerTeam, EntityList _enemyTeam, int _numberOfBattles, std::shared_ptr<Odyssey::Entity> _turnIndicatorModel)
{
	// Assign the player team and the enemy team
	mPlayerTeam = _playerTeam;
	mEnemyTeam = _enemyTeam;

	// Add all of the characters from the player's team to the allCharacters vector
	for (int i = 0; i < mPlayerTeam.size(); i++)
		mAllCharacters.push_back(mPlayerTeam[i]);

	// Add all of the characters from the enemy's team to the allCharacters vector
	for (int i = 0; i < mEnemyTeam.size(); i++)
		mAllCharacters.push_back(mEnemyTeam[i]);

	// Add Boss to the mAllCharacters
	mAllCharacters.push_back(mBossCharacter);

	// Set the current level to 1
	mCurrentLevel = 1;

	// Set the number of levels for this tower
	mNumberOfLevels = _numberOfBattles;
	mCurrentBattle = nullptr;

	// Set the turn indicator model
	tmTurnIndicator = _turnIndicatorModel;
}

void TowerManager::CreateBattleInstance()
{
	// TODO: REMOVE POST BUILD 02
	if (mCurrentLevel == 1)
		system("CLS");

	// Send off the current level number
	Odyssey::EventManager::getInstance().publish(new LevelStartEvent(mCurrentLevel, mPlayerTeam[0]->getComponent<Character>()->GetName(), mPlayerTeam[1]->getComponent<Character>()->GetName(), mPlayerTeam[2]->getComponent<Character>()->GetName(),
																					mPlayerTeam[0]->getComponent<Character>()->GetPortraitPath(), mPlayerTeam[1]->getComponent<Character>()->GetPortraitPath(), mPlayerTeam[2]->getComponent<Character>()->GetPortraitPath()));

	// Create the battle instance
	mCurrentBattle = new BattleInstance(mPlayerTeam, mEnemyTeam, tmTurnIndicator);

	// Since we created a BattleInstance we will be in combat
	SetTowerState(IN_BATTLE);

	std::cout << "Created a battle instance\n" << std::endl;
	std::cout << "- Player Team\n" << std::endl;
	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		Character* myChar = mPlayerTeam[i]->getComponent<Character>();
	}

	std::cout << "- Enemy Team\n" << std::endl;
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		Character* myChar = mEnemyTeam[i]->getComponent<Character>();
	}

	std::cout << "The current level is " << mCurrentLevel << "\n" << std::endl;

}

void TowerManager::DestroyBattleInstance()
{
	// Destory pointer and set it to a nullptr
	if (mCurrentBattle)
	{
		delete mCurrentBattle;
		mCurrentBattle = nullptr;
	}
	
	// Since we destoryed the BattleInstance we will NOT be in combat
	SetTowerState(NOT_IN_BATTLE);

	std::cout << "Destroyed a battle instance\n" << std::endl;
}

void TowerManager::TogglePauseMenu()
{
	//Toggle mIsPaused bool
	mIsPaused = !mIsPaused;

	// Toggle pause menu canvas
	Odyssey::UICanvas* pauseMenuCanvas = GameUIManager::getInstance().GetPauseMenu()->getComponent<Odyssey::UICanvas>();
	GameUIManager::getInstance().ToggleCanvas(pauseMenuCanvas, !pauseMenuCanvas->isActive());

	// Loop through all of the characters and toggle their animator
	for (int i = 0; i < mAllCharacters.size(); i++)
		mAllCharacters[i]->getComponent<Odyssey::Animator>()->setActive(!mAllCharacters[i]->getComponent<Odyssey::Animator>()->isActive());
}

void TowerManager::ShowOptionsMenu()
{
	// Turn off the pause menu
	Odyssey::UICanvas* pauseMenuCanvas = GameUIManager::getInstance().GetPauseMenu()->getComponent<Odyssey::UICanvas>();
	GameUIManager::getInstance().ToggleCanvas(pauseMenuCanvas, false);

	// Turn on the options menu
	Odyssey::UICanvas* optionsMenuCanvas = GameUIManager::getInstance().GetOptionsMenu()->getComponent<Odyssey::UICanvas>();
	GameUIManager::getInstance().ToggleCanvas(optionsMenuCanvas, true);
}

void TowerManager::GoToMainMenu()
{
	SetTowerState(NOT_IN_BATTLE);

	for (int i = 0; i < mPlayerTeam.size(); i++)
	{
		if (mPlayerTeam[i] != nullptr)
		{
			// Set all of the healths for each player on the enemy team back to 100 and their dead status to false
			// This will show a sim of entering a new battle
			mPlayerTeam[i]->getComponent<Character>()->ResetMe();
			mPlayerTeam[i]->getComponent<Odyssey::Animator>()->playClip("Idle");
		}

		// TODO: REFACTOR LATER
		scene->removeEntity(mPlayerTeam[i].get());
	}

	// TODO: REFACTOR LATER
	for (int i = 0; i < mEnemyTeam.size(); i++)
	{
		if (mEnemyTeam[i])
		{
			scene->removeEntity(mEnemyTeam[i].get());
		}
	}

	// Deactivate the rewards screen
	Rewards->setActive(false);
	// Deactivate the pause menu
	std::shared_ptr<Odyssey::Entity> pauseMenu = GameUIManager::getInstance().GetPauseMenu();
	GameUIManager::getInstance().ToggleCanvas(pauseMenu->getComponent<Odyssey::UICanvas>(), false);

	// Unregister callbacks for the buttons
	// Set the pause menu button callbacks
	//GameUIManager::getInstance().GetResumeButton()->unregisterCallback("onMouseClick");
	//GameUIManager::getInstance().GetOptionsButton()->unregisterCallback("onMouseClick");
	//GameUIManager::getInstance().GetMainMenuButton()->unregisterCallback("onMouseClick");

	// Set the current level back to 1
	mCurrentLevel = 1;
	
	// Turn off battle music
	RedAudioManager::Instance().Stop("BackgroundBattle");

	delete mCurrentBattle;
	mCurrentBattle = nullptr;
	mIsPaused = true;
	// Switch to main menu scene
	Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("MainMenu"));
}
