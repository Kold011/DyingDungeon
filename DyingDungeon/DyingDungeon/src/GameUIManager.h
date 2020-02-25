#pragma once
#include "Entity.h"
#include "Character.h"
#include "Scene.h"
#include "Rectangle2D.h"
#include "Text2D.h"
#include "Sprite2D.h"
#include "StatTracker.h"
#include "UIElement.h"

class GameUIManager
{
public:

	enum class CharacterType
	{
		Paladin, Mage
	};

	// This struct will contain all of the UI elements associated with the character HUD
	//struct CharacterHUD
	//{
	//	Odyssey::UICanvas* pCanvas;
	//	Odyssey::Sprite2D* pPortrait;
	//	Odyssey::Rectangle2D* pXpBar;
	//	Odyssey::Text2D* pCharacterName;
	//	Odyssey::Text2D* pLevelNumber;
	//	Odyssey::Text2D* pAttackNumber;
	//	Odyssey::Text2D* pDefenseNumber;
	//	Odyssey::Text2D* pSpeedNumber;
	//	Odyssey::Rectangle2D* pHealthBar;
	//	Odyssey::Text2D* pHealthNumber;
	//	Odyssey::Rectangle2D* pManaBar;
	//	Odyssey::Text2D* pManaNumber;
	//	Odyssey::Text2D* pTurnNumber;
	//
	//	// Skills
	//	Odyssey::Sprite2D* pSkill1;
	//	Odyssey::UICanvas* pSkill1Canvas;
	//	Odyssey::Sprite2D* pSkill2;
	//	Odyssey::UICanvas* pSkill2Canvas;
	//	Odyssey::Sprite2D* pSkill3;
	//	Odyssey::UICanvas* pSkill3Canvas;
	//	Odyssey::Sprite2D* pSkill4;
	//	Odyssey::UICanvas* pSkill4Canvas;
	//
	//	// Status Effects
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//	//Odyssey::Sprite2D* pAttackUp;
	//};

	// This struct will hold the elements needed in order to animate the health and mana bars
	struct AnimatingBar
	{
		Odyssey::Rectangle2D* pBar = nullptr;
		Odyssey::Text2D* pBarText = nullptr;
		float pMaxValue = 0.0f;
		float pCurrValue = 0.0f; // This will change in the update function
		float pNewValue = 0.0f;

		// Used for checking while updating
		bool pTookDamage;
	};

public: // Singleton pattern
		/**
		 *	Get the singleton instance of the game ui manager.
		 *	@param[in] void
		 *	@return InputManager& The singleton instance of the game ui manager.
		 */
	static GameUIManager& getInstance();
	~GameUIManager() { }

private: // Singleton pattern
	GameUIManager() 
	{ 
		mLoadoutPortraits[0][1] = nullptr;
		mLoadoutPortraits[0][2] = nullptr;
		mLoadoutPortraits[0][3] = nullptr;

		mLoadoutPortraits[1][1] = nullptr;
		mLoadoutPortraits[1][2] = nullptr;
		mLoadoutPortraits[1][3] = nullptr;

		mLoadoutPortraits[2][1] = nullptr;
		mLoadoutPortraits[2][2] = nullptr;
		mLoadoutPortraits[2][3] = nullptr;
	}

public: // Functions

	// Create the battle log UI 
	void CreateBattleLog(Odyssey::Scene* _sceneToAddTo);

	// Toggle canvases on and off
	void ToggleCanvas(Odyssey::UICanvas* _canvas, bool _isActive);

	// Creation of the main menu
	void CreateMainMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	// Creation of the tower select menu
	void CreateTowerSelectMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	// Creation of the team select menu
	void CreateTeamSelectMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	void ShowSaveLoadoutMenu();

	void HideLoadoutMenu();

	void SaveLoadout1();

	// Creation of the pause menu
	void CreatePauseMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	// Creation of the stats menu
	void CreateStatsMenuCanvas(Odyssey::Scene* _sceneToAddTo);

	// TODO: M3B1 ONLY REFACTOR LATER
	void CreateCreditsMenuCanvas(Odyssey::Scene* _sceneToAddTo);
	// TODO: M3B1 ONLY END

	void ShowStatsMenu();

	// TODO: M3B1 ONLY REFACTOR LATER
	void ShowCreditsMenu();
	void HideCreditsMenu();
	void ShowMainOptions();
	void HideMainOptions();
	// TODO: M3B1 ONLY END

	void StatsMenuPrevTurn();

	void StatsMenuNextTurn();

	void StatsMenuPrevLevel();

	void StatsMenuNextLevel();

	void StatsMenuNextRound();

	void StatsMenuPrevRound();

	void HideStatsMenu();

	// Set the characters HUD Elements
	void AssignCharacterHudElements(Character* _newCharacter, Odyssey::Entity* _newHud);
	 
	// Create hero character portrait
	void CreateHeroHud(Odyssey::Entity* _gameObjectToAddTo, DirectX::XMFLOAT2 _hudPosition);

	// Create enemy character portrait
	void CreateEnemyHud(Odyssey::Entity* _gameObjectToAddTo, DirectX::XMFLOAT2 _hudPosition);

	// Create the skill popups
	void CreateSkillHoverHud(Odyssey::Entity* _gameObjectToAddTo, DirectX::XMFLOAT2 _hudPosition);

	// Create Hp Popup
	Odyssey::UICanvas* CreateHpPopup(Odyssey::Entity* _objToAddTo);

	// Create The UI portraits for the characters
	//Odyssey::UICanvas* CreateCharacterPortrait(DirectX::XMFLOAT2 _hudPosition, DirectX::XMFLOAT2 _hpPopupPosition, std::wstring _imageName, Odyssey::Entity* _gameObjectToAddTo, Character* owner);
	
	// Character info popup for team selection screen
	Odyssey::UICanvas* SetupInfoPopup(Odyssey::Entity* _objToAddTo, Character* _character, DirectX::XMFLOAT2 _popupPosition);

	// Increase the char
	// Add HUD to the character hud list
	void AddHudToList(Odyssey::Entity* _newHud);
	// Add character health and mana bars to update list in order for the bars to be animated
	void AddCharacterHpBarsToUpdateList(Character* _currCharacter, float _previousHpAmount, float _newHpAmount);
	void AddCharacterMpBarsToUpdateList(Character* _currCharacter, float _previousMpAmount, float _newMpAmount);
	// Update health bar
	void UpdateCharacterBars(float _deltaTime);
	// Add character's health popup to update list in order from them to be updated
	void AddHpPopupToUpdateList(Character* _currCharacter, bool _tookDamage, float _changeInHP);
	// Update health popups
	void UpdateCharacterHealthPopups(float _deltaTime);
	// UPdate turn number
	void UpdateCharacterTurnNumber(Character* _currCharacter, int _turnNumber);

	//Updates
	void UpdateStatsMenu();

	// Update Combat Log
	void UpdateCombatLogIcons(Character* caster, Character* target, Skills* skill);
	void UpdateCombatLogText(float dmg);
	void ClearCombatLog();

	//Getters
	// Get battle log text
	Odyssey::Text2D* GetBattleLogText() { return mBattleLogText; }

	// Get Combat Log Canvas
	Odyssey::UICanvas* GetCombatLogCanvas() { return mCombatLogCanvas; }

	// Get the main menu
	Odyssey::Entity* GetMainMenu() { return mMainMenu; }
	Odyssey::Sprite2D* GetApeBackground() { return mApeBackground; }
	Odyssey::Sprite2D* GetAnimatedLaser() { return mAnimatedLaser; }
	Odyssey::Text2D* GetNewGameText() { return mNewGameText; }
	Odyssey::Text2D* GetStatsText() { return mStatsText; }
	// TODO: M3B1 ONLY REFACTOR LATER
	Odyssey::Text2D* GetCreditsText() { return mCreditsText; }
	Odyssey::Text2D* GetExitGameText() { return mExitGameText; }
	// TODO: M3B1 ONLY END

	// Get the tower select menu
	Odyssey::Entity* GetTowerSelectMenu() { return mTowerSelectMenu; }
	// Get the sprite buttons from the tower select menu
	std::vector<Odyssey::Sprite2D*> GetDoorSprites() { return mDoorImages; }
	// Get the tower info canvas
	Odyssey::UICanvas* GetTowerInfoCanvas() { return mTowerInfoCanvas; }

	// Get the team select menu
	Odyssey::Entity* GetTeamSelectMenu() { return mTeamSelectMenu; }
	// Get the arrow sprite when selecting a team
	std::vector<Odyssey::Sprite2D*> GetTeamSelectionArrows() { return mTeamSelectionArrows; }
	// Get the text slots for the names
	std::vector<Odyssey::Text2D*> GetNameSlots() { return mNameSlots; }
	// Get the show info button sprites
	std::vector<Odyssey::Sprite2D*> GetShowInfoButtons() { return mShowInfoButtons; }
	//
	Odyssey::Sprite2D** GetLoadoutButtons() { return mLoadoutButtons; }
	//
	Odyssey::Sprite2D* GetLoadLoadoutButton() { return mLoadLoadoutButton; }
	//
	Odyssey::Sprite2D* GetSaveLoadoutButton() { return mSaveLoadoutButton; }
	//
	Odyssey::Text2D* GetCancelLoadoutButton() { return mCancelLoadoutButton; }
	//
	Odyssey::Entity* GetLoadoutMenu() { return mLoadoutMenu; }
	//
	Odyssey::Sprite2D** GetSaveConfermationButtons() { return mSaveConfermationButtons; }
	//
	Odyssey::Entity* GetSaveConfermationMenu() { return mSaveLoadoutConfermation; }
	//
	Odyssey::Sprite2D* GetLoadoutPortraits(unsigned int index_1, unsigned int index_2) { return mLoadoutPortraits[index_1][index_2]; }
	//
	Odyssey::Rectangle2D** GetLoadoutPortraitBackgrounds() { return mLoadoutPortraitBackground; }
	//
	Odyssey::Text2D* GetBackTextOptions() { return mOptionsBack; }
	//
	Odyssey::Text2D* GetOptionsButtonMain() { return mMainOptionsText; }
	// Get the enter battle button sprite
	Odyssey::Sprite2D* GetEnterBattleButton() { return mEnterBattleButton; }

	Odyssey::Sprite2D** GetMainMenuPlusVolumeButtons() { return mMainPlusImage; }

	Odyssey::Sprite2D** GetMainMenuMinusVolumeButtons() { return mMainMinusImage; }

	// Get the pause menu
	Odyssey::Entity* GetPauseMenu() { return mPauseMenu; }
	// Get the options menu		   
	Odyssey::Entity* GetOptionsMenu() { return mOptionsMenu; }
	// Get the stats menu		   
	Odyssey::Entity* GetStatsMenu() { return mStatsMenu; }
	// Get the rectangle buttons from the pause menu
	Odyssey::Rectangle2D* GetResumeButton() { return mResumeBackground; }
	Odyssey::Rectangle2D* GetOptionsButton() { return mOptionsBackground; }
	Odyssey::Rectangle2D* GetMainMenuButton() { return mMainMenuBackground; }

	// Get the list of all of the character hud canvases
	std::vector<Odyssey::Entity*> GetCharacterHuds() { return mCharacterHudList; }

	//Setters
	void SetPauseMenu(Odyssey::Entity* _pauseMenu) { mPauseMenu = _pauseMenu; }

	void SetOptionsMenu(Odyssey::Entity* _optionsMenu) { mOptionsMenu = _optionsMenu; }

	void SetStatsMenu(Odyssey::Entity* _statsMenu) { mStatsMenu = _statsMenu; }

	void SetScreenWidthAndHeight(UINT _width, UINT _height) { screenWidth = _width; screenHeight = _height; }

	/*
	 * Set the new battle text
	 * @param[in] wstring newText to be added and bool concat wether or not to concat.
	 * @return void
	*/
	void SetBattleLogText(std::wstring text, bool concat);
	
private: // Varibales

	// Battle Log Object
	Odyssey::UICanvas* mCombatLogCanvas = nullptr;
	Odyssey::Text2D* mBattleLogText = nullptr;
	Odyssey::Entity* mBattleLog = nullptr;
	std::vector<Odyssey::Text2D*> mBattleLogVec;
	std::vector<Odyssey::Sprite2D*> mCombatCasterIcons;
	std::vector<Odyssey::Sprite2D*> mCombatSkillIcons;
	std::vector<Odyssey::Sprite2D*> mCombatTargetIcons;

	// Battle Log Colors
	DirectX::XMFLOAT3 newCombatLogColor;

	// Menu Entities
	Odyssey::Entity* mMainMenu = nullptr;
	Odyssey::Entity* mMainMenuOptions = nullptr;
	Odyssey::Entity* mTowerSelectMenu = nullptr;
	Odyssey::Entity* mTeamSelectMenu = nullptr;
	Odyssey::Entity* mLoadoutMenu = nullptr;
	Odyssey::Entity* mSaveLoadoutConfermation = nullptr;
	Odyssey::Entity* mPauseMenu = nullptr;
	Odyssey::Entity* mOptionsMenu = nullptr;
	Odyssey::Entity* mStatsMenu = nullptr;
	Odyssey::Entity* mCreditsMenu = nullptr;

	// Main Menu Items
	Odyssey::Sprite2D* mApeBackground = nullptr;
	Odyssey::Sprite2D* mAnimatedLaser = nullptr;
	Odyssey::Text2D* mNewGameText = nullptr;
	Odyssey::Text2D* mStatsText = nullptr;
	Odyssey::Text2D* mOptionsBack = nullptr;
	Odyssey::Sprite2D* mMainPlusImage[4] = { nullptr, nullptr, nullptr, nullptr };
	Odyssey::Sprite2D* mMainMinusImage[4] = { nullptr, nullptr, nullptr, nullptr };
	Odyssey::Rectangle2D* mMainVolumeBar[4] = { nullptr,nullptr,nullptr,nullptr };
	// TODO: M3B1 ONLY REFACTOR LATER
	Odyssey::Text2D* mCreditsText = nullptr;
	Odyssey::Text2D* mExitGameText = nullptr;
	Odyssey::Text2D* mCreditsBackText = nullptr;
	Odyssey::Text2D* mMainOptionsText = nullptr;
	// TODO: M3B1 ONLY END

	// Tower Menu Items
	Odyssey::Text2D* mTowerSelectTitle = nullptr;
	std::vector<Odyssey::Sprite2D*> mDoorImages;
	Odyssey::UICanvas* mTowerInfoCanvas = nullptr;

	// Team Menu Items
	// Enter Battle Button
	Odyssey::Sprite2D* mEnterBattleButton = nullptr;
	//Load Loadout Button
	Odyssey::Sprite2D* mLoadLoadoutButton = nullptr;
	//Save Loadout Button
	Odyssey::Sprite2D* mSaveLoadoutButton = nullptr;
	//Loadout Buttons
	Odyssey::Sprite2D* mLoadoutButtons[3];
	//Loadout pop-up
	Odyssey::Rectangle2D* mLoadoutPortraitBackground[3] = { nullptr, nullptr, nullptr };
	//Loadout Character Portraits
	Odyssey::Sprite2D* mLoadoutPortraits[3][3];
	//Save Confermation Buttons
	Odyssey::Sprite2D* mSaveConfermationButtons[2] = {nullptr, nullptr};
	//Loadout Cancel Button
	Odyssey::Text2D* mCancelLoadoutButton = nullptr;
	// The arrow sprites
	std::vector<Odyssey::Sprite2D*> mTeamSelectionArrows;
	// The name text slots
	std::vector<Odyssey::Text2D*> mNameSlots;
	// The show info buttons
	std::vector<Odyssey::Sprite2D*> mShowInfoButtons;

	// Pause Menu Items
	Odyssey::Rectangle2D* mBlackBackground = nullptr;
	Odyssey::Rectangle2D* mSmallerBlackBackground = nullptr;
	Odyssey::Text2D* mPauseTitle = nullptr;
	Odyssey::Rectangle2D* mResumeBackground = nullptr;
	Odyssey::Text2D* mResumeText = nullptr;
	Odyssey::Rectangle2D* mOptionsBackground = nullptr;
	Odyssey::Text2D* mOptionsText = nullptr;
	Odyssey::Rectangle2D* mMainMenuBackground = nullptr;
	Odyssey::Text2D* mMainMenuText = nullptr;

	// Options Menu Items
	Odyssey::Text2D* mOptionsTitle = nullptr;
	Odyssey::Text2D* mVolumeText = nullptr;
	Odyssey::Rectangle2D* mVolumeBar[4] = { nullptr,nullptr,nullptr,nullptr };
	Odyssey::Sprite2D* mPlusImage[4] = { nullptr, nullptr, nullptr, nullptr };
	Odyssey::Sprite2D* mMinusImage[4] = { nullptr, nullptr, nullptr, nullptr };
	Odyssey::Text2D* mBackButtonText = nullptr;

	//Stats Menu Items
	unsigned int mStatMenuCurrentLevel = 1;
	unsigned int mStatMenuCurrentRound = 1;
	unsigned int mStatMenuCurrentTurn = 1;
	Odyssey::Text2D* mStatsBackButtonText = nullptr;
	Odyssey::Sprite2D* mStatsPrevButtonTurn = nullptr;
	Odyssey::Sprite2D* mStatsNextButtonTurn = nullptr;
	Odyssey::Sprite2D* mStatsPrevButtonRound = nullptr;
	Odyssey::Sprite2D* mStatsNextButtonRound = nullptr;
	Odyssey::Sprite2D* mStatsPrevButtonLevel = nullptr;
	Odyssey::Sprite2D* mStatsNextButtonLevel = nullptr;

	// Colors
	DirectX::XMFLOAT4 mTextColor = { 255.0f, 255.0f, 255.0f, 1.0f };
	DirectX::XMFLOAT4 mHealthBarColor = { 0.0f, 180.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 mManaBarColor = { 0.0f, 180.0f, 180.0f, 1.0f };
	DirectX::XMFLOAT4 mTurnOrderColor = { 255.0f, 210.0f, 0.0f, 1.0f };

	// Vectors
	// List of the HUDs
	std::vector<Odyssey::Entity*> mCharacterHudList;
	std::vector<Odyssey::Text2D*> mCharacterHpPopupList;
	std::vector<Odyssey::Rectangle2D*> mCharacterBarsList;

	// List of HP popups I need to update
	std::vector<Odyssey::Text2D*> mUpdateHpPopupList;

	// List of character bars I need to update
	std::vector<std::shared_ptr<AnimatingBar>> mUpdateCharacterBarsList;

	// Queues

	// Entities

	// Ints
	UINT screenWidth = 0;
	UINT screenHeight = 0;
	UINT battleTextWidth = 0;
	UINT battleTextHeight = 0;

	// Floats

	// Bools

private: // Functions
	void CreateOptionsMenu(Odyssey::Scene* _sceneToAddTo);
	void OptionsBackButton();
	void DecreaseMasterVolume();
	void DecreaseBackgroundVolume();
	void DecreaseSFXVolume();
	void DecreaseDialogVolume();
	void IncreaseMasterVolume();

	void IncreaseBackgroundVolume();

	void IncreaseSFXVolume();

	void IncreaseDialogVolume();

	// Skill Icon Creation Fucntions
	void SetupSkillIcons(Odyssey::Entity* _objToAddTo, DirectX::XMFLOAT2 _hudPosition);
	//void SetupStatusEffects(Odyssey::Entity* _objToAddTo, Character* _newCharacter, DirectX::XMFLOAT2 _hudPosition, Odyssey::Entity* _newHud);

	// TODO: REFACTOR THIS LATER
	Odyssey::UICanvas* CreatePopup(Odyssey::Entity* entity);
};
