#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/audiothread.h"
#include "coreengine/mainapp.h"
#include "objects/base/label.h"

ObjectManager::ObjectManager()
    : RessourceManagement<ObjectManager>("/objects/res.xml", "")
{
    setObjectName("ObjectManager");
    Interpreter::setCppOwnerShip(this);
    loadRessources("/cursor/res.xml");
}

oxygine::spButton ObjectManager::createButton(QString text, qint32 width, QString tooltip, QString resAnim)
{
    oxygine::spButton pButton = oxygine::spButton::create();
    pButton->setResAnim(ObjectManager::getInstance()->getResAnim(resAnim));
    pButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    //Create Actor with Text and add it to button as child
    spLabel textField = spLabel::create(width - 10);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    textField->setStyle(style);
    textField->setHtmlText(text);
    if (width < 0)
    {
        width = textField->getTextRect().getWidth() + 10;
        if (!Settings::getSmallScreenDevice())
        {
            if (width < 180)
            {
                width = 180;
            }
        }
    }
    textField->setWidth(width - 10);
    textField->setPosition(width / 2 - textField->getTextRect().getWidth() / 2, 5);
    if (textField->getX() < 5)
    {
        textField->setX(5);
    }
    textField->setTooltipText(tooltip);
    pButton->setSize(width, 40);
    oxygine::spClipRectActor clipRect = oxygine::spClipRectActor::create();
    clipRect->setSize(pButton->getSize());
    textField->setSize(pButton->getSize());
    clipRect->addChild(textField);
    pButton->addChild(clipRect);

    oxygine::Sprite* ptr = pButton.get();
    pButton->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButton->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        Mainapp::getInstance()->getAudioThread()->playSound("button.wav");
    });
    return pButton;
}


oxygine::spButton ObjectManager::createIconButton(QString icon, qint32 size)
{
    oxygine::spButton pButton = oxygine::spButton::create();
    oxygine::ResAnim* pAnim = ObjectManager::getInstance()->getResAnim("button_square");
    pButton->setResAnim(pAnim);
    pButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    pButton->setSize(size, size);

    oxygine::spSprite pSprite = oxygine::spSprite::create();
    pAnim = ObjectManager::getInstance()->getResAnim(icon);
    pSprite->setResAnim(pAnim);
    pSprite->setPosition((size - pSprite->getWidth()) / 2,
                         (size - pSprite->getHeight()) / 2);
    pButton->addChild(pSprite);

    oxygine::Sprite* ptr = pButton.get();
    pButton->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButton->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        Mainapp::getInstance()->getAudioThread()->playSound("button.wav");
    });
    return pButton;
}

oxygine::spButton ObjectManager::createIconButton(oxygine::spSprite pSprite, qint32 size)
{
    oxygine::spButton pButton = oxygine::spButton::create();
    pButton->setResAnim(ObjectManager::getInstance()->getResAnim("button_square"));
    pButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    pButton->setSize(size, size);
    pSprite->setPosition((size - pSprite->getWidth()) / 2,
                         (size - pSprite->getHeight()) / 2);
    pButton->addChild(pSprite);

    oxygine::Sprite* ptr = pButton.get();
    pButton->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButton->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        Mainapp::getInstance()->getAudioThread()->playSound("button.wav");
    });
    return pButton;
}
