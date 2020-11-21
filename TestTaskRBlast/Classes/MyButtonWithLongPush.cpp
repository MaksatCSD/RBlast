#include "MyButtonWithLongPush.h"

USING_NS_CC;

namespace myGUI
{

    MyButtonWithLongPush* myGUI::MyButtonWithLongPush::create(cocos2d::Size contentSize, cocos2d::Node* idleSprite, cocos2d::Node* pushedSprite, cocos2d::Node* dragOutSprite, cocos2d::Node* pushedLongSprite)
	{
        MyButtonWithLongPush* ret = new (std::nothrow) MyButtonWithLongPush();
        if (ret && ret->init(contentSize, idleSprite, pushedSprite, dragOutSprite, pushedLongSprite))
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
	}

    bool MyButtonWithLongPush::init(cocos2d::Size contentSize, cocos2d::Node* idleSprite, cocos2d::Node* pushedSprite, cocos2d::Node* dragOutSprite, cocos2d::Node* pushedLongSprite)
    {
        MyButton::init(contentSize, idleSprite, pushedSprite, dragOutSprite);
        if (pushedLongSprite)
        {
            pushedLongSprite->setPosition(contentSize.width / 2, contentSize.height / 2);
            bindChildToState(pushedLongSprite, ButtonState::PushedLong);
            this->addChild(pushedLongSprite);
        }

        this->scheduleUpdate();
        return true;
    }

    void MyButtonWithLongPush::onMousePress(cocos2d::EventMouse* mouseEv)
    {
        MyButton::onMousePress(mouseEv); 

        if (m_currentState == ButtonState::Pushed)
            m_lastPushedTime = std::chrono::steady_clock::now();
    }

    void MyButtonWithLongPush::onMouseRelease(cocos2d::EventMouse* mouseEv)
    {
        MyButton::onMouseRelease(mouseEv);

        if (m_currentState == ButtonState::PushedLong)
        {
            if (m_longPushReleaseCallback)
                m_longPushReleaseCallback();

            EventCustom event("MyButtonReleased");
            event.setUserData(this);
            _eventDispatcher->dispatchEvent(&event);

            setState(ButtonState::Idle);
        }
    }

    void myGUI::MyButtonWithLongPush::onMouseEnter(cocos2d::EventMouse* mouseEv)
    {
        if (m_currentState == ButtonState::DragOut)
        {
            if (std::chrono::steady_clock::now() - m_lastPushedTime >= m_pushlongTimeout)
                setState(ButtonState::PushedLong);
            else
                setState(ButtonState::Pushed);
        }
    }

    void myGUI::MyButtonWithLongPush::onMouseLeftExpandZone(cocos2d::EventMouse* mouseEv)
    {
        MyButton::onMouseLeftExpandZone(mouseEv);

        if (m_currentState == ButtonState::PushedLong)
            setState(ButtonState::DragOut);
    }

    void MyButtonWithLongPush::update(float delta)
    {
        if (m_currentState == ButtonState::Pushed)
        {
            if (std::chrono::steady_clock::now() - m_lastPushedTime >= m_pushlongTimeout)
                setState(ButtonState::PushedLong);
        }
    }



}