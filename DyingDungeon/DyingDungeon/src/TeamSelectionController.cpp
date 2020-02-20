#include "TeamSelectionController.h"
#include "InputManager.h"
#include "RedAudioManager.h"
#include "EventManager.h"
#include "GameUIManager.h"
#include "TeamManager.h"
#include "CharacterFactory.h"
#include "HeroComponent.h"
#include "Material.h"
#include "TowerManager.h"
#include "SaveLoad.h"

CLASS_DEFINITION(Odyssey::Component, TeamSelectionController)
TeamSelectionController::TeamSelectionController(Odyssey::Application* application)
{
	mApplication = application;
	mCurrentTower = nullptr;
}

std::shared_ptr<Odyssey::Component> TeamSelectionController::clone() const
{
	return std::make_shared<TeamSelectionController>(*this);
}

void TeamSelectionController::initialize()
{
	// Reset bools and ints
	changedTheScene = false;
	mEnterBattle = false;
	mBuildIndex = 0;

	// Create the models and info popups
	CreateModelsAndPopups();

	// Don't display some of the characters
	TurnOffOtherModels();

	// Register callbacks
	GameUIManager::getInstance().GetEnterBattleButton()->registerCallback("onMouseClick", this, &TeamSelectionController::EnterBattle);

	// Register callbacks for the arrows
	GameUIManager::getInstance().GetTeamSelectionArrows()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot1Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot1Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot2Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[3]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot2Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[4]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot3Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[5]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot3Index);

	// Register the show info button callbacks
	GameUIManager::getInstance().GetShowInfoButtons()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::ToggleShowInfoPopup1);
	GameUIManager::getInstance().GetShowInfoButtons()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::ToggleShowInfoPopup2);
	GameUIManager::getInstance().GetShowInfoButtons()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::ToggleShowInfoPopup3);

	//Register the save and load callbacks
	GameUIManager::getInstance().GetLoadLoadoutButton()->registerCallback("onMouseClick", this, &TeamSelectionController::ShowLoadLoadoutMenu);
	GameUIManager::getInstance().GetSaveLoadoutButton()->registerCallback("onMouseClick", this, &TeamSelectionController::ShowSaveLoadoutMenu);


	// Set the player positions
	mPlayerPositions.clear();
	mPlayerPositions.resize(3);
	mPlayerPositions[0] = DirectX::XMVectorSet(-5.0f, 0.0f, 10.0f, 1.0f); // First Character Selected
	mPlayerPositions[1] = DirectX::XMVectorSet(0.0f, 0.0f, 10.0f, 1.0f); // Second Character Selected
	mPlayerPositions[2] = DirectX::XMVectorSet(5.0f, 0.0f, 10.0f, 1.0f); // Third Character Selected

	// Clear the player team from Team Manager before adding in new characters
	TeamManager::getInstance().ClearPlayerTeamEnumList();

	if (SaveLoad::Instance().LoadLoadOut("Last_Loadout"))
	{
		mSlot1CharacterList[mSlot1Index]->setVisible(false);
		mSlot1Index = SaveLoad::Instance().GetLoadOut("Last_Loadout").index[0];
		mSlot1CharacterList[mSlot1Index]->setVisible(true);
		ChangeSlotName(0, mSlot1CharacterList[mSlot1Index]->getComponent<Character>()->GetName());
	
		mSlot2CharacterList[mSlot2Index]->setVisible(false);
		mSlot2Index = SaveLoad::Instance().GetLoadOut("Last_Loadout").index[1];
		mSlot2CharacterList[mSlot2Index]->setVisible(true);
		ChangeSlotName(1, mSlot2CharacterList[mSlot2Index]->getComponent<Character>()->GetName());
		
		mSlot3CharacterList[mSlot3Index]->setVisible(false);
		mSlot3Index = SaveLoad::Instance().GetLoadOut("Last_Loadout").index[2];
		mSlot3CharacterList[mSlot3Index]->setVisible(true);
		ChangeSlotName(2, mSlot3CharacterList[mSlot3Index]->getComponent<Character>()->GetName());
	}
	//SaveLoad::Instance().LoadLoadOut();
}

void TeamSelectionController::update(double deltaTime)
{
	if (mEnterBattle && !changedTheScene)
	{
		changedTheScene = true;
		RedAudioManager::Instance().StopGroup("BackgroundMenu");
		RedAudioManager::Instance().Loop("TorchBurningQuietly");
		RedAudioManager::Instance().SetVolume("TorchBurningQuietly", 600);
		RedAudioManager::Instance().Loop("BackgroundBattle");

		// Set up the tower manager with how many levels we want
		mCurrentTower->getComponent<TowerManager>()->SetUpTowerManager(TeamManager::getInstance().GetEnemiesToCreateList().size());

		// Change the scene to the game
		Odyssey::EventManager::getInstance().publish(new Odyssey::SceneChangeEvent("Scene One"));
	}
	else if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::M))
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
	else if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::A) && Odyssey::InputManager::getInstance().getKeyPress(KeyCode::S))
	{
		GameplayTypes::HEROID ids[3] = { mSlot1CharacterList[mSlot1Index]->getComponent<HeroComponent>()->GetID(), mSlot2CharacterList[mSlot2Index]->getComponent<HeroComponent>()->GetID(), mSlot3CharacterList[mSlot3Index]->getComponent<HeroComponent>()->GetID() };
		unsigned int indecies[3] = { (unsigned int)mSlot1Index, (unsigned int)mSlot2Index, (unsigned int)mSlot3Index };
		SaveLoad::Instance().AddLoadOut("Last_Loadout", ids, indecies);
		SaveLoad::Instance().SaveLoadOut();
	}
	else if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::T))
	{
		Odyssey::Entity* loadingScreenPrefab = mApplication->createPrefab();
		Odyssey::UICanvas* canvas = loadingScreenPrefab->addComponent<Odyssey::UICanvas>();

		// Loading Screen Code
		Odyssey::TextProperties titleProperties;
		titleProperties.fontName = L"Tw Cen MT Condensed";
		titleProperties.fontSize = 32;
		titleProperties.textAlignment = Odyssey::TextAlignment::Left;
		titleProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
		titleProperties.bold = true;

		Odyssey::TextProperties paragraphProperties;
		paragraphProperties.fontName = L"Tw Cen MT Condensed";
		paragraphProperties.fontSize = 18;
		paragraphProperties.textAlignment = Odyssey::TextAlignment::Left;
		paragraphProperties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
		paragraphProperties.bold = false;

		// Set the text to use
		std::wstring titleText = L"Renaud - The Zealous Knight";
		std::wstring backgroundFile = L"assets/images/FantasyBackground1.jpg";
		std::wstring spriteFile = L"assets/images/Renaud.png";
		std::wstring paragraphText = L"Renaud has always been a devout follower of the church. He grew up in the church, training his entire life to become a paladin. Renaud has found absolution within the church�s teachings and knows nothing beyond them. Ever since the day Ganfaul killed The Council and took control of Metis, Renaud has sought to overthrow him and return the church to its former glory.";
		// Stat Icons
		std::wstring healthIcon = L"assets/images/Meat.png";
		std::wstring healthText = L"Health: 100";
		std::wstring manaIcon = L"assets/images/mp.png";
		std::wstring manaText = L"Mana: 100";
		std::wstring attackIcon = L"assets/images/Sword.png";
		std::wstring attackText = L"Attack: 65";
		std::wstring defenseIcon = L"assets/images/Shield.png";
		std::wstring defenseText = L"Defense: 80";
		std::wstring speedIcon = L"assets/images/Speed.png";
		std::wstring speedText = L"Speed: 20";

		std::wstring skill1Text = L"Judgement";
		std::wstring skill1Icon = L"assets/images/Paladin_Skill_1.png";
		std::wstring skill2Text = L"Smite";
		std::wstring skill2Icon = L"assets/images/Paladin_Skill_2.png";
		std::wstring skill3Text = L"Shield of Light";
		std::wstring skill3Icon = L"assets/images/Paladin_Skill_3.png";
		std::wstring skill4Text = L"Blessing of Light";
		std::wstring skill4Icon = L"assets/images/Paladin_Skill_4.png";

		// Add the background image and the title/image
		//Odyssey::UICanvas* canvas = gSceneOneCamera->addComponent<Odyssey::UICanvas>();
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), backgroundFile, 1280, 720);
		canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(90.0f, 90.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.6f), 650, 400);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(100.0f, 100.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 500, 100, titleText, titleProperties);
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(850.0f, 125.0f), spriteFile, 350, 500);

		// Add the summary and background
		paragraphProperties.italic = true;
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(115.0f, 150.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 610, 500, paragraphText, paragraphProperties);

		// Stats line - Health
		paragraphProperties.italic = false;
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(220.0f, 290.0f), healthIcon, 30, 30);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(265.0f, 295.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f), 100, 100, healthText, paragraphProperties);

		// Stats line - Mana
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(380.0f, 290.0f), manaIcon, 30, 30);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(420.0f, 295.0f), DirectX::XMFLOAT4(0.0f, 255.0f, 255.0f, 1.0f), 100, 100, manaText, paragraphProperties);

		// Stats line - Attack
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(170.0f, 330.0f), attackIcon, 30, 30);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(205.0f, 330.0f), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 1.0f), 100, 100, attackText, paragraphProperties);

		// Stats line - Defense
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(300.0f, 330.0f), defenseIcon, 25, 30);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(335.0f, 330.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), 100, 100, defenseText, paragraphProperties);

		// Stats line - Speed
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(440.0f, 330.0f), speedIcon, 30, 30);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(475.0f, 330.0f), DirectX::XMFLOAT4(255.0f, 255.0f, 0.0f, 1.0f), 100, 100, speedText, paragraphProperties);

		// Skill 1
		paragraphProperties.fontSize = 24;
		//gSceneOneCamera->getComponent<CameraController>()->mRect = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(108.0f, 378.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 0.5f), 44, 44);
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(110.0f, 380.0f), skill1Icon, 40, 40);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(170.0f, 382.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill1Text, paragraphProperties);

		// Skill 2
		//gSceneOneCamera->getComponent<CameraController>()->mRect = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(408.0f, 378.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 0.5f), 44, 44);
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(410.0f, 380.0f), skill2Icon, 40, 40);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(470.0f, 382.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill2Text, paragraphProperties);

		// Skill 3
		//gSceneOneCamera->getComponent<CameraController>()->mRect = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(108.0f, 428.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 0.5f), 44, 44);
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(110.0f, 430.0f), skill3Icon, 40, 40);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(170.0f, 432.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill3Text, paragraphProperties);

		// Skill 4
		//gSceneOneCamera->getComponent<CameraController>()->mRect = canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(408.0f, 428.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 0.5f), 44, 44);
		canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(410.0f, 430.0f), skill4Icon, 40, 40);
		canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(470.0f, 432.0f), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 400, 100, skill4Text, paragraphProperties);
		
		// Show the screen
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(loadingScreenPrefab, &loadingScreen, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }, DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f }));
	}
	else if (Odyssey::InputManager::getInstance().getKeyUp(KeyCode::T))
	{
		Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(loadingScreen));
	}
}

void TeamSelectionController::onDestroy()
{
	// Unregister callbacks
	GameUIManager::getInstance().GetEnterBattleButton()->unregisterCallback("onMouseClick");

	// Unregister callbacks for the arrows
	GameUIManager::getInstance().GetTeamSelectionArrows()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[2]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[3]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[4]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[5]->unregisterCallback("onMouseClick");

	// Unregister the show info button callbacks
	GameUIManager::getInstance().GetShowInfoButtons()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetShowInfoButtons()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetShowInfoButtons()[2]->unregisterCallback("onMouseClick");

	GameUIManager::getInstance().GetLoadoutButtons()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetLoadoutButtons()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetLoadoutButtons()[2]->unregisterCallback("onMouseClick");

	GameUIManager::getInstance().GetLoadoutButtons()[0]->unregisterCallback("onMouseEnter");
	GameUIManager::getInstance().GetLoadoutButtons()[1]->unregisterCallback("onMouseEnter");
	GameUIManager::getInstance().GetLoadoutButtons()[2]->unregisterCallback("onMouseEnter");

	GameUIManager::getInstance().GetLoadoutButtons()[0]->unregisterCallback("onMouseExit");
	GameUIManager::getInstance().GetLoadoutButtons()[1]->unregisterCallback("onMouseExit");
	GameUIManager::getInstance().GetLoadoutButtons()[2]->unregisterCallback("onMouseExit");

	GameUIManager::getInstance().GetLoadLoadoutButton()->unregisterCallback("onMouseClick");

	GameUIManager::getInstance().GetSaveConfermationButtons()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetSaveConfermationButtons()[1]->unregisterCallback("onMouseClick");

	GameUIManager::getInstance().GetCancelLoadoutButton()->unregisterCallback("onMouseClick");
}

void TeamSelectionController::CreateModelsAndPopups()
{
	// Set up variables
	float xOffset = -5.0f;
	float yHeight = -2.0f;
	float zDepth = 8.0f;
	Odyssey::Entity* character = nullptr;
	Odyssey::Entity* infoPopup = mEntity->getScene()->createEntity();
	Odyssey::Entity* prefab = nullptr;
	Odyssey::UICanvas* infoPopupCanvas = nullptr;
	DirectX::XMVECTOR position = DirectX::XMVectorSet(xOffset, yHeight, zDepth, 1.0f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 140.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT2 uiPosition = { 120.0f, 145.0f };

	// Make Slot 1 Characters
	bool makeSlot1Characters = true;
	if (makeSlot1Characters)
	{
		// Paladin
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Paladin);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Mage
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Mage);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Bard
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Bard);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Warrior
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Warrior);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Monk
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Monk);
		rotation = DirectX::XMVectorSet(0.0f, 320.0f, 0.0f, 1.0f);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		rotation = DirectX::XMVectorSet(0.0f, 140.0f, 0.0f, 1.0f);
		mSlot1CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot1CharacterInfoPopupList.push_back(infoPopupCanvas);
	}

	// Update variables for the slot 2 characters
	xOffset = 0.0f;
	position = DirectX::XMVectorSet(xOffset, yHeight, zDepth, 1.0f);
	rotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
	uiPosition = { 490.0f, 145.0f };

	// Make Slot 2 Characters
	bool makeSlot2Characters = true;
	if (makeSlot2Characters)
	{
		// Mage
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Mage);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Bard
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Bard);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Warrior
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Warrior);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Monk
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Monk);
		rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		rotation = DirectX::XMVectorSet(0.0f, 180.0f, 0.0f, 1.0f);
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Paladin
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Paladin);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot2CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot2CharacterInfoPopupList.push_back(infoPopupCanvas);
	}

	// Update variables for the slot 3 characters
	xOffset = 5.0f;
	position = DirectX::XMVectorSet(xOffset, yHeight, zDepth, 1.0f);
	rotation = DirectX::XMVectorSet(0.0f, 220.0f, 0.0f, 1.0f);
	uiPosition = { 860.0f, 145.0f };

	// Make Slot 3 Characters
	bool makeSlot3Characters = true;
	if (makeSlot3Characters)
	{
		// Bard
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Bard);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Warrior
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Warrior);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Monk
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Monk); 
		rotation = DirectX::XMVectorSet(0.0f, 40.0f, 0.0f, 1.0f);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		rotation = DirectX::XMVectorSet(0.0f, 220.0f, 0.0f, 1.0f);
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Paladin
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Paladin);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
		// Mage
		prefab = CharacterFactory::getInstance().GetCharacterPrefab(CharacterFactory::CharacterOptions::Mage);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &character, position, rotation));
		mSlot3CharacterList.push_back(character);
		infoPopupCanvas = GameUIManager::getInstance().SetupInfoPopup(infoPopup, character->getComponent<Character>(), uiPosition);
		mSlot3CharacterInfoPopupList.push_back(infoPopupCanvas);
	}
}

void TeamSelectionController::TurnOffOtherModels()
{
	// Turn off every character except the first character in the list
	for (int i = 1; i < mSlot1CharacterList.size(); i++)
	{
		mSlot1CharacterList[i]->setVisible(false);
	}

	// Turn off every character except the first character in the list
	for (int i = 1; i < mSlot2CharacterList.size(); i++)
	{
		mSlot2CharacterList[i]->setVisible(false);
	}
	
	// Turn off every character except the first character in the list
	for (int i = 1; i < mSlot3CharacterList.size(); i++)
	{
		mSlot3CharacterList[i]->setVisible(false);
	}
}

void TeamSelectionController::EnterBattle()
{
	/*RedAudioManager::Instance().StopGroup("BackgroundMenu");
	RedAudioManager::Instance().Loop("TorchBurningQuietly");
	RedAudioManager::Instance().SetVolume("TorchBurningQuietly", 200);
	RedAudioManager::Instance().Loop("BackgroundBattle");*/

	GameplayTypes::HEROID ids[3] = { mSlot1CharacterList[mSlot1Index]->getComponent<HeroComponent>()->GetID(), mSlot2CharacterList[mSlot2Index]->getComponent<HeroComponent>()->GetID(), mSlot3CharacterList[mSlot3Index]->getComponent<HeroComponent>()->GetID() };
	unsigned int indecies[3] = { (unsigned int)mSlot1Index, (unsigned int)mSlot2Index, (unsigned int)mSlot3Index };
	SaveLoad::Instance().AddLoadOut("Last_Loadout", ids, indecies);
	SaveLoad::Instance().SaveLoadOut();
  
	// Check what characters are shown on the screen for the slot 1
	for (int i = 0; i < mSlot1CharacterList.size(); i++)
	{
		// Check if the character is visible or not
		if (mSlot1CharacterList[i]->isVisible())
		{
			// Add the character to the player enum list
			AddCharacterTypeToPlayerTeam(mSlot1CharacterList[i]->getComponent<Character>()->GetName());

			// Turn off the info popup before entering battle
			mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(false);
		}
	}

	// Check what characters are shown on the screen for the slot 1
	for (int i = 0; i < mSlot2CharacterList.size(); i++)
	{
		// Check if the character is visible or not
		if (mSlot2CharacterList[i]->isVisible())
		{
			// Add the character to the player enum list
			AddCharacterTypeToPlayerTeam(mSlot2CharacterList[i]->getComponent<Character>()->GetName());

			// Turn off the info popup before entering battle
			mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(false);
		}
	}

	// Check what characters are shown on the screen for the slot 1
	for (int i = 0; i < mSlot3CharacterList.size(); i++)
	{
		// Check if the character is visible or not
		if (mSlot3CharacterList[i]->isVisible())
		{
			// Add the character to the player enum list
			AddCharacterTypeToPlayerTeam(mSlot3CharacterList[i]->getComponent<Character>()->GetName());

			// Turn off the info popup before entering battle
			mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(false);
		}
	}

	// Allow the user to enter the battle scene after selecting their characters
	mEnterBattle = true;
}

void TeamSelectionController::AddCharacterTypeToPlayerTeam(std::wstring _characterName)
{
	// Define the new hero type
	TeamManager::HeroType newHeroType = TeamManager::HeroType::Paladin;

	// Set the enum based on the name of the character
	if (_characterName == L"Paladin")
		newHeroType = TeamManager::HeroType::Paladin;
	else if (_characterName == L"Mage")
		newHeroType = TeamManager::HeroType::Mage;
	else if (_characterName == L"Bard")
		newHeroType = TeamManager::HeroType::Bard;
	else if (_characterName == L"Warrior")
		newHeroType = TeamManager::HeroType::Warrior;
	else if (_characterName == L"Monk")
		newHeroType = TeamManager::HeroType::Monk;
	else
		std::cout << "Not the correct name so we defaulted to Paladin in TeamSelectionController.cpp";

	// Add the enum to the players to create list
	TeamManager::getInstance().AddCharacterEnumToPlayerTeam(newHeroType);

	// Increase the build index fo rthe next player when adding him to the list
	mBuildIndex++;
}

// Change the slot 1 character
void TeamSelectionController::DecreaseSlot1Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot1CharacterInfoPopupList[mSlot1Index]->isActive();
	// Set the old info popup to false
	mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(false);

	// Decrease the slot index
	mSlot1Index--;

	// If this go negative, set it to the size of a list - 1
	if (mSlot1Index < 0)
		mSlot1Index = (int)mSlot1CharacterList.size() - 1;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(0, mSlot1CharacterList[mSlot1Index]->getComponent<Character>()->GetName());
}

// Change the slot 1 character
void TeamSelectionController::IncreaseSlot1Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot1CharacterInfoPopupList[mSlot1Index]->isActive();
	// Set the old info popup to false
	mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(false);

	// Increase the slot index
	mSlot1Index++;

	// If this go above size of a list - 1, set it 0
	if (mSlot1Index > mSlot1CharacterList.size() - 1)
		mSlot1Index = 0;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot1CharacterInfoPopupList[mSlot1Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot1CharacterList[mSlot1Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(0, mSlot1CharacterList[mSlot1Index]->getComponent<Character>()->GetName());
}

// Change the slot 2 character
void TeamSelectionController::DecreaseSlot2Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot2CharacterInfoPopupList[mSlot2Index]->isActive();
	// Set the old info popup to false
	mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(false);

	// Decrease the slot index
	mSlot2Index--;

	// If this go negative, set it to the size of a list - 1
	if (mSlot2Index < 0)
		mSlot2Index = (int)mSlot2CharacterList.size() - 1;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(1, mSlot2CharacterList[mSlot2Index]->getComponent<Character>()->GetName());
}

// Change the slot 2 character
void TeamSelectionController::IncreaseSlot2Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot2CharacterInfoPopupList[mSlot2Index]->isActive();
	// Set the old info popup to false
	mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(false);

	// Increase the slot index
	mSlot2Index++;

	// If this go above size of a list - 1, set it 0
	if (mSlot2Index > mSlot2CharacterList.size() - 1)
		mSlot2Index = 0;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot2CharacterInfoPopupList[mSlot2Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot2CharacterList[mSlot2Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(1, mSlot2CharacterList[mSlot2Index]->getComponent<Character>()->GetName());
}

// Change the slot 3 character
void TeamSelectionController::DecreaseSlot3Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot3CharacterInfoPopupList[mSlot3Index]->isActive();
	// Set the old info popup to false
	mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(false);

	// Decrease the slot index
	mSlot3Index--;

	// If this go negative, set it to the size of a list - 1
	if (mSlot3Index < 0)
		mSlot3Index = (int)mSlot3CharacterList.size() - 1;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(2, mSlot3CharacterList[mSlot3Index]->getComponent<Character>()->GetName());
}

// Change the slot 3 character
void TeamSelectionController::IncreaseSlot3Index()
{
	// Check if the popup is currently on or not
	bool isPopupOn = mSlot3CharacterInfoPopupList[mSlot3Index]->isActive();
	// Set the old info popup to false
	mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(false);
	// Disable the current character that is visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(false);

	// Increase the slot index
	mSlot3Index++;

	// If this go above size of a list - 1, set it 0
	if (mSlot3Index > mSlot3CharacterList.size() - 1)
		mSlot3Index = 0;

	// Check if the previous popup was on or not
	if (isPopupOn)
		// Set the new info popup to true
		mSlot3CharacterInfoPopupList[mSlot3Index]->setActive(true);
	// Enable the new current character that will need to be visible in scene
	mSlot3CharacterList[mSlot3Index]->setVisible(true);

	// Change the slot name for the character that is now on the screen
	ChangeSlotName(2, mSlot3CharacterList[mSlot3Index]->getComponent<Character>()->GetName());
}

// Toggle slot 1 stat popup
void TeamSelectionController::ToggleShowInfoPopup1()
{
	// Find which character is visiable
	for (int i = 0; i < mSlot1CharacterList.size(); i++)
	{
		// Check if the character is visible
		if (mSlot1CharacterList[i]->isVisible())
		{
			mSlot1CharacterInfoPopupList[i]->setActive(!mSlot1CharacterInfoPopupList[i]->isActive());
			break;
		}
	}
}

// Toggle slot 2 stat popup
void TeamSelectionController::ToggleShowInfoPopup2()
{
	// Find which character is visiable
	for (int i = 0; i < mSlot2CharacterList.size(); i++)
	{
		// Check if the character is visible
		if (mSlot2CharacterList[i]->isVisible())
		{
			mSlot2CharacterInfoPopupList[i]->setActive(!mSlot2CharacterInfoPopupList[i]->isActive());
			break;
		}
	}
}

// Toggle slot 2 stat popup
void TeamSelectionController::ToggleShowInfoPopup3()
{
	// Find which character is visiable
	for (int i = 0; i < mSlot3CharacterList.size(); i++)
	{
		// Check if the character is visible
		if (mSlot3CharacterList[i]->isVisible())
		{
			mSlot3CharacterInfoPopupList[i]->setActive(!mSlot3CharacterInfoPopupList[i]->isActive());
			break;
		}
	}
}

// Change the name of the new character displaying
void TeamSelectionController::ChangeSlotName(int _slotIndex, std::wstring _newName)
{
	GameUIManager::getInstance().GetNameSlots()[_slotIndex]->setText(_newName);
}

void TeamSelectionController::onHoverLoadout1()
{
	GameUIManager::getInstance().GetLoadoutPortraitBackgrounds()[0]->setVisible(true);
	/*for (int i = 0; i < 3; i++)
	{*/
		if (SaveLoad::Instance().LoadLoadOut("Loadout_1"))
		{
			GameUIManager::getInstance().GetLoadoutPortraits(0, 0)->setSprite(mSlot1CharacterList[SaveLoad::Instance().GetLoadOut("Loadout_1").index[0]]->getComponent<Character>()->GetPortraitPath(), GameUIManager::getInstance().GetLoadoutPortraits(0, 0)->getDimensions().x, GameUIManager::getInstance().GetLoadoutPortraits(0, 0)->getDimensions().y);
			GameUIManager::getInstance().GetLoadoutPortraits(0, 1)->setSprite(mSlot2CharacterList[SaveLoad::Instance().GetLoadOut("Loadout_1").index[1]]->getComponent<Character>()->GetPortraitPath(), GameUIManager::getInstance().GetLoadoutPortraits(0, 1)->getDimensions().x, GameUIManager::getInstance().GetLoadoutPortraits(0, 1)->getDimensions().y);
			GameUIManager::getInstance().GetLoadoutPortraits(0, 2)->setSprite(mSlot3CharacterList[SaveLoad::Instance().GetLoadOut("Loadout_1").index[2]]->getComponent<Character>()->GetPortraitPath(), GameUIManager::getInstance().GetLoadoutPortraits(0, 2)->getDimensions().x, GameUIManager::getInstance().GetLoadoutPortraits(0, 2)->getDimensions().y);
			GameUIManager::getInstance().GetLoadoutPortraits(0, 0)->setVisible(true);
			GameUIManager::getInstance().GetLoadoutPortraits(0, 1)->setVisible(true);
			GameUIManager::getInstance().GetLoadoutPortraits(0, 2)->setVisible(true);
		}
	//}
}

void TeamSelectionController::onExitHoverLoadout1()
{
	GameUIManager::getInstance().GetLoadoutPortraitBackgrounds()[0]->setVisible(false);
	for (int i = 0; i < 3; i++)
	{
		GameUIManager::getInstance().GetLoadoutPortraits(0, i)->setVisible(false);
	}
}

void TeamSelectionController::onHoverLoadout2()
{
	GameUIManager::getInstance().GetLoadoutPortraitBackgrounds()[1]->setVisible(true);
	/*for (int i = 0; i < 3; i++)
	{*/
		if (SaveLoad::Instance().LoadLoadOut("Loadout_2"))
		{
			GameUIManager::getInstance().GetLoadoutPortraits(1, 0)->setSprite(mSlot1CharacterList[SaveLoad::Instance().GetLoadOut("Loadout_2").index[0]]->getComponent<Character>()->GetPortraitPath(), GameUIManager::getInstance().GetLoadoutPortraits(1, 0)->getDimensions().x, GameUIManager::getInstance().GetLoadoutPortraits(1, 0)->getDimensions().y);
			GameUIManager::getInstance().GetLoadoutPortraits(1, 1)->setSprite(mSlot2CharacterList[SaveLoad::Instance().GetLoadOut("Loadout_2").index[1]]->getComponent<Character>()->GetPortraitPath(), GameUIManager::getInstance().GetLoadoutPortraits(1, 1)->getDimensions().x, GameUIManager::getInstance().GetLoadoutPortraits(1, 1)->getDimensions().y);
			GameUIManager::getInstance().GetLoadoutPortraits(1, 2)->setSprite(mSlot3CharacterList[SaveLoad::Instance().GetLoadOut("Loadout_2").index[2]]->getComponent<Character>()->GetPortraitPath(), GameUIManager::getInstance().GetLoadoutPortraits(1, 2)->getDimensions().x, GameUIManager::getInstance().GetLoadoutPortraits(1, 2)->getDimensions().y);
			GameUIManager::getInstance().GetLoadoutPortraits(1, 0)->setVisible(true);
			GameUIManager::getInstance().GetLoadoutPortraits(1, 1)->setVisible(true);
			GameUIManager::getInstance().GetLoadoutPortraits(1, 2)->setVisible(true);
		}
	//}
}

void TeamSelectionController::onExitHoverLoadout2()
{
	GameUIManager::getInstance().GetLoadoutPortraitBackgrounds()[1]->setVisible(false);
	for (int i = 0; i < 3; i++)
	{
		GameUIManager::getInstance().GetLoadoutPortraits(1, i)->setVisible(false);
	}
}

void TeamSelectionController::onHoverLoadout3()
{
	GameUIManager::getInstance().GetLoadoutPortraitBackgrounds()[2]->setVisible(true);
	/*for (int i = 0; i < 3; i++)
	{*/
		if (SaveLoad::Instance().LoadLoadOut("Loadout_3"))
		{
			GameUIManager::getInstance().GetLoadoutPortraits(2, 0)->setSprite(mSlot1CharacterList[SaveLoad::Instance().GetLoadOut("Loadout_3").index[0]]->getComponent<Character>()->GetPortraitPath(), GameUIManager::getInstance().GetLoadoutPortraits(2, 0)->getDimensions().x, GameUIManager::getInstance().GetLoadoutPortraits(2, 0)->getDimensions().y);
			GameUIManager::getInstance().GetLoadoutPortraits(2, 1)->setSprite(mSlot2CharacterList[SaveLoad::Instance().GetLoadOut("Loadout_3").index[1]]->getComponent<Character>()->GetPortraitPath(), GameUIManager::getInstance().GetLoadoutPortraits(2, 1)->getDimensions().x, GameUIManager::getInstance().GetLoadoutPortraits(2, 1)->getDimensions().y);
			GameUIManager::getInstance().GetLoadoutPortraits(2, 2)->setSprite(mSlot3CharacterList[SaveLoad::Instance().GetLoadOut("Loadout_3").index[2]]->getComponent<Character>()->GetPortraitPath(), GameUIManager::getInstance().GetLoadoutPortraits(2, 2)->getDimensions().x, GameUIManager::getInstance().GetLoadoutPortraits(2, 2)->getDimensions().y);
			GameUIManager::getInstance().GetLoadoutPortraits(2, 0)->setVisible(true);
			GameUIManager::getInstance().GetLoadoutPortraits(2, 1)->setVisible(true);
			GameUIManager::getInstance().GetLoadoutPortraits(2, 2)->setVisible(true);
		}
	//}
}

void TeamSelectionController::onExitHoverLoadout3()
{
	GameUIManager::getInstance().GetLoadoutPortraitBackgrounds()[2]->setVisible(false);
	for (int i = 0; i < 3; i++)
	{
		GameUIManager::getInstance().GetLoadoutPortraits(2, i)->setVisible(false);
	}
}

void TeamSelectionController::ShowSaveLoadoutMenu()
{
	GameUIManager::getInstance().GetLoadoutButtons()[0]->setSprite(L"assets/images/TeamSelectionImages/SmallBoard.png", GameUIManager::getInstance().GetLoadoutButtons()[0]->getDimensions().x, GameUIManager::getInstance().GetLoadoutButtons()[0]->getDimensions().y);
	GameUIManager::getInstance().GetLoadoutButtons()[1]->setSprite(L"assets/images/TeamSelectionImages/SmallBoard.png", GameUIManager::getInstance().GetLoadoutButtons()[1]->getDimensions().x, GameUIManager::getInstance().GetLoadoutButtons()[1]->getDimensions().y);
	GameUIManager::getInstance().GetLoadoutButtons()[2]->setSprite(L"assets/images/TeamSelectionImages/SmallBoard.png", GameUIManager::getInstance().GetLoadoutButtons()[2]->getDimensions().x, GameUIManager::getInstance().GetLoadoutButtons()[2]->getDimensions().y);

	GameUIManager::getInstance().GetLoadoutMenu()->setActive(true);

	// Register callbacks
	GameUIManager::getInstance().GetEnterBattleButton()->unregisterCallback("onMouseClick");

	// Register callbacks for the arrows
	GameUIManager::getInstance().GetTeamSelectionArrows()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[2]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[3]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[4]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[5]->unregisterCallback("onMouseClick");

	// Register the show info button callbacks
	GameUIManager::getInstance().GetShowInfoButtons()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetShowInfoButtons()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetShowInfoButtons()[2]->unregisterCallback("onMouseClick");

	//Register the save and load callbacks
	GameUIManager::getInstance().GetLoadLoadoutButton()->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetSaveLoadoutButton()->unregisterCallback("onMouseClick");

	if (SaveLoad::Instance().LoadLoadOut("Loadout_1")) 
	{
		GameUIManager::getInstance().GetLoadoutButtons()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::ConfermationSave1);
		GameUIManager::getInstance().GetLoadoutButtons()[0]->registerCallback("onMouseEnter", this, &TeamSelectionController::onHoverLoadout1);
		GameUIManager::getInstance().GetLoadoutButtons()[0]->registerCallback("onMouseExit", this, &TeamSelectionController::onExitHoverLoadout1);
	}
	else
	{
		GameUIManager::getInstance().GetLoadoutButtons()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::SaveLoadout1);
	}

	if (SaveLoad::Instance().LoadLoadOut("Loadout_2"))
	{
		GameUIManager::getInstance().GetLoadoutButtons()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::ConfermationSave2);
		GameUIManager::getInstance().GetLoadoutButtons()[1]->registerCallback("onMouseEnter", this, &TeamSelectionController::onHoverLoadout2);
		GameUIManager::getInstance().GetLoadoutButtons()[1]->registerCallback("onMouseExit", this, &TeamSelectionController::onExitHoverLoadout2);
	}
	else
	{
		GameUIManager::getInstance().GetLoadoutButtons()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::SaveLoadout2);
	}

	if (SaveLoad::Instance().LoadLoadOut("Loadout_3"))
	{
		GameUIManager::getInstance().GetLoadoutButtons()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::ConfermationSave3);
		GameUIManager::getInstance().GetLoadoutButtons()[2]->registerCallback("onMouseEnter", this, &TeamSelectionController::onHoverLoadout3);
		GameUIManager::getInstance().GetLoadoutButtons()[2]->registerCallback("onMouseExit", this, &TeamSelectionController::onExitHoverLoadout3);
	}
	else
	{
		GameUIManager::getInstance().GetLoadoutButtons()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::SaveLoadout3);
	}

	GameUIManager::getInstance().GetCancelLoadoutButton()->registerCallback("onMouseClick", this, &TeamSelectionController::HideLoadLoadoutMenu);
}

void TeamSelectionController::ShowLoadLoadoutMenu()
{
	GameUIManager::getInstance().GetLoadoutMenu()->setActive(true);

	// Register callbacks
	GameUIManager::getInstance().GetEnterBattleButton()->unregisterCallback("onMouseClick");

	// Register callbacks for the arrows
	GameUIManager::getInstance().GetTeamSelectionArrows()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[2]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[3]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[4]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetTeamSelectionArrows()[5]->unregisterCallback("onMouseClick");

	// Register the show info button callbacks
	GameUIManager::getInstance().GetShowInfoButtons()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetShowInfoButtons()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetShowInfoButtons()[2]->unregisterCallback("onMouseClick");

	//Register the save and load callbacks
	GameUIManager::getInstance().GetLoadLoadoutButton()->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetSaveLoadoutButton()->unregisterCallback("onMouseClick");

	if (SaveLoad::Instance().LoadLoadOut("Loadout_1")) {
		GameUIManager::getInstance().GetLoadoutButtons()[0]->setSprite(L"assets/images/TeamSelectionImages/SmallBoard.png", GameUIManager::getInstance().GetLoadoutButtons()[0]->getDimensions().x, GameUIManager::getInstance().GetLoadoutButtons()[0]->getDimensions().y);
		GameUIManager::getInstance().GetLoadoutButtons()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::LoadLoadout1);
		GameUIManager::getInstance().GetLoadoutButtons()[0]->registerCallback("onMouseEnter", this, &TeamSelectionController::onHoverLoadout1);
		GameUIManager::getInstance().GetLoadoutButtons()[0]->registerCallback("onMouseExit", this, &TeamSelectionController::onExitHoverLoadout1);
	}
	else
	{
		GameUIManager::getInstance().GetLoadoutButtons()[0]->setSprite(L"assets/images/TeamSelectionImages/SmallBoardInactive.png", GameUIManager::getInstance().GetLoadoutButtons()[0]->getDimensions().x, GameUIManager::getInstance().GetLoadoutButtons()[0]->getDimensions().y);
	}
	if (SaveLoad::Instance().LoadLoadOut("Loadout_2")) {
		GameUIManager::getInstance().GetLoadoutButtons()[1]->setSprite(L"assets/images/TeamSelectionImages/SmallBoard.png", GameUIManager::getInstance().GetLoadoutButtons()[1]->getDimensions().x, GameUIManager::getInstance().GetLoadoutButtons()[1]->getDimensions().y);
		GameUIManager::getInstance().GetLoadoutButtons()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::LoadLoadout2);
		GameUIManager::getInstance().GetLoadoutButtons()[1]->registerCallback("onMouseEnter", this, &TeamSelectionController::onHoverLoadout2);
		GameUIManager::getInstance().GetLoadoutButtons()[1]->registerCallback("onMouseExit", this, &TeamSelectionController::onExitHoverLoadout2);
	}
	else
	{
		GameUIManager::getInstance().GetLoadoutButtons()[1]->setSprite(L"assets/images/TeamSelectionImages/SmallBoardInactive.png", GameUIManager::getInstance().GetLoadoutButtons()[1]->getDimensions().x, GameUIManager::getInstance().GetLoadoutButtons()[1]->getDimensions().y);
	}
	if (SaveLoad::Instance().LoadLoadOut("Loadout_3")) {
		GameUIManager::getInstance().GetLoadoutButtons()[2]->setSprite(L"assets/images/TeamSelectionImages/SmallBoard.png", GameUIManager::getInstance().GetLoadoutButtons()[2]->getDimensions().x, GameUIManager::getInstance().GetLoadoutButtons()[2]->getDimensions().y);
		GameUIManager::getInstance().GetLoadoutButtons()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::LoadLoadout3);
		GameUIManager::getInstance().GetLoadoutButtons()[2]->registerCallback("onMouseEnter", this, &TeamSelectionController::onHoverLoadout3);
		GameUIManager::getInstance().GetLoadoutButtons()[2]->registerCallback("onMouseExit", this, &TeamSelectionController::onExitHoverLoadout3);
	}
	else
	{
		GameUIManager::getInstance().GetLoadoutButtons()[2]->setSprite(L"assets/images/TeamSelectionImages/SmallBoardInactive.png", GameUIManager::getInstance().GetLoadoutButtons()[2]->getDimensions().x, GameUIManager::getInstance().GetLoadoutButtons()[2]->getDimensions().y);
	}
	GameUIManager::getInstance().GetCancelLoadoutButton()->registerCallback("onMouseClick", this, &TeamSelectionController::HideLoadLoadoutMenu);

}

void TeamSelectionController::HideLoadLoadoutMenu()
{
	GameUIManager::getInstance().GetLoadoutMenu()->setActive(false);
	GameUIManager::getInstance().GetLoadoutButtons()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetLoadoutButtons()[1]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetLoadoutButtons()[2]->unregisterCallback("onMouseClick");

	GameUIManager::getInstance().GetLoadoutButtons()[0]->unregisterCallback("onMouseEnter");
	GameUIManager::getInstance().GetLoadoutButtons()[1]->unregisterCallback("onMouseEnter");
	GameUIManager::getInstance().GetLoadoutButtons()[2]->unregisterCallback("onMouseEnter");

	GameUIManager::getInstance().GetLoadoutButtons()[0]->unregisterCallback("onMouseExit");
	GameUIManager::getInstance().GetLoadoutButtons()[1]->unregisterCallback("onMouseExit");
	GameUIManager::getInstance().GetLoadoutButtons()[2]->unregisterCallback("onMouseExit");

	// Register callbacks
	GameUIManager::getInstance().GetEnterBattleButton()->registerCallback("onMouseClick", this, &TeamSelectionController::EnterBattle);

	// Register callbacks for the arrows
	GameUIManager::getInstance().GetTeamSelectionArrows()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot1Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot1Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot2Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[3]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot2Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[4]->registerCallback("onMouseClick", this, &TeamSelectionController::DecreaseSlot3Index);
	GameUIManager::getInstance().GetTeamSelectionArrows()[5]->registerCallback("onMouseClick", this, &TeamSelectionController::IncreaseSlot3Index);

	// Register the show info button callbacks
	GameUIManager::getInstance().GetShowInfoButtons()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::ToggleShowInfoPopup1);
	GameUIManager::getInstance().GetShowInfoButtons()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::ToggleShowInfoPopup2);
	GameUIManager::getInstance().GetShowInfoButtons()[2]->registerCallback("onMouseClick", this, &TeamSelectionController::ToggleShowInfoPopup3);

	//Register the save and load callbacks
	GameUIManager::getInstance().GetLoadLoadoutButton()->registerCallback("onMouseClick", this, &TeamSelectionController::ShowLoadLoadoutMenu);
	GameUIManager::getInstance().GetSaveLoadoutButton()->registerCallback("onMouseClick", this, &TeamSelectionController::ShowSaveLoadoutMenu);
}

void TeamSelectionController::SaveLoadout1()
{
	ConfermationNo();

	GameplayTypes::HEROID ids[3] = { mSlot1CharacterList[mSlot1Index]->getComponent<HeroComponent>()->GetID(), mSlot2CharacterList[mSlot2Index]->getComponent<HeroComponent>()->GetID(), mSlot3CharacterList[mSlot3Index]->getComponent<HeroComponent>()->GetID() };
	unsigned int indecies[3] = { (unsigned int)mSlot1Index, (unsigned int)mSlot2Index, (unsigned int)mSlot3Index };
	SaveLoad::Instance().AddLoadOut("Loadout_1", ids, indecies);
	SaveLoad::Instance().SaveLoadOut();

	HideLoadLoadoutMenu();
}

void TeamSelectionController::SaveLoadout2()
{
	ConfermationNo();

	GameplayTypes::HEROID ids[3] = { mSlot1CharacterList[mSlot1Index]->getComponent<HeroComponent>()->GetID(), mSlot2CharacterList[mSlot2Index]->getComponent<HeroComponent>()->GetID(), mSlot3CharacterList[mSlot3Index]->getComponent<HeroComponent>()->GetID() };
	unsigned int indecies[3] = { (unsigned int)mSlot1Index, (unsigned int)mSlot2Index, (unsigned int)mSlot3Index };
	SaveLoad::Instance().AddLoadOut("Loadout_2", ids, indecies);
	SaveLoad::Instance().SaveLoadOut();

	HideLoadLoadoutMenu();
}

void TeamSelectionController::SaveLoadout3()
{
	ConfermationNo();

	GameplayTypes::HEROID ids[3] = { mSlot1CharacterList[mSlot1Index]->getComponent<HeroComponent>()->GetID(), mSlot2CharacterList[mSlot2Index]->getComponent<HeroComponent>()->GetID(), mSlot3CharacterList[mSlot3Index]->getComponent<HeroComponent>()->GetID() };
	unsigned int indecies[3] = { (unsigned int)mSlot1Index,(unsigned int)mSlot2Index, (unsigned int)mSlot3Index };
	SaveLoad::Instance().AddLoadOut("Loadout_3", ids, indecies);
	SaveLoad::Instance().SaveLoadOut();

	HideLoadLoadoutMenu();
}

void TeamSelectionController::LoadLoadout1()
{
	if (SaveLoad::Instance().LoadLoadOut("Loadout_1"))
	{
		mSlot1CharacterList[mSlot1Index]->setVisible(false);
		mSlot1Index = SaveLoad::Instance().GetLoadOut("Loadout_1").index[0];
		mSlot1CharacterList[mSlot1Index]->setVisible(true);
		ChangeSlotName(0, mSlot1CharacterList[mSlot1Index]->getComponent<Character>()->GetName());

		mSlot2CharacterList[mSlot2Index]->setVisible(false);
		mSlot2Index = SaveLoad::Instance().GetLoadOut("Loadout_1").index[1];
		mSlot2CharacterList[mSlot2Index]->setVisible(true);
		ChangeSlotName(1, mSlot2CharacterList[mSlot2Index]->getComponent<Character>()->GetName());

		mSlot3CharacterList[mSlot3Index]->setVisible(false);
		mSlot3Index = SaveLoad::Instance().GetLoadOut("Loadout_1").index[2];
		mSlot3CharacterList[mSlot3Index]->setVisible(true);
		ChangeSlotName(2, mSlot3CharacterList[mSlot3Index]->getComponent<Character>()->GetName());

		HideLoadLoadoutMenu();
	}
}

void TeamSelectionController::LoadLoadout2()
{
	if (SaveLoad::Instance().LoadLoadOut("Loadout_2"))
	{
		mSlot1CharacterList[mSlot1Index]->setVisible(false);
		mSlot1Index = SaveLoad::Instance().GetLoadOut("Loadout_2").index[0];
		mSlot1CharacterList[mSlot1Index]->setVisible(true);
		ChangeSlotName(0, mSlot1CharacterList[mSlot1Index]->getComponent<Character>()->GetName());

		mSlot2CharacterList[mSlot2Index]->setVisible(false);
		mSlot2Index = SaveLoad::Instance().GetLoadOut("Loadout_2").index[1];
		mSlot2CharacterList[mSlot2Index]->setVisible(true);
		ChangeSlotName(1, mSlot2CharacterList[mSlot2Index]->getComponent<Character>()->GetName());

		mSlot3CharacterList[mSlot3Index]->setVisible(false);
		mSlot3Index = SaveLoad::Instance().GetLoadOut("Loadout_2").index[2];
		mSlot3CharacterList[mSlot3Index]->setVisible(true);
		ChangeSlotName(2, mSlot3CharacterList[mSlot3Index]->getComponent<Character>()->GetName());

		HideLoadLoadoutMenu();
	}
}

void TeamSelectionController::LoadLoadout3()
{
	if (SaveLoad::Instance().LoadLoadOut("Loadout_3"))
	{
		mSlot1CharacterList[mSlot1Index]->setVisible(false);
		mSlot1Index = SaveLoad::Instance().GetLoadOut("Loadout_3").index[0];
		mSlot1CharacterList[mSlot1Index]->setVisible(true);
		ChangeSlotName(0, mSlot1CharacterList[mSlot1Index]->getComponent<Character>()->GetName());

		mSlot2CharacterList[mSlot2Index]->setVisible(false);
		mSlot2Index = SaveLoad::Instance().GetLoadOut("Loadout_3").index[1];
		mSlot2CharacterList[mSlot2Index]->setVisible(true);
		ChangeSlotName(1, mSlot2CharacterList[mSlot2Index]->getComponent<Character>()->GetName());

		mSlot3CharacterList[mSlot3Index]->setVisible(false);
		mSlot3Index = SaveLoad::Instance().GetLoadOut("Loadout_3").index[2];
		mSlot3CharacterList[mSlot3Index]->setVisible(true);
		ChangeSlotName(2, mSlot3CharacterList[mSlot3Index]->getComponent<Character>()->GetName());

		HideLoadLoadoutMenu();
	}
}

void TeamSelectionController::ConfermationSave1()
{
	GameUIManager::getInstance().GetSaveConfermationMenu()->setActive(true);
	GameUIManager::getInstance().GetSaveConfermationButtons()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::ConfermationNo);
	GameUIManager::getInstance().GetSaveConfermationButtons()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::SaveLoadout1);
}

void TeamSelectionController::ConfermationSave2()
{
	GameUIManager::getInstance().GetSaveConfermationMenu()->setActive(true);
	GameUIManager::getInstance().GetSaveConfermationButtons()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::ConfermationNo);
	GameUIManager::getInstance().GetSaveConfermationButtons()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::SaveLoadout2);
}

void TeamSelectionController::ConfermationSave3()
{
	GameUIManager::getInstance().GetSaveConfermationMenu()->setActive(true);
	GameUIManager::getInstance().GetSaveConfermationButtons()[0]->registerCallback("onMouseClick", this, &TeamSelectionController::ConfermationNo);
	GameUIManager::getInstance().GetSaveConfermationButtons()[1]->registerCallback("onMouseClick", this, &TeamSelectionController::SaveLoadout3);
}

void TeamSelectionController::ConfermationNo()
{
	GameUIManager::getInstance().GetSaveConfermationMenu()->setActive(false);
	GameUIManager::getInstance().GetSaveConfermationButtons()[0]->unregisterCallback("onMouseClick");
	GameUIManager::getInstance().GetSaveConfermationButtons()[1]->unregisterCallback("onMouseClick");
}

