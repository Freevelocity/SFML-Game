#pragma once


#include "Component.h"

#include <vector>
#include <memory>


namespace GUI
{

    class Container : public Component
    {
    public:
        typedef std::shared_ptr<Container> Ptr;


    public:
        Container();

        void Pack(Component::Ptr pComponent);

        virtual bool isSelectable() const;
        virtual void handleEvent(const sf::Event& pEvent);


    private:
        virtual void draw(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;

        bool HasSelection() const;
        void Select(std::size_t pIndex);
        void SelectNext();
        void SelectPrevious();


    private:
        std::vector<Component::Ptr> m_oChildren;
        int	 m_oSelectedChild;
    };

}

