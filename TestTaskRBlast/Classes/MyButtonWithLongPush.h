#pragma once

#include "MyButton.h"
#include <chrono>

namespace myGUI
{
	class MyButtonWithLongPush : public MyButton
	{
	public:
		static MyButtonWithLongPush* create(cocos2d::Size contentSize, cocos2d::Node* idleSprite, cocos2d::Node* pushedSprite, cocos2d::Node* dragOutSprite, cocos2d::Node* pushedLongSprite);
		bool init(cocos2d::Size contentSize, cocos2d::Node* idleSprite, cocos2d::Node* pushedSprite, cocos2d::Node* dragOutSprite, cocos2d::Node* pushedLongSprite);

		void setLongPushReleaseCallback(std::function<void()> func) { m_longPushReleaseCallback = func; };

	protected:
		std::function<void()> m_longPushReleaseCallback = nullptr;

		virtual void onMousePress(cocos2d::EventMouse* mouseEv)			override;
		virtual void onMouseRelease(cocos2d::EventMouse* mouseEv)		override;
		virtual void onMouseEnter(cocos2d::EventMouse* mouseEv)			override;
		virtual void onMouseLeftExpandZone(cocos2d::EventMouse* mouseEv)override;
		virtual void update(float delta)								override;

		

	private:
		std::chrono::steady_clock::time_point	m_lastPushedTime;
		std::chrono::milliseconds				m_pushlongTimeout = std::chrono::milliseconds(500);

	};

}
