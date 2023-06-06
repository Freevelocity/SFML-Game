#include "Container.h"
#include "Foreach.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
	Container::Container()
		: m_oChildren()
		, m_oSelectedChild(-1)
	{
	}


	void Container::Pack(Component::Ptr pComponent)
	{
		m_oChildren.push_back(pComponent);

		if (!HasSelection() && pComponent->isSelectable())
			Select(m_oChildren.size() - 1);
	}

	bool Container::isSelectable() const
	{
		return false;
	}

	void Container::handleEvent(const sf::Event& pEvent)
	{
		// If we have selected a child then give it events
		if (HasSelection() && m_oChildren[m_oSelectedChild]->isActive())
		{
			m_oChildren[m_oSelectedChild]->handleEvent(pEvent);
		}
		else if (pEvent.type == sf::Event::KeyReleased)
		{
			if (pEvent.key.code == sf::Keyboard::W || pEvent.key.code == sf::Keyboard::Up)
			{
				SelectPrevious();
			}
			else if (pEvent.key.code == sf::Keyboard::S || pEvent.key.code == sf::Keyboard::Down)
			{
				SelectNext();
			}
			else if (pEvent.key.code == sf::Keyboard::Return || pEvent.key.code == sf::Keyboard::Space)
			{
				if (HasSelection())
					m_oChildren[m_oSelectedChild]->activate();
			}
		}
	}

	void Container::draw(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
	{
		pStates.transform *= getTransform();

		FOREACH(const Component::Ptr & oChild, m_oChildren)
			pTarget.draw(*oChild, pStates);
	}

	bool Container::HasSelection() const
	{
		return m_oSelectedChild >= 0;
	}

	void Container::Select(std::size_t pIndex)
	{
		if (m_oChildren[pIndex]->isSelectable())
		{
			if (HasSelection())
				m_oChildren[m_oSelectedChild]->deselect();

			m_oChildren[pIndex]->select();
			m_oSelectedChild = pIndex;
		}
	}

	void Container::SelectNext()
	{
		if (!HasSelection())
			return;

		// Search next component that is selectable, wrap around if necessary
		int iNext = m_oSelectedChild;
		do
			iNext = (iNext + 1) % m_oChildren.size();
		while (!m_oChildren[iNext]->isSelectable());

		// Select that component
		Select(iNext);
	}

	void Container::SelectPrevious()
	{
		if (!HasSelection())
			return;

		// Search previous component that is selectable, wrap around if necessary
		int iPrev = m_oSelectedChild;
		do
			iPrev = (iPrev + m_oChildren.size() - 1) % m_oChildren.size();
		while (!m_oChildren[iPrev]->isSelectable());

		// Select that component
		Select(iPrev);
	}
}