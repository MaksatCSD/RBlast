#include "MyButton.h"

USING_NS_CC;

namespace myGUI
{

    MyButton* MyButton::create(const cocos2d::Size& contentSize, cocos2d::Node* idleSprite, cocos2d::Node* pushedSprite, cocos2d::Node* dragOutSprite)
    {
        MyButton* ret = new (std::nothrow) MyButton();
        if (ret && ret->init(contentSize, idleSprite, pushedSprite, dragOutSprite))
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool MyButton::init(const cocos2d::Size& contentSize, cocos2d::Node* idleSprite, cocos2d::Node* pushedSprite, cocos2d::Node* dragOutSprite)
    {
        this->setContentSize(contentSize);
        this->setAnchorPoint({ 0.5f, 0.5f });

        if (idleSprite)
        {
            idleSprite->setPosition(contentSize.width / 2, contentSize.height / 2);
            bindChildToState(idleSprite, ButtonState::Idle);
            this->addChild(idleSprite);
        }
        if (pushedSprite)
        {
            pushedSprite->setPosition(contentSize.width/2, contentSize.height/2);
            bindChildToState(pushedSprite, ButtonState::Pushed);
            this->addChild(pushedSprite);
        }
        if (dragOutSprite)
        {
            dragOutSprite->setPosition(contentSize.width / 2, contentSize.height / 2);
            bindChildToState(dragOutSprite, ButtonState::DragOut);
            this->addChild(dragOutSprite);
        }
        
        auto mouseListener = EventListenerMouse::create();
        mouseListener->onMouseDown  = CC_CALLBACK_1(MyButton::onMousePress, this);
        mouseListener->onMouseUp    = CC_CALLBACK_1(MyButton::onMouseRelease, this);
        mouseListener->onMouseMove  = CC_CALLBACK_1(MyButton::onMouseMove, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);


        return true;
    }

    void MyButton::setExpandZoneOffset(float offset)
    {
        if (offset >= 0) m_expandZoneOffset = offset;
    }

    void MyButton::setSafeZoneOffset(float offset)
    {
        if (offset >= 0) m_safeZoneOffset = offset;
    }

    void MyButton::bindChildToState(cocos2d::Node* node, ButtonState bs)
    {
        m_childBinds[bs].push_back(node);
        if (bs != m_currentState) node->setVisible(false);
    }

    void myGUI::MyButton::setState(ButtonState state)
    {
        // отключаем элементы текущего состояния
        for (auto& n : m_childBinds[m_currentState])
            n->setVisible(false);

        // включаем элементы нового состояния
        for (auto& n : m_childBinds[state])
            n->setVisible(true);

        m_currentState = state;
    }
   
    bool myGUI::MyButton::isMouseOnZone(const cocos2d::Point& cursorPos, float zoneOffset)
    {
        const auto boundBox = getBoundingBox();
        const auto localCursorPos = _parent->convertToNodeSpace(cursorPos);

        const Rect r{ boundBox.origin.x - zoneOffset * _scaleX, boundBox.origin.y - zoneOffset * _scaleZ,
                boundBox.size.width + zoneOffset * 2 * _scaleX, boundBox.size.height + zoneOffset * 2 * _scaleY };

        return r.containsPoint({ localCursorPos.x, localCursorPos.y });
    }


    void myGUI::MyButton::onMousePress(cocos2d::EventMouse* mouseEv)
    {
        if (m_isCursorOnContent)
        {
            setState(ButtonState::Pushed);
        }
    }

    void myGUI::MyButton::onMouseRelease(cocos2d::EventMouse* mouseEv)
    {
        if (m_currentState == ButtonState::Pushed)
        {
            if (m_releaseCallback)
                m_releaseCallback();


            EventCustom event("MyButtonReleased");
            event.setUserData(this);
            _eventDispatcher->dispatchEvent(&event);

            setState(ButtonState::Idle);
        }
        else if (m_currentState == ButtonState::DragOut)
        {
            setState(ButtonState::Idle);
        }
    }

    void myGUI::MyButton::onMouseMove(cocos2d::EventMouse* mouseEv)
    {
        if (m_isCursorOnContent && !isMouseOnZone(mouseEv->getLocationInView(), m_expandZoneOffset))
        {
            onMouseLeftExpandZone(mouseEv);
            m_isCursorOnContent = false;
        }
        else if (!m_isCursorOnContent && isMouseOnZone(mouseEv->getLocationInView(), m_expandZoneOffset))
        {
            onMouseEnter(mouseEv);
            m_isCursorOnContent = true;
        }

        if (m_isCursorOnSafeZone && !isMouseOnZone(mouseEv->getLocationInView(), m_expandZoneOffset + m_safeZoneOffset))
        {
            onMouseLeftSafeZone(mouseEv);
            m_isCursorOnSafeZone = false;
        }
        else if (!m_isCursorOnSafeZone && isMouseOnZone(mouseEv->getLocationInView(), m_expandZoneOffset + m_safeZoneOffset))
        {
            m_isCursorOnSafeZone = true;
        }
    }


    void myGUI::MyButton::onMouseEnter(cocos2d::EventMouse* mouseEv)
    {
       if (m_currentState == ButtonState::DragOut)
            setState(ButtonState::Pushed);
    }

    void myGUI::MyButton::onMouseLeftSafeZone(cocos2d::EventMouse* mouseEv)
    {
        setState(ButtonState::Idle);
    }

    void myGUI::MyButton::onMouseLeftExpandZone(cocos2d::EventMouse* mouseEv)
    {
        if (m_currentState == ButtonState::Pushed)
            setState(ButtonState::DragOut);
    }
}

