#include "GameUIManager.h"
#include "RedAudioManager.h"
#include "UICanvas.h"

// TODO: REFACTOR LATER
#include "SkillHoverComponent.h"

#define BackgroundBigOpacity 0.5f
#define BackgroundSmallOpacity 0.8f

GameUIManager& GameUIManager::getInstance()
{
	static GameUIManager instance;
	return instance;
}

void GameUIManager::CreateBattleLog(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	// Create the battle log object
	mBattleLog = std::make_shared<Odyssey::Entity>();
	mBattleLog->addComponent<Odyssey::UICanvas>();
	// Get the canvas
	Odyssey::UICanvas* battleLogCanvas = mBattleLog->getComponent<Odyssey::UICanvas>();

	// Setup init values
	UINT width = battleTextWidth = (screenWidth / 4) + 50;
	UINT height = battleTextHeight = (screenHeight / 16);

	DirectX::XMFLOAT2 position = { 10.0f, (static_cast<float>(screenHeight) / 3.0f) + 280.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color

	// Setup text properties
	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = false;
	properties.fontSize = 14.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	properties.fontName = L"Tw Cen MT Condensed";

	// Create the battle log text
	mBattleLogText = battleLogCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"", properties);
	mBattleLogVec.push_back(mBattleLogText);
	position.y -= 50.0f;
	mBattleLogText = battleLogCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"", properties);
	mBattleLogVec.push_back(mBattleLogText);


	// Add the entity to the game scene
	_sceneToAddTo->addEntity(mBattleLog);
}


void GameUIManager::SetBattleLogText(std::wstring newText, bool concat)
{
	if (concat)
	{
		mBattleLogVec[0]->setText(mBattleLogVec[0]->getText().append(L" "));
		mBattleLogVec[0]->setText(mBattleLogVec[0]->getText().append(newText));
		return;
	}


	for (int i = mBattleLogVec.size() - 1; i > 0; i--)
	{
		if (i - 1 >= 0)
			mBattleLogVec[i]->setText(mBattleLogVec[i-1]->getText());
	}
	
	mBattleLogVec[0]->setText(newText);

}

void GameUIManager::ToggleCanvas(Odyssey::UICanvas* _canvas, bool _isActive)
{
	// Set the passed in entity's canvas to active or deactived based on the bool _isActive.
	_canvas->setActive(_isActive);
}

// This is where I will design and add all the elements into the main menu screen
void GameUIManager::CreateMainMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	//Create the pause menu pointer
	mMainMenu = std::make_shared<Odyssey::Entity>();
	mMainMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the main menu
	Odyssey::UICanvas* mainMenuCanvas = mMainMenu->getComponent<Odyssey::UICanvas>();

	// Setup init values
	UINT width = screenWidth;
	UINT height = screenHeight;
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color
	// Set up text properties
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 60.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Tw Cen MT Condensed";

	// Create title text
	height = 100;
	mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"The Dying Dungeon", properties);

	// Create new game text 
	properties.bold = false;
	properties.fontSize = 40.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	position.x += 200.0f;
	position.y += 250.0f;
	width = 200;
	height = 40;
	// TODO: MAYBE COMMENT THIS IN? LOOKS DUMB
	//mainMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f), width, height);
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mNewGameText = mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"New Game", properties);
	position.y += 70.0f;
	width += 70;
	mStatsText = mainMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Game Statistics", properties);
	// Setup team logo
	mTeamLogo = mainMenuCanvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(0.0f, 0.0f), L"assets/images/GreatApeStudiosLogo.png", screenWidth, screenHeight);

	// Add the pause menu to the main scene
	_sceneToAddTo->addEntity(mMainMenu);
}

// This is where I will design and add all the elements into the tower select screen
void GameUIManager::CreateTowerSelectMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	// Create the tower select menu pointer
	mTowerSelectMenu = std::make_shared<Odyssey::Entity>();
	mTowerSelectMenu->addComponent<Odyssey::UICanvas>();
	mTowerInfoCanvas = mTowerSelectMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the pause menu
	Odyssey::UICanvas* towerSelectMenuCanvas = mTowerSelectMenu->getComponent<Odyssey::UICanvas>();

	// Initialize variables
	UINT width = screenWidth; // Width
	UINT height = 50; // Height
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color
	Odyssey::TextProperties properties;

	// Create tower select title
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 50.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Tw Cen MT Condensed";
	mTowerSelectTitle = towerSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Select A Tower To Enter", properties);

	// Create door to click on
	width = 195;
	height = 231;
	position = { 50.83f, (static_cast<float>(screenHeight) - static_cast<float>(height))};
	Odyssey::Sprite2D* doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	// Create door to click on
	position.x +=  width + 50.83f;
	doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	// Create door to click on
	position.x += width + 50.83f;
	doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	// Create door to click on
	position.x += width + 50.83f;
	doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	// Create door to click on
	position.x += width + 50.83f;
	doorImage = towerSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/DoorImages/MedievalDoor-1.png", width, height);
	mDoorImages.push_back(doorImage);

	// Start creating the tower info popup
	width = 500;
	height = 250;
	position.x = (static_cast<float>(screenWidth) / 2.0f) - (static_cast<float>(width) / 2.0f);
	position.y = 100.0f;
	color = { 0.0f, 0.0f, 0.0f, 1.0f };
	Odyssey::Rectangle2D* rect = mTowerInfoCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	rect->setOpacity(BackgroundSmallOpacity);

	// Add in tower level text
	height = 25;
	// Give a little padding
	position.x += 5.0f;
	position.y += 5.0f;
	// Set text elements
	properties.fontSize = 25.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Tower Level: 1", properties);

	// Add in enemies text and icons
	position.y += static_cast<float>(height) + 10.0f;
	float previousX = position.x;
	width = 120;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Enemies:", properties);
	// Add in the icons
	position.x += static_cast<float>(width);
	width = 64;
	height = 64;
	mTowerInfoCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/SkeletonPortrait.png", width, height);
	position.x += static_cast<float>(width) + 10.0f;
	mTowerInfoCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/GanfaulPortrait.jpg", width, height);

	// Set up rewards text
	position.x = previousX;
	position.y += static_cast<float>(height) + 5.0f;
	width = 300;
	height = 25;
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Rewards: 900 XP", properties);

	// Set up description
	position.y += static_cast<float>(height) + 20.0f;
	width = 300;
	height = 15;
	properties.fontSize = 15.0f;
	properties.italic = true;
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Description:", properties);
	position.y += static_cast<float>(height) + 5.0f;
	width = 500;
	height = 200;
	properties.fontSize = 12.0f;
	mTowerInfoCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"A grand pair of worn statues in a overcast mountain top marks the entrance to this dungeon. Beyond the pair of worn statues lies a grand, humid room. It's covered in remains, ash and ash. Your torch allows you to see carved out openings filled with pottery, wornand ravished by time itself", properties);
	// Disable the tower info canvas
	mTowerInfoCanvas->setActive(false);

	// Add the pause menu to the game scene most likely
	_sceneToAddTo->addEntity(mTowerSelectMenu);
}

void GameUIManager::CreateTeamSelectMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	// Create the tower select menu pointer
	mTeamSelectMenu = std::make_shared<Odyssey::Entity>();
	mTeamSelectMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the pause menu
	Odyssey::UICanvas* teamSelectMenuCanvas = mTeamSelectMenu->getComponent<Odyssey::UICanvas>();

	// Initialize variables
	UINT width = screenWidth; // Width
	UINT height = 50; // Height
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color
	Odyssey::TextProperties properties;

	// Create team select title
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 50.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Tw Cen MT Condensed";
	teamSelectMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Select 3 Team Members", properties);

	// Create the 3 boxes where the character's images will go when you click on a certain hero
	position.x = 50.0f;
	width = 200;
	height = 200;
	
	// First team member
	position.y += 30.0f;
	firstTeamMemberSlot = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/blackSquare.png", width, height);

	// Second team member
	position.y += static_cast<float>(height) + 30.0f;
	secondTeamMemberSlot = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/blackSquare.png", width, height);

	// Third team member
	position.y += static_cast<float>(height) + 30.0f;
	thirdTeamMemberSlot = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/blackSquare.png", width, height);

	// Make the clickable clear rectangle for the character's when selecting the team members
	width = 256;
	height = 400;
	position.x = 400.0f;
	position.y = 250.0f;
	//paladinSelectionImage = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/blackOutlineSquare.png", width, height);
	paladinSelectionImage = teamSelectMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), width, height);

	position.x = 825.0f;
	position.y = 250.0f;
	//mageSelectionImage = teamSelectMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/blackOutlineSquare.png", width, height);
	mageSelectionImage = teamSelectMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), width, height);

	// Add the mTeamSelectMenu object to the team selection scene
	_sceneToAddTo->addEntity(mTeamSelectMenu);
}

// This is where I will design and add all elements into the pause menu canvas
void GameUIManager::CreatePauseMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	//Create the pause menu pointer
	mPauseMenu = std::make_shared<Odyssey::Entity>();
	mPauseMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the pause menu
	Odyssey::UICanvas* pauseMenuCanvas = mPauseMenu->getComponent<Odyssey::UICanvas>();

	// Create the rectangle object
	UINT width = 1920; // Width
	UINT height = 1080; // Height
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f }; // Color
	// Add the rectangle to the pause menu canvas
	mBlackBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Make the rectangle have 50% transparency
	mBlackBackground->setOpacity(BackgroundBigOpacity);

	// Create the pause menu smaller black rectangle
	width = 640;
	height = 360;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	// Add the rectangle to the pause menu canvas
	mSmallerBlackBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	mSmallerBlackBackground->setOpacity(BackgroundSmallOpacity);

	// Create Pause Title
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 60.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Tw Cen MT Condensed";
	mPauseTitle = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, 640, 60, L"Paused", properties);

	// Resume Button
	width /= 2.5f;
	height = 50;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 50.0f;
	color = { 30.0f, 180.0f, 30.0f, 1.0f };
	// Add the reusume background to the canvas
	mResumeBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Resume text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.bold = false;
	properties.fontSize = 30.0f;
	mResumeText = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"RESUME", properties);

	// Options Button
	position.y += 75.0f;
	color = { 30.0f, 180.0f, 30.0f, 1.0f };
	// Add the options background to the canvas
	mOptionsBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Options text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mOptionsText = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"OPTIONS", properties);

	// Main Menu Button
	position.y += 75.0f;
	color = { 180.0f, 30.0f, 30.0f, 1.0f };
	// Add the main menu background to the canvas
	mMainMenuBackground = pauseMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Main menu text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	mMainMenuText = pauseMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"MAIN MENU", properties);

	// Add the pause menu to the game scene most likely
	_sceneToAddTo->addEntity(mPauseMenu);
	// Turn off the canvas when creating it
	ToggleCanvas(mPauseMenu->getComponent<Odyssey::UICanvas>(), false);

	// Make options menu
	CreateOptionsMenu(_sceneToAddTo);
}

void GameUIManager::CreateStatsMenuCanvas(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	//Create the pause menu pointer
	mStatsMenu = std::make_shared<Odyssey::Entity>();
	mStatsMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the main menu
	Odyssey::UICanvas* statsMenuCanvas = mStatsMenu->getComponent<Odyssey::UICanvas>();

	// Setup init values
	//UINT width = screenWidth;
	//UINT height = screenHeight;
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT2 graphPosition = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 31.0f, 255.0f, 244.0f, 1.0f }; // Color
	// Set up text properties
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 60.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";

	UINT graphBackgroundWidth = 750;
	UINT graphBackgroundHeight = 450;
	graphPosition.x = position.x = (screenWidth / 2.0f) - (static_cast<float>(graphBackgroundWidth) / 2.0f);
	graphPosition.y = position.y = (screenHeight / 2.0f) - (static_cast<float>(graphBackgroundHeight) / 2.0f);
	statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), graphBackgroundWidth, graphBackgroundHeight);
	statsMenuCanvas->addElement<Odyssey::Text2D>(position, color, graphBackgroundWidth, graphBackgroundHeight, L"NO STATS", properties);
	statsMenuCanvas->getElement<Odyssey::Text2D>()->setVisible(false);

	UINT graphWidth = graphBackgroundWidth + 20;
	UINT graphHeight = graphBackgroundHeight + 20;

	//UINT barWidth = (graphWidth - 80) / 7;
	//float max_damage = 200.0f;
	//position.x += 50.0f;

	//for (int i = 0; i < 7; i++)
	//{
	//	UINT barHeight = (graphHeight - 80) * (static_cast<float>(rand() % (200 - 75 + 1) + 75) / max_damage);
	//	position.y = graphPosition.y + (graphHeight - barHeight) - 60.0f;
	//	statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth - 5, barHeight)->setVisible(true);
	//	position.x += barWidth;
	//	properties.fontSize = 15.0f;
	//	statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - barWidth, position.y + barHeight), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), 100, 25, std::to_wstring(i + 1), properties);
	//}
	//float intervals = max_damage / 10.0f;
	//for (int j = 0; j < 10; j++)
	//{
	//	statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(graphPosition.x, graphPosition.y + ((graphBackgroundHeight - 60.0f) * 0.1f * j)), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), 50, 50, std::to_wstring(max_damage - (j*intervals)).substr(0, 4), properties);
	//}

	position.y = graphPosition.y - (graphHeight/2.0f) + 40.0f;
	position.x = graphPosition.x;

	properties.fontSize = 60.0f;
	statsMenuCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), graphBackgroundWidth, graphBackgroundHeight, L"Damage", properties);

	position.y = graphPosition.y + (graphHeight / 2.0f) + 20.0f;
	properties.fontSize = 30.0f;

	mStatsBackButtonText = statsMenuCanvas->addElement<Odyssey::Text2D>(position, DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), graphBackgroundWidth, graphBackgroundHeight, L"Back", properties);
	mStatsBackButtonText->registerCallback("onMouseClick", this, &GameUIManager::HideStatsMenu);

	_sceneToAddTo->addEntity(mStatsMenu);
	
	ToggleCanvas(mStatsMenu->getComponent<Odyssey::UICanvas>(), false);

}

void GameUIManager::ToggleStatsMenu()
{
	mMainMenu->getComponent<Odyssey::UICanvas>()->setActive(false);
	mStatsMenu->getComponent<Odyssey::UICanvas>()->setActive(true);
	UpdateGraph();
}

void GameUIManager::HideStatsMenu()
{
	mMainMenu->getComponent<Odyssey::UICanvas>()->setActive(true);
	mStatsMenu->getComponent<Odyssey::UICanvas>()->setActive(false);
}

void GameUIManager::UpdateGraph()
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 graphPosition = { 0.0f, 0.0f };
	DirectX::XMFLOAT4 color = { 31.0f, 255.0f, 244.0f, 1.0f };

	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 60.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Constantia";

	UINT graphBackgroundWidth = 750;
	UINT graphBackgroundHeight = 450;
	graphPosition.x = position.x = (screenWidth / 2.0f) - (static_cast<float>(graphBackgroundWidth) / 2.0f);
	graphPosition.y = position.y = (screenHeight / 2.0f) - (static_cast<float>(graphBackgroundHeight) / 2.0f);

	/*for (int i = 1; i < mStatsMenu->getComponent<Odyssey::UICanvas>()->getElements<Odyssey::Rectangle2D>().size(); i++)
	{
		mStatsMenu->getComponent<Odyssey::UICanvas>()->removeElement(mStatsMenu->getComponent<Odyssey::UICanvas>()->getElements<Odyssey::Rectangle2D>()[i]);
	}
	for (int j = 1; j < mStatsMenu->getComponent<Odyssey::UICanvas>()->getElements<Odyssey::Text2D>().size(); j++)
	{
		mStatsMenu->getComponent<Odyssey::UICanvas>()->removeElement(mStatsMenu->getComponent<Odyssey::UICanvas>()->getElements<Odyssey::Text2D>()[j]);
	}*/

	Odyssey::UICanvas* statsMenuCanvas = mStatsMenu->getComponent<Odyssey::UICanvas>();
	UINT graphWidth = graphBackgroundWidth + 20;
	UINT graphHeight = graphBackgroundHeight + 20;
	
	if (StatTracker::Instance().GetLevelSize() > 0) {

		statsMenuCanvas->getElement<Odyssey::Text2D>()[0].setVisible(false);

		graphPosition.x = (screenWidth / 2.0f) - (static_cast<float>(graphBackgroundWidth) / 2.0f);
		graphPosition.y = (screenHeight / 2.0f) - (static_cast<float>(graphBackgroundHeight) / 2.0f);

		float max_damage = 0.0f;

		for (int k = 0; k < StatTracker::Instance().GetRoundCount(1); k++)
		{
			float curr_dmg = StatTracker::Instance().CalculateDamageDealt(1, (k + 1));
			if (max_damage < curr_dmg)
			{
				max_damage = curr_dmg;
			}
		}

		UINT barWidth = (graphWidth - 80) / StatTracker::Instance().GetRoundCount(1);
		position.x += 50.0f;

		for (int l = 0; l < StatTracker::Instance().GetRoundCount(1); l++)
		{
			UINT barHeight = (graphHeight - 80) * (StatTracker::Instance().CalculateDamageDealt(1, (l + 1)) / max_damage);
			position.y = graphPosition.y + (graphHeight - barHeight) - 60.0f;
			statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth - 5, barHeight)->setVisible(true);
			position.x += barWidth;
			properties.fontSize = 15.0f;
			statsMenuCanvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(position.x - barWidth, position.y + barHeight), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), 100, 25, std::to_wstring(l + 1), properties);
		}
	}
	else
	{
		statsMenuCanvas->getElement<Odyssey::Text2D>()[0].setVisible(true);
	}
	/*if (StatTracker::Instance().GetLevelSize() > 0) {
		UINT barWidth = graphWidth / StatTracker::Instance().GetRoundCount(1);
		statsMenuCanvas->getElement<Odyssey::Text2D>()[0].setVisible(false);
		position.x += 20.0f;
		position.y += 20.0f;
		for (int i = 0; i < StatTracker::Instance().GetRoundCount(1); i++)
		{
			statsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, graphHeight);
		}
	}
	else
	{
		statsMenuCanvas->getElement<Odyssey::Text2D>()[0].setVisible(true);
	}*/
	

}

// This is where I will design and add all elements into the options menu canvas
void GameUIManager::CreateOptionsMenu(std::shared_ptr<Odyssey::Scene> _sceneToAddTo)
{
	// Set options menu pointer and add a canvas
	mOptionsMenu = std::make_shared<Odyssey::Entity>();
	mOptionsMenu->addComponent<Odyssey::UICanvas>();
	// Get canvas component of the options menu
	Odyssey::UICanvas* optionsMenuCanvas = mOptionsMenu->getComponent<Odyssey::UICanvas>();

	// Set init values
	// Create the rectangle object
	UINT width = 1920; // Width
	UINT height = 1080; // Height
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f }; // Color
	// Add the rectangle to the options menu canvas
	mBlackBackground = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Make the rectangle have 50% transparency
	mBlackBackground->setOpacity(BackgroundBigOpacity);

	// Create the pause menu smaller black rectangle
	width = 640;
	height = 360;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	DirectX::XMFLOAT2 originalPosition = position;
	// Add the rectangle to the options menu canvas
	mSmallerBlackBackground = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	mSmallerBlackBackground->setOpacity(BackgroundSmallOpacity);

	// Create options title
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 60.0f;
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	properties.fontName = L"Tw Cen MT Condensed";
	mOptionsTitle = optionsMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, 60, L"Options", properties);

	// Adjust volume text
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.bold = false;
	properties.fontSize = 20.0f;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 75.0f;
	mVolumeText = optionsMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Adjust Volume", properties);
	// Volume bar
	width /= 2.0f;
	height = 25;
	position = { ((screenWidth / 2.0f) - (width / 2.0f)) , ((screenHeight / 2.0f) - (height / 2.0f)) };
	position.y -= 45.0f;
	// Create the volume bar background
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(position.x - 2.5f, position.y - 2.5f), color, width + 5.0f, height + 5.0f);
	color = { 70.0f, 70.0f, 70.0f, 1.0f };
	optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// Add the volume bar to the canvas
	color = { 50.0f, 180.0f, 255.0f, 1.0f };
	mVolumeBar = optionsMenuCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	// TODO :: FILL THE RECTANGLE BASED ON THE VOLUME LEVEL
	mVolumeBar->setFill(static_cast<float>(RedAudioManager::Instance().GetVolume()) / 1000.0f);

	// Create the plus and minus symbols that the user can click on to adjust the volume
	UINT imageWidth = 32;
	UINT imageHeight = 32;
	position.x -= static_cast<float>(imageWidth) * 1.5f;
	position.y -= 4.0f;
	// Minus Symbol
	mMinusImage = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/minusSymbol.png", imageWidth, imageHeight);
	mMinusImage->registerCallback("onMouseClick", this, &GameUIManager::DecreaseVolume);
	// Plus Symbol
	position.x += (static_cast<float>(imageWidth) * 1.5f) + width + (static_cast<float>(imageWidth) / 2.0f);
	mPlusImage = optionsMenuCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/plusSymbol.png", imageWidth, imageHeight);
	mPlusImage->registerCallback("onMouseClick", this, &GameUIManager::IncreaseVolume);

	// Create the options back button
	position = originalPosition;
	width = 100.0f;
	height = 30.0f;
	position.y += 360.0f - height;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	properties.textAlignment = Odyssey::TextAlignment::Center;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	// Add the back button text
	mBackButtonText = optionsMenuCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Back", properties);
	// Have the back button go to the pause menu
	mBackButtonText->registerCallback("onMouseClick", this, &GameUIManager::OptionsBackButton);

	// Add the options menu to the game scene most likely
	_sceneToAddTo->addEntity(mOptionsMenu);
	// Turn off the canvas when creating it
	ToggleCanvas(mOptionsMenu->getComponent<Odyssey::UICanvas>(), false);
}

// This function will get called when you click on the back button on the options menu in pause
void GameUIManager::OptionsBackButton()
{
	// Turn off the options menu's canvas
	mOptionsMenu->getComponent<Odyssey::UICanvas>()->setActive(false);
	
	// Turn on the pause menu's canvas
	mPauseMenu->getComponent<Odyssey::UICanvas>()->setActive(true);
}

// Create the character's UI Portrait
Odyssey::UICanvas* GameUIManager::CreateCharacterPortrait(float anchorX, float anchorY, std::wstring _imageName, Odyssey::Entity* _gameObjectToAddTo, Character* owner)
{
	// Create CharacterHUD object
	std::shared_ptr<CharacterHUD> newHUD = std::make_shared<CharacterHUD>();
	// Set the canvas 
	newHUD->pCanvas = _gameObjectToAddTo->addComponent<Odyssey::UICanvas>();
	
	// Create Hero UI if the character is a hero
	if (owner->IsHero())
	{
		Odyssey::TextProperties properties;
		properties.bold = false;
		properties.italic = false;
		properties.fontSize = 14.0f;
		properties.textAlignment = Odyssey::TextAlignment::Left;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
		properties.fontName = L"Tw Cen MT Condensed";

		// Get the position of the anchor points
		DirectX::XMFLOAT2 position = { anchorX, anchorY };
		// Save the original position for the future when creating new items
		DirectX::XMFLOAT2 originalPosition = position;
		// Set the image width and height
		// Set the bar width and height for the Rectangle2Ds
		UINT imageWidth = 359;
		UINT imageHeight = 109;
		UINT barWidth = 252.5f;
		UINT barHeight = 21;
		DirectX::XMFLOAT4 color = { 116.0f, 71.0f, 201.0f, 1.0f };

		// Create the base ui template
		newHUD->pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/HeroUILayout4.0.png", imageWidth, imageHeight);

		// Create the character's image
		imageWidth = 71;
		imageHeight = 68;
		position.x += 2;
		position.y += 1;
		// Only add portrait if there is an image name
		if (_imageName != L" ")
			newHUD->pPortrait = newHUD->pCanvas->addElement<Odyssey::Sprite2D>(position, _imageName, imageWidth, imageHeight);

		// Create the xp/name bar
		//TODO::Create XP Bars In The Character Class To Assign
		position.x += imageWidth + 2.0f;
		newHUD->pXpBar = newHUD->pCanvas->addElement<Odyssey::Rectangle2D>(position, color, barWidth, barHeight);

		// Create the text element of the character's name
		color = { 0.0f, 0.0f, 0.0f, 1.0f };
		position.x += 7.5f;
		// Add the name to the canvas
		properties.bold = true;

		newHUD->pCharacterName = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, owner->GetName(), properties);

		properties.bold = false;

		// Create the character's level number text next to the XP bar
		position.x += barWidth;
		color = { 255.0f, 255.0f, 255.0f, 1.0f };
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
		newHUD->pLevelNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, L"01", properties);

		// Add the text elements for Attack, Defense, and Speed;
		position = originalPosition;
		position.x += 100.0f;
		position.y += 22.0f;
		properties.fontSize = 12.0f;
		properties.textAlignment = Odyssey::TextAlignment::Left;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
		color = { 255.0f, 255.0f, 255.0f, 1.0f };
		// Attack number
		newHUD->pAttackNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring(static_cast<int>(owner->GetAtk() * 100.0f)), properties);
		// Defense number
		position.y += 16.5f;
		newHUD->pDefenseNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring(static_cast<int>(owner->GetDef() * 100.0f)), properties);
		// Speed number
		position.y += 16.5f;
		newHUD->pSpeedNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, color, 20, barHeight, std::to_wstring((int)owner->GetSpeed()), properties);

		// Health and Mana bars
		// Set the position to the origanal top left position
		position = originalPosition;
		barHeight = 17;
		barWidth = 356;
		position.x += 2.0f;
		position.y += imageHeight + 3.5f;
		// Create and assign the health bar
		newHUD->pHealthBar = newHUD->pCanvas->addElement<Odyssey::Rectangle2D>(position, mHealthBarColor, barWidth, barHeight);
		newHUD->pHealthBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		// Create the text for the health numbers of the character
		color = { 255.0f, 255.0f, 255.0f, 1.0f };
		properties.fontSize = 10.5f;
		position.x += 5.0f;
		newHUD->pHealthNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, std::to_wstring((int)owner->GetHP()) + L"/" + std::to_wstring((int)owner->GetMaxHP()), properties);
		// Create and assign the mana bar
		position.x -= 5.0f;
		position.y += barHeight + 1.5;
		newHUD->pManaBar = newHUD->pCanvas->addElement<Odyssey::Rectangle2D>(position, mManaBarColor, barWidth, barHeight);
		newHUD->pManaBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		// Create the text for the mana numbers of the character
		color = { 255.0f, 255.0f, 255.0f, 1.0f };
		position.x += 5.0f;
		newHUD->pManaNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, color, barWidth, barHeight, std::to_wstring((int)owner->GetMana()) + L"/" + std::to_wstring((int)owner->GetMaxMana()), properties);

		// Position where the turn number will be located
		position = originalPosition;
		position.x += 5.0f;
		properties.fontSize = 14.0f;
		// Assign the character's turn order text
		newHUD->pTurnNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, mTurnOrderColor, 32, 32, L"1", properties);

		// STUFF
		//Paladin_Skill_1.png
		//position = originalPosition;
		//position.x += 134.0f;
		//position.y += 24.0f;
		//Odyssey::Sprite2D* s1 = newHUD->pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Paladin_Skill_1.png", 52, 45);
		//position.x += 56.5f;
		//Odyssey::Sprite2D* s2 = newHUD->pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Paladin_Skill_1.png", 52, 45);
		//position.x += 56.5f;
		//Odyssey::Sprite2D* s3 = newHUD->pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Paladin_Skill_1.png", 52, 45);
		//position.x += 56.5f;
		//Odyssey::Sprite2D* s4 = newHUD->pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/Paladin_Skill_1.png", 52, 45);
		
		// TODO: REFACTOR LATER
		//newHUD->pCanvas->getEntity()->addComponent<SkillHoverComponent>();
		//newHUD->pCanvas->getEntity()->getComponent<SkillHoverComponent>()->registerSprite(s1, CreatePopup(newHUD->pCanvas->getEntity()));
		//newHUD->pCanvas->getEntity()->getComponent<SkillHoverComponent>()->registerSprite(s2, CreatePopup(newHUD->pCanvas->getEntity()));
		//newHUD->pCanvas->getEntity()->getComponent<SkillHoverComponent>()->registerSprite(s3, CreatePopup(newHUD->pCanvas->getEntity()));
		//newHUD->pCanvas->getEntity()->getComponent<SkillHoverComponent>()->registerSprite(s4, CreatePopup(newHUD->pCanvas->getEntity()));
	}
	// Create the Enemy UI if the character is NOT a hero
	else
	{
		Odyssey::TextProperties properties;
		properties.bold = false;
		properties.italic = false;
		properties.fontSize = 14.0f;
		properties.textAlignment = Odyssey::TextAlignment::Left;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
		properties.fontName = L"Tw Cen MT Condensed";

		// Get the position of the anchor points
		DirectX::XMFLOAT2 position = { anchorX, anchorY };
		// Save the original position for the future when creating new items
		DirectX::XMFLOAT2 originalPosition = position;
		// Set the image width and height
		// Set the bar width and height for the Rectangle2Ds
		UINT imageWidth = 228;
		UINT imageHeight = 43;
		UINT barWidth = 185;
		UINT barHeight = 12;
		DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f };

		// Add in the enemy hud template
		newHUD->pCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/EnemyHUDTemplate.jpg", imageWidth, imageHeight);

		// Add in the enemy's portrait picture
		imageWidth = 43;
		imageHeight = 43;
		// Only add the portrait image if there is one
		if (_imageName != L" ")
			newHUD->pCanvas->addElement<Odyssey::Sprite2D>(position, _imageName, imageWidth, imageHeight);

		// Add in the enemy's health bar
		position.x += imageWidth;
		newHUD->pHealthBar = newHUD->pCanvas->addElement<Odyssey::Rectangle2D>(position, mHealthBarColor, barWidth, barHeight);
		newHUD->pHealthBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		// Add big health text
		position.x += barWidth;
		properties.fontSize = 25.0f;
		properties.textAlignment = Odyssey::TextAlignment::Center;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
		// Create but don't show the mini hp text
		newHUD->pHealthNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, mTextColor, 43, 43, std::to_wstring((int)owner->GetHP()), properties);
		newHUD->pHealthNumber->setVisible(false);

		// Add in the enemy's mana bar
		newHUD->pManaBar = newHUD->pCanvas->addElement<Odyssey::Rectangle2D>(position, mManaBarColor, barWidth, barHeight);
		newHUD->pManaBar->enableColorLerp(DirectX::XMFLOAT3(255.0f, 0.0f, 0.0f));
		newHUD->pManaBar->setVisible(false);
		newHUD->pManaNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, mTextColor, 43, 43, std::to_wstring((int)owner->GetHP()), properties);
		newHUD->pManaNumber->setVisible(false);

		// Position where the turn number will be located
		position = originalPosition;
		position.x += 5.0f;
		properties.fontSize = 14.0f;
		properties.textAlignment = Odyssey::TextAlignment::Left;
		properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
		// Assign the character's turn order text
		newHUD->pTurnNumber = newHUD->pCanvas->addElement<Odyssey::Text2D>(position, mTurnOrderColor, 32, 32, L"1", properties);
	}

	// Only add the skill icons if it is a hero
	if (owner->IsHero())
	{
		// Create the skill icons for the character's hud
		DirectX::XMFLOAT2 pHudPosition = { anchorX, anchorY };
		SetupSkillIcons(_gameObjectToAddTo, owner, pHudPosition);
	}
	// Add the canvas to the mHudCharacterList
	mCharacterHudList.push_back(newHUD);
	// Return the canvas we just created 
	return newHUD->pCanvas;
}


void GameUIManager::SetupSkillIcons(Odyssey::Entity* _objToAddTo, Character* _newCharacter, DirectX::XMFLOAT2 _hudPosition)
{
	Odyssey::UICanvas* canvas1 = _objToAddTo->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas2 = _objToAddTo->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas3 = _objToAddTo->addComponent<Odyssey::UICanvas>();
	Odyssey::UICanvas* canvas4 = _objToAddTo->addComponent<Odyssey::UICanvas>();
	SkillHoverComponent* hover = _objToAddTo->addComponent<SkillHoverComponent>();

	// Set the correct offset position for the skills
	float xAnchor = _hudPosition.x + 134.0f;
	float yAnchor = _hudPosition.y + 24.0f;

	// 1st Skill
	// Skill Icon
	Odyssey::Sprite2D* skill1 = _objToAddTo->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Paladin_Skill_1.png", 52, 45);
	// Skill Hover Popup
	SetupSkillHover(canvas1, L"Paladin", L"Basic Attack", L"assets/images/Paladin_Skill_1.png", L"0", L"Attack", L"1", L"15 dmg",
		L"Description: Strike an enemy with divine power dealing 15 damage with a 30% chance to apply provoke. Restores 5 mana.");
	// Basic Attack trigger
	hover->registerSprite(skill1, canvas1);

	// Increment the icon
	xAnchor += 56.5f;

	// 2nd Skill
	// Skill Icon
	Odyssey::Sprite2D* skill2 = _objToAddTo->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Paladin_Skill_2.png", 52, 45);
	// Skill Hover Popup
	SetupSkillHover(canvas2, L"Paladin", L"Judgement", L"assets/images/Paladin_Skill_2.png", L"15", L"Attack", L"1", L"200 dmg",
		L"Description: Smite the enemy with holy light dealing 200 damage and healing the paladin for 15 health. Costs 15 mana.");
	// Wind Slash trigger
	hover->registerSprite(skill2, canvas2);

	// Increment the icon
	xAnchor += 56.5f;

	// 3rd Skill
	// Skill Icon
	Odyssey::Sprite2D* skill3 = _objToAddTo->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Paladin_Skill_3.png", 52, 45);
	// Skill Hover Popup
	SetupSkillHover(canvas3, L"Paladin", L"Shield of Light", L"assets/images/Paladin_Skill_3.png", L"20", L"Support", L"4", L"+25 shield",
		L"Description: A shield of light slams down in front of all team members granting a 25 health shield for 3 turns. Costs 20 mana.");
	// Firestorm trigger
	hover->registerSprite(skill3, canvas3);

	// Increment the icon
	xAnchor += 56.5f;

	// 4th Skill
	// Skill Icon
	Odyssey::Sprite2D* skill4 = _objToAddTo->getComponent<Odyssey::UICanvas>()->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(xAnchor, yAnchor), L"assets/images/Paladin_Skill_4.png", 52, 45);
	// Skill Hover Popup
	SetupSkillHover(canvas4, L"Paladin", L"Blessing of Light", L"assets/images/Paladin_Skill_4.png", L"15", L"Support", L"4", L"+50% def",
		L"Description: Protects all allies from harm granting them 50% reduced damage for 3 turns. Costs 15 mana.");
	// Lightning Bolt trigger
	hover->registerSprite(skill4, canvas4);
}

void GameUIManager::SetupSkillHover(Odyssey::UICanvas* canvas, std::wstring character, std::wstring skillName, std::wstring icon, std::wstring manaCost, std::wstring skillType, std::wstring numTargets, std::wstring skillValue, std::wstring description)
{
	// Append the number of targets
	std::wstring targets = L"Targets: ";
	std::wstring valueText;
	targets = targets.append(numTargets.c_str());

	DirectX::XMFLOAT4 themeColor;

	if (character == L"Paladin")
	{
		themeColor = DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f);
	}
	else if (character == L"Mage")
	{
		themeColor = DirectX::XMFLOAT4(31.0f, 255.0f, 203.0f, 1.0f);
	}

	if (skillType == L"Attack")
	{
		valueText = L"Damage: ";
		valueText = valueText.append(skillValue.c_str());
	}
	else if (skillType == L"Support")
	{
		valueText = L"Value: ";
		valueText = valueText.append(skillValue.c_str());
	}
	else if (skillType == L"Heal")
	{
		valueText = L"Heal: ";
		valueText = valueText.append(skillValue.c_str());
	}

	UINT windowWidth = screenWidth;
	UINT windowHeight = screenHeight;
	float x = 950;
	float y = 425;
	UINT width = 300;
	UINT height = 150;
	UINT pad = 10;

	Odyssey::TextProperties title;
	title.bold = true;
	title.italic = false;
	title.fontSize = 24.0f;
	title.textAlignment = Odyssey::TextAlignment::Center;
	title.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	title.fontName = L"Tw Cen MT Condensed";

	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = true;
	properties.fontSize = 14.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	properties.fontName = L"Tw Cen MT Condensed";

	// Background and Separators
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y), DirectX::XMFLOAT4(50.5f, 50.5f, 50.5f, 0.75f), width, height);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 40), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 3);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 80), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 3);

	// Title Text and Icons
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 40, y), themeColor, width - 80, 40, skillName, title);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x, y), icon, 40, 40);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x + width - 40, y), DirectX::XMFLOAT4(50.0f, 50.0f, 50.0f, 1.0f), 40, 40);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + width - 40, y), DirectX::XMFLOAT4(0.0f, 122.5f, 122.5f, 1.0f), 40, 40, manaCost, title);

	// Skill Info
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad, y + 40 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 25 + pad, y + 50), themeColor, 150, 50, skillType, properties);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + 80 + pad, y + 40 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 105 + pad, y + 50), themeColor, 150, 50, targets, properties);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + 175 + pad, y + 40 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 200 + pad, y + 50), themeColor, 150, 50, skillValue, properties);

	// Description
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 85),
		DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), height - 110 - pad, description, properties);
	canvas->setActive(false);
}

void GameUIManager::UpdateCharacterBars(Character* _currCharacter)
{
	// Get the ratio from current health and max health
	float healthRatio = _currCharacter->GetHP() / _currCharacter->GetMaxHP();
	// Get the ratio from current mana and max mana
	float manaRatio = _currCharacter->GetMana() / _currCharacter->GetMaxMana();

	// Set the fill of the character's health bar
	mCharacterHudList[_currCharacter->GetHudIndex()]->pHealthBar->setFill(healthRatio);
	// Set the fill of the character's mana bar
	mCharacterHudList[_currCharacter->GetHudIndex()]->pManaBar->setFill(manaRatio);
}

void GameUIManager::UpdateCharacterTurnNumber(Character* _currCharacter, int _turnNumber)
{
	// If the turn number is 666, that means he is dead and the text needs to be set to X
	if (_turnNumber == 666)
		mCharacterHudList[_currCharacter->GetHudIndex()]->pTurnNumber->setText(L"X");
	else
		mCharacterHudList[_currCharacter->GetHudIndex()]->pTurnNumber->setText(std::to_wstring(_turnNumber));
}

void GameUIManager::DecreaseVolume()
{
	// Decrease volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume() - 50);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume()) / 1000.0f;
	mVolumeBar->setFill(volumeRatio);
}

void GameUIManager::IncreaseVolume()
{
	// Increase volume with Red's audio manager
	RedAudioManager::Instance().SetMasterVolume(RedAudioManager::Instance().GetVolume() + 50);

	// Set the fill of the volume bar
	float volumeRatio = static_cast<float>(RedAudioManager::Instance().GetVolume()) / 1000.0f;
	mVolumeBar->setFill(volumeRatio);
}

Odyssey::UICanvas* GameUIManager::CreatePopup(Odyssey::Entity* entity)
{
	UINT windowWidth = 1280;
	UINT windowHeight = 720;
	float x = 910;
	float y = 350;
	UINT width = 360;
	UINT height = 200;
	UINT pad = 10;

	Odyssey::TextProperties title;
	title.bold = true;
	title.italic = false;
	title.fontSize = 30.0f;
	title.textAlignment = Odyssey::TextAlignment::Center;
	title.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	title.fontName = L"Tw Cen MT Condensed";

	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 16.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
	properties.fontName = L"Tw Cen MT Condensed";

	Odyssey::UICanvas* canvas = entity->addComponent<Odyssey::UICanvas>();

	// Background and Separators
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y), DirectX::XMFLOAT4(50.5f, 50.5f, 50.5f, 0.75f), width, height);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 50), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 10);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 100), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 10);

	// Title Text and Icons
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 50, y), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), width - 100, 50, L"Basic Attack", title);
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x, y), L"assets/images/Paladin_Skill_1.png", 50, 50);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x + width - 50, y), DirectX::XMFLOAT4(50.0f, 50.0f, 50.0f, 1.0f), 50, 50);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + width - 50, y), DirectX::XMFLOAT4(0.0f, 122.5f, 122.5f, 1.0f), 50, 50, L"25", title);

	// Skill Info
	properties.italic = true;
	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + pad, y + 60 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 25 + pad, y + 70), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 150, 50, L"Attack", properties);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + 100 + pad, y + 60 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 125 + pad, y + 70), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 150, 50, L"Targets: 1", properties);

	canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x + 225 + pad, y + 60 + pad), L"assets/images/Sword.png", 20, 20);
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 250 + pad, y + 70), DirectX::XMFLOAT4(255.0f, 203.0f, 31.0f, 1.0f), 150, 50, L"Damage: 15", properties);

	// Description
	canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 115), DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f), width - (2 * pad), height - 110 - pad, L"Description: Strikes a single enemy with 100 % chance to apply provoke.", properties);
	canvas->setActive(false);

	return canvas;
}
