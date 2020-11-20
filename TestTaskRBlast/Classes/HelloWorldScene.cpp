/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MyButton.h"
#include "MyButtonWithLongPush.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);



    //------------------------------
    // ДЕМОНСТРАЦИОНННАЯ СЦЕНА
    //------------------------------

    // надпись для демонстрации приема событий: кнопка-отправитель, состояние кнопки на момент отправления
    auto eventDemoLabel = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (eventDemoLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        eventDemoLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - eventDemoLabel->getContentSize().height));

        this->addChild(eventDemoLabel, 1);
    }



    // Левая кнопка
    /////////////////////////////////////////////////////////////////////////////////
    
    // загрузка спрайтов
    auto idleSprite = Sprite::create("idleImage.png");
    auto pushedSprite = Sprite::create("pushedImage.png");
    auto dragOutSprite = Sprite::create("dragOutimage.png");
    {
        if (idleSprite == nullptr)
        {
        }
        else
        {
            idleSprite->setCenterRect({ 23, 23, 4, 4 });
            idleSprite->setContentSize({ 100, 50 });

        }

        if (pushedSprite == nullptr)
        {

        }
        else
        {
            pushedSprite->setCenterRect({ 23, 23, 4, 4 });
            pushedSprite->setContentSize({ 100, 50 });
        }

        if (dragOutSprite == nullptr)
        {

        }
        else
        {
            dragOutSprite->setCenterRect({ 23, 23, 4, 4 });
            dragOutSprite->setContentSize({ 100, 50 });
        }
    }

    // надпись для демонстрации коллбека при отпускании кнопки
    auto callbackDemoLabel = Label::createWithTTF("button callback", "fonts/Marker Felt.ttf", 24);
    callbackDemoLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x - 100, visibleSize.height / 2 + origin.y + 70));
    this->addChild(callbackDemoLabel);

    auto idleLabel = Label::createWithTTF("Idle", "fonts/Marker Felt.ttf", 18);         idleLabel->setPosition(40, 15);
    auto pushedLabel = Label::createWithTTF("Pushed", "fonts/Marker Felt.ttf", 18);     pushedLabel->setPosition(40, 15);
    auto dragoutLabel = Label::createWithTTF("Dragout", "fonts/Marker Felt.ttf", 18);   dragoutLabel->setPosition(40, 15);

    auto mb = myGUI::MyButton::create({ 80 , 30 }, idleSprite, pushedSprite, dragOutSprite);
    this->addChild(mb);

    mb->setExpandZoneOffset(10);
    mb->setSafeZoneOffset(20);
    mb->addChild(idleLabel);    mb->bindChildToState(idleLabel, myGUI::MyButton::ButtonState::Idle);
    mb->addChild(pushedLabel);  mb->bindChildToState(pushedLabel, myGUI::MyButton::ButtonState::Pushed);
    mb->addChild(dragoutLabel); mb->bindChildToState(dragoutLabel, myGUI::MyButton::ButtonState::DragOut);
    
    // назначение коллбэка отпускания
    mb->setReleaseCallback([callbackDemoLabel]()
        {
            if (callbackDemoLabel->isVisible())
                callbackDemoLabel->setVisible(false);
            else
                callbackDemoLabel->setVisible(true);
        });

    mb->setPosition(Vec2(visibleSize.width / 2 + origin.x - 100, visibleSize.height / 2 + origin.y));



    // Левая кнопка
    /////////////////////////////////////////////////////////////////////////////////

    // загрузка спрайтов
    auto idleSprite2 = Sprite::create("idleImage.png");
    auto pushedSprite2 = Sprite::create("pushedImage.png");  
    auto dragOutSprite2 = Sprite::create("dragOutimage.png");    
    auto pushedLongSprite2 = Sprite::create("pushedLongImage.png");    
   
    {
        if (idleSprite2 == nullptr)
        {
        }
        else
        {
            idleSprite2->setCenterRect({ 23, 23, 4, 4 });
            idleSprite2->setContentSize({ 100, 50 });
        }

        if (pushedSprite2 == nullptr)
        {

        }
        else
        {
            pushedSprite2->setCenterRect({ 23, 23, 4, 4 });
            pushedSprite2->setContentSize({ 100, 50 });
        }

        if (dragOutSprite2 == nullptr)
        {

        }
        else
        {
            dragOutSprite2->setCenterRect({ 23, 23, 4, 4 });
            dragOutSprite2->setContentSize({ 100, 50 });
        }

        if (pushedLongSprite2 == nullptr)
        {
        }
        else
        {
            pushedLongSprite2->setCenterRect({ 23, 23, 4, 4 });
            pushedLongSprite2->setContentSize({ 100, 50 });
        }
    }

    auto pushedLongCallbackDemoLabel = Label::createWithTTF("LongPush button callback", "fonts/Marker Felt.ttf", 24);
    pushedLongCallbackDemoLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x + 100, visibleSize.height / 2 + origin.y + 70));
    this->addChild(pushedLongCallbackDemoLabel);

    auto idleLabel2 = Label::createWithTTF("Idle", "fonts/Marker Felt.ttf", 18);         idleLabel2->setPosition(40, 15);
    auto pushedLabel2 = Label::createWithTTF("Pushed", "fonts/Marker Felt.ttf", 18);     pushedLabel2->setPosition(40, 15);
    auto dragoutLabel2 = Label::createWithTTF("Dragout", "fonts/Marker Felt.ttf", 18);   dragoutLabel2->setPosition(40, 15);
    auto longPressLabel2 = Label::createWithTTF("PushedLong", "fonts/Marker Felt.ttf", 18); longPressLabel2->setPosition(40, 15);

    auto mb2 = myGUI::MyButtonWithLongPush::create({ 80 , 30 }, idleSprite2, pushedSprite2, dragOutSprite2, pushedLongSprite2);
    this->addChild(mb2);
    mb2->setExpandZoneOffset(10);
    mb2->setSafeZoneOffset(20);
    mb2->addChild(idleLabel2);    mb2->bindChildToState(idleLabel2, myGUI::MyButton::ButtonState::Idle);
    mb2->addChild(pushedLabel2);  mb2->bindChildToState(pushedLabel2, myGUI::MyButton::ButtonState::Pushed);
    mb2->addChild(dragoutLabel2); mb2->bindChildToState(dragoutLabel2, myGUI::MyButton::ButtonState::DragOut);
    mb2->addChild(longPressLabel2); mb2->bindChildToState(longPressLabel2, myGUI::MyButton::ButtonState::PushedLong);

    // назначение коллбэка отпускания после длинного нажатия
    mb2->setLongPushReleaseCallback([pushedLongCallbackDemoLabel]()
        {
            if (pushedLongCallbackDemoLabel->isVisible())
                pushedLongCallbackDemoLabel->setVisible(false);
            else
                pushedLongCallbackDemoLabel->setVisible(true);
        });

    mb2->setPosition(Vec2(visibleSize.width / 2 + origin.x + 100, visibleSize.height / 2 + origin.y));
    
    
    // Принимаем события кнопок
    auto listener = EventListenerCustom::create("MyButtonReleased", [=](EventCustom* event) {
        myGUI::MyButton* button = static_cast<myGUI::MyButton*>(event->getUserData());
        std::string str = "released ";

        if (button == mb)
            str += "button1 state:";
        else
            str += "button2 state:";

        auto state = button->getCurrentButtonState();
        if (state == myGUI::MyButton::ButtonState::Pushed)
            str += "Pushed";
        else if (state == myGUI::MyButton::ButtonState::PushedLong)
            str += "PusdhedLong";


        eventDemoLabel->setString(str);
        });

    _eventDispatcher->addEventListenerWithFixedPriority(listener, 1);


    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
