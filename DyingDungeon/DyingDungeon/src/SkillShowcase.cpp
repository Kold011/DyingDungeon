#include "OdysseyEngine.h"
#include "SkillShowcase.h"

CLASS_DEFINITION(Component, SkillShowcase)

void SkillShowcase::initialize()
{
	
}

void SkillShowcase::update(double deltaTime)
{
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::F))
	{
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = {};

		// Get the prefab's position and rotation
		auto c1Pos = caster1->getComponent<Odyssey::Transform>()->getPosition();
		auto c1Rot = caster1->getComponent<Odyssey::Transform>()->getEulerRotation();
		position = DirectX::XMLoadFloat3(&c1Pos);
		rotation = DirectX::XMLoadFloat3(&c1Rot);

		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(caster1, &caster1Spawn, position, rotation));
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::O))
	{
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = {};

		// Get the prefab's position and rotation
		auto g1Pos = ganfaul1->getComponent<Odyssey::Transform>()->getPosition();
		auto g1Rot = ganfaul1->getComponent<Odyssey::Transform>()->getEulerRotation();
		position = DirectX::XMLoadFloat3(&g1Pos);
		rotation = DirectX::XMLoadFloat3(&g1Rot);

		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(ganfaul1, &ganfaul1Spawn, position, rotation));
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::I))
	{
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = {};

		// Get the prefab's position and rotation
		auto s1Pos = skeleton1->getComponent<Odyssey::Transform>()->getPosition();
		auto s1Rot = skeleton1->getComponent<Odyssey::Transform>()->getEulerRotation();
		position = DirectX::XMLoadFloat3(&s1Pos);
		rotation = DirectX::XMLoadFloat3(&s1Rot);

		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(skeleton1, &skeleton1Spawn, position, rotation));

		auto s2Pos = skeleton2->getComponent<Odyssey::Transform>()->getPosition();
		auto s2Rot = skeleton2->getComponent<Odyssey::Transform>()->getEulerRotation();
		position = DirectX::XMLoadFloat3(&s2Pos);
		rotation = DirectX::XMLoadFloat3(&s2Rot);

		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(skeleton2, &skeleton2Spawn, position, rotation));
	}
	
	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::Y))
	{
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		auto b1Pos = bard1->getComponent<Odyssey::Transform>()->getPosition();
		position = DirectX::XMLoadFloat3(&b1Pos);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(bard1, &bard1Spawn, position, rotation));

		auto b2Pos = bard2->getComponent<Odyssey::Transform>()->getPosition();
		position = DirectX::XMLoadFloat3(&b2Pos);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(bard2, &bard2Spawn, position, rotation));

		auto b3Pos = bard3->getComponent<Odyssey::Transform>()->getPosition();
		position = DirectX::XMLoadFloat3(&b3Pos);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(bard3, &bard3Spawn, position, rotation));

		auto b4Pos = bard4->getComponent<Odyssey::Transform>()->getPosition();
		position = DirectX::XMLoadFloat3(&b4Pos);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(bard4, &bard4Spawn, position, rotation));
	}

	if (Odyssey::InputManager::getInstance().getKeyPress(KeyCode::T))
	{
		// Placeholder for spawn position and rotation
		DirectX::XMVECTOR position = {};
		DirectX::XMVECTOR rotation = {};

		// Get the prefab's position and rotation
		auto p1Pos = paladin1->getComponent<Odyssey::Transform>()->getPosition();
		auto p1Rot = paladin1->getComponent<Odyssey::Transform>()->getEulerRotation();
		position = DirectX::XMLoadFloat3(&p1Pos);
		rotation = DirectX::XMLoadFloat3(&p1Rot);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(paladin1, &paladin1Spawn, position, rotation));

		// Get the prefab's position and rotation
		auto p2Pos = paladin2->getComponent<Odyssey::Transform>()->getPosition();
		auto p2Rot = paladin2->getComponent<Odyssey::Transform>()->getEulerRotation();
		position = DirectX::XMLoadFloat3(&p2Pos);
		rotation = DirectX::XMLoadFloat3(&p2Rot);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(paladin2, &paladin2Spawn, position, rotation));

		// Get the prefab's position and rotation
		auto p3Pos = paladin3->getComponent<Odyssey::Transform>()->getPosition();
		auto p3Rot = paladin3->getComponent<Odyssey::Transform>()->getEulerRotation();
		position = DirectX::XMLoadFloat3(&p3Pos);
		rotation = DirectX::XMLoadFloat3(&p3Rot);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(paladin3, &paladin3Spawn, position, rotation));

		// Get the prefab's position and rotation
		auto p4Pos = paladin4->getComponent<Odyssey::Transform>()->getPosition();
		auto p4Rot = paladin4->getComponent<Odyssey::Transform>()->getEulerRotation();
		position = DirectX::XMLoadFloat3(&p4Pos);
		rotation = DirectX::XMLoadFloat3(&p4Rot);
		Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(paladin4, &paladin4Spawn, position, rotation));
	}
}

std::shared_ptr<Odyssey::Component> SkillShowcase::clone() const
{
	return std::make_shared<SkillShowcase>(*this);
}

void SkillShowcase::onDestroy()
{
}
