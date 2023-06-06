#include "Component.h"
namespace GUI
{

	Component::Component()
		: m_bIsSelected(false)
		, m_bIsActive(false)
	{
	}

	Component::~Component()
	{
	}

	bool Component::isSelected() const
	{
		return m_bIsSelected;
	}

	void Component::select()
	{
		m_bIsSelected = true;
	}

	void Component::deselect()
	{
		m_bIsSelected = false;
	}

	bool Component::isActive() const
	{
		return m_bIsActive;
	}

	void Component::activate()
	{
		m_bIsActive = true;
	}

	void Component::deactivate()
	{
		m_bIsActive = false;
	}

}