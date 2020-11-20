#pragma once
#include "cocos2d.h"
#include <functional>
#include <map>
#include <vector>

namespace myGUI
{		
	// для получения события отпускания, слушать кастомное событие MyButtonReleased MyButtonLongPressReleased
	class MyButton : public cocos2d::Node
	{	
	public:
		enum class ButtonState
		{
			Idle,
			Pushed,
			PushedLong,
			DragOut
		};

		static MyButton* create(cocos2d::Size contentSize, cocos2d::Node* idleSprite, cocos2d::Node* pushedSprite, cocos2d::Node* dragOutSprite);
		bool init(cocos2d::Size contentSize, cocos2d::Node* idleSprite, cocos2d::Node* pushedSprite, cocos2d::Node* dragOutSprite);
		void bindChildToState(cocos2d::Node*, ButtonState bs);
		
		void setExpandZoneOffset(float offset);
		void setSafeZoneOffset(float offset);
		void setReleaseCallback(std::function<void()> func) { m_releaseCallback = func; };

		ButtonState getCurrentButtonState() { return m_currentState; };

	protected:
		float m_expandZoneOffset		= 10.f;					// отступ от границ contentSize
		float m_safeZoneOffset			= 10.f;					// отступ от expandZone
		bool m_isCursorOnContent		= false;				// для индикации входа и выхода курсора в зону Expand
		bool m_isCursorOnSafeZone		= false;				// для индикации входа и выхода курсора в зону Safe
		ButtonState m_currentState		= ButtonState::Idle;
		std::map<ButtonState, std::vector<cocos2d::Node*>> m_childBinds = {};
		std::function<void()> m_releaseCallback = nullptr;


		void setState(ButtonState);
		bool isMouseOnZone(cocos2d::Point cursorPos, float zoneOffset = 0.f);


		virtual void onMousePress(cocos2d::EventMouse* mouseEv);
		virtual void onMouseRelease(cocos2d::EventMouse* mouseEv);
		virtual void onMouseMove(cocos2d::EventMouse* mouseEv);

		virtual void onMouseEnter(cocos2d::EventMouse* mouseEv);
		virtual void onMouseLeftSafeZone(cocos2d::EventMouse* mouseEv);
		virtual void onMouseLeftExpandZone(cocos2d::EventMouse* mouseEv);

	};

}