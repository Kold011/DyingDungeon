#include "Component.h"
#include "Entity.h"
#include <string>

namespace Odyssey
{
	const std::size_t Component::Type = std::hash<std::string>()(TO_STRING(Component));

	bool Component::isClassType(const std::size_t classType) const
	{
		return classType == Type;
	}

	Component::Component()
	{
		mIsActive = true;
		mEntity = nullptr;
	}

	Component::Component(const Component& copy)
	{
		mEntity = copy.mEntity;
		mIsActive = copy.mIsActive;
	}

	Component::~Component()
	{

	}

	void Component::initialize()
	{

	}

	void Component::update(double deltaTime)
	{

	}

	void Component::onDestroy()
	{
	}
	
	Entity* Component::getEntity()
	{
		return mEntity;
	}

	void Component::setEntity(Entity* Entity)
	{
		mEntity = Entity;
	}

	void Component::setActive(bool enable)
	{
		mIsActive = enable;
	}

	bool Component::isActive()
	{
		return mIsActive;
	}
}