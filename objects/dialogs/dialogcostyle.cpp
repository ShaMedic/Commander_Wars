#include <QFile>

#include "objects/dialogs/dialogcostyle.h"

#include "coreengine/mainapp.h"
#include "coreengine/userdata.h"
#include "coreengine/globalutils.h"
#include "coreengine/console.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/cospritemanager.h"

#include "spritingsupport/spritecreator.h"

DialogCOStyle::DialogCOStyle(QString coid)
    : QObject(),
      m_currentCOID(coid)
{
    setObjectName("DialogCOStyle");
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    m_pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_pOkButton = pObjectManager->createButton(tr("Apply"), 150);
    m_pOkButton->setPosition(Settings::getWidth() - m_pOkButton->getWidth() - 30, Settings::getHeight() - 10 - m_pOkButton->getHeight());
    m_pSpriteBox->addChild(m_pOkButton);
    m_pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [this, pCOSpriteManager](oxygine::Event* event)
    {
        event->stopPropagation();
        Userdata::getInstance()->addCOStyle(m_currentCOID, m_ResFilePath, m_colorTable, m_maskTable, m_useColorBox);
        emit pCOSpriteManager->sigLoadResAnim(m_currentCOID, m_ResFilePath, m_colorTable, m_maskTable, m_useColorBox);
        emit sigFinished();
    });

    // cancel button
    oxygine::spButton pExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    pExitButton->setPosition(30, Settings::getHeight() - 10 - pExitButton->getHeight());
    m_pSpriteBox->addChild(pExitButton);
    pExitButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigCancel();
    });
    qint32 heigth = Settings::getHeight() - 320;
    if (Settings::getSmallScreenDevice())
    {
        heigth = Settings::getHeight() - 100;
    }
    qint32 width = Settings::getWidth() / 2 - 80;
    qint32 pixelSize = width;
    if (pixelSize > heigth)
    {
        pixelSize = heigth;
    }
    pixelSize = pixelSize / 256;
    if (pixelSize > 2)
    {
        pixelSize = 2;
    }
    qint32 colorSelectorWidth = 0;
    if (!Settings::getSmallScreenDevice())
    {
        m_pColorSelector = spColorSelector::create(Qt::white, pixelSize);
        m_pColorSelector->setPosition(30, 30);
        m_pSpriteBox->addChild(m_pColorSelector);
        connect(m_pColorSelector.get(), &ColorSelector::sigSelecetedColorChanged, this, &DialogCOStyle::selecetedColorChanged, Qt::QueuedConnection);
        colorSelectorWidth = m_pColorSelector->getWidth();
    }
    QSize size(Settings::getWidth() - colorSelectorWidth - 75, heigth);
    m_pCOPanel = spPanel::create(true, size, size);
    m_pCOPanel->setPosition(Settings::getWidth() - size.width() - 30, 30);
    m_pSpriteBox->addChild(m_pCOPanel);


    auto currentStyle = Userdata::getInstance()->getCOStyle(m_currentCOID);
    QString styleID = "";
    if (currentStyle != nullptr)
    {
        styleID = std::get<1>(*currentStyle);
        styleID = styleID.toLower().remove(0, styleID.indexOf(m_currentCOID.toLower()));
        styleID = styleID.replace(m_currentCOID.toLower(), "");
    }
    m_Styles = pCOSpriteManager->getCOStyles(coid);
    m_Styles.push_front("");

    qint32 currentStyleId = 0;
    for (qint32 i = 0; i < m_Styles.size(); i++)
    {
        if (styleID == m_Styles[i])
        {
            addCOStyle(m_Styles[i], true);
            currentStyleId = i;
        }
        else
        {
            addCOStyle(m_Styles[i], false);
        }
    }

    if (!Settings::getSmallScreenDevice())
    {
        oxygine::spTextField textField = oxygine::spTextField::create();
        oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;
        textField->setStyle(style);
        textField->setHtmlText(tr("Predefined Styles"));
        textField->setPosition(Settings::getWidth() / 2 - 10 - textField->getTextRect().getWidth(),  Settings::getHeight()  - 10 - m_pOkButton->getHeight());
        m_pSpriteBox->addChild(textField);
    }
    connect(this, &DialogCOStyle::sigCOStyleChanged, this, &DialogCOStyle::changeCOStyle, Qt::QueuedConnection);

    QSize size2(Settings::getWidth() - 60, 100);
    m_pPixelPanel = spPanel::create(true, size2, size2);
    m_pPixelPanel->setPosition(30, Settings::getHeight() - 175);
    if (Settings::getSmallScreenDevice())
    {
        m_pPixelPanel->setVisible(false);
    }
    m_pSpriteBox->addChild(m_pPixelPanel);

    changeCOStyle(currentStyleId);

    if (currentStyle != nullptr)
    {
        m_maskTable = std::get<3>(*currentStyle);
        for (qint32 i = 0; i < m_maskTable.width(); i++)
        {
            if (i < m_Pixels.size())
            {
                QColor color = m_maskTable.pixelColor(i, 0);
                m_Pixels[i]->setColor(color.red(), color.green(), color.blue(), 255);
            }
        }
    }
    updateSprites();
    connect(this, &DialogCOStyle::sigCancel, this, &DialogCOStyle::remove, Qt::QueuedConnection);
    connect(this, &DialogCOStyle::sigFinished, this, &DialogCOStyle::remove, Qt::QueuedConnection);
    pApp->continueRendering();
}

void DialogCOStyle::remove()
{
    m_pCOBoxes.clear();
    m_Pixels.clear();
    detach();
}

void DialogCOStyle::selecetedColorChanged(QColor color)
{
    if (m_currentPixel < m_maskTable.width())
    {
        m_maskTable.setPixelColor(m_currentPixel, 0, color);
        m_Pixels[m_currentPixel]->setColor(color.red(), color.green(), color.blue(), 255);
        updateSprites();
    }
}

void DialogCOStyle::changeCOStyle(qint32 index)
{
    if (index != m_CurrentIndex)
    {
        COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
        for (qint32 i = 0; i < m_Styles.size(); i++)
        {
            oxygine::ResAnim* pAnim = pCOSpriteManager->oxygine::Resources::getResAnim((m_currentCOID + m_Styles[i] + "+nrm"));
            m_pCOSprites[i]->setResAnim(pAnim);
        }
        m_CurrentIndex = index;
        if (m_pPredefinedStyles.get() != nullptr)
        {
            m_pPredefinedStyles->detach();
            m_pPredefinedStyles = nullptr;
        }
        oxygine::ResAnim* pAnim = pCOSpriteManager->oxygine::Resources::getResAnim((m_currentCOID + "+nrm"));
        QString filePath = GlobalUtils::makePathRelative(pAnim->getResPath());
        QString style = m_Styles[index];
        filePath = filePath.replace("+nrm.png", "");
        m_ResFilePath = filePath + style;
        CONSOLE_PRINT("Using res file path: " + m_ResFilePath, Console::eDEBUG);
        QFile file(Settings::getUserPath() + m_ResFilePath + "+table.png");
        if (!file.exists())
        {
            file.setFileName(oxygine::Resource::RCC_PREFIX_PATH + m_ResFilePath + "+table.png");
        }
        if (file.exists())
        {
            m_useColorBox = false;
            QStringList items;
            m_baseColorTable.load(file.fileName());
            m_colorTable = m_baseColorTable.copy(0, 0, m_baseColorTable.width(), 1);
            m_maskTable = m_baseColorTable.copy(0, 0, m_baseColorTable.width(), 1);
            for (qint32 i = 0; i < m_baseColorTable.height(); i++)
            {
                items.append(tr("CO Style ") + QString::number(i));
            }
            m_pPredefinedStyles = spDropDownmenu::create(200, items);
            m_pSpriteBox->addChild(m_pPredefinedStyles);
        }
        else
        {
            m_useColorBox = true;
            QImage src;
            if (QFile::exists(Settings::getUserPath() + m_ResFilePath + "+nrm.png"))
            {
                src = QImage(Settings::getUserPath() + m_ResFilePath + "+nrm.png");
            }
            else
            {
                src = QImage(oxygine::Resource::RCC_PREFIX_PATH + m_ResFilePath + "+nrm.png");
            }
            m_baseColorTable = SpriteCreator::createColorTable(src);
            m_colorTable = m_baseColorTable.copy(0, 0, m_baseColorTable.width(), 1);
            m_maskTable = m_baseColorTable.copy(0, 0, m_baseColorTable.width(), 1);
            QStringList items;
            items.append(tr("CO Style ") + QString::number(0));
            m_pPredefinedStyles = spDropDownmenu::create(200, items);
            m_pSpriteBox->addChild(m_pPredefinedStyles);
        }
        if (Settings::getSmallScreenDevice())
        {
            m_pPredefinedStyles->setPosition(Settings::getWidth() / 2 - m_pPredefinedStyles->getWidth() / 2, Settings::getHeight() - 10 - m_pOkButton->getHeight());
        }
        else
        {
            m_pPredefinedStyles->setPosition(Settings::getWidth() / 2 + 10, Settings::getHeight() -  10 - m_pOkButton->getHeight());
        }
        connect(m_pPredefinedStyles.get(), &DropDownmenu::sigItemChanged, this, [this](qint32 item)
        {
            m_maskTable = m_baseColorTable.copy(0, item, m_baseColorTable.width(), 1);
            if (!Settings::getSmallScreenDevice())
            {
                for (qint32 i = 0; i < m_maskTable.width(); i++)
                {
                    if (i < m_Pixels.size())
                    {
                        QColor color = m_maskTable.pixelColor(i, 0);
                        m_Pixels[i]->setColor(color.red(), color.green(), color.blue(), 255);
                    }
                }
            }
            updateSprites();
        });
        if (!Settings::getSmallScreenDevice())
        {
            m_pPixelPanel->clearContent();
            m_Pixels.clear();
            qint32 y = 10;
            qint32 xStep = 0;

            m_PixelsSelector = oxygine::spColorRectSprite::create();
            m_PixelsSelector->setColor(QColor(32, 200, 32, 255));
            m_PixelsSelector->setSize(20, 20);
            m_PixelsSelector->setPosition(xStep * 22 - 2 + 5, 10 - 2);
            m_PixelsSelector->setPriority(-1);
            m_pPixelPanel->addItem(m_PixelsSelector);
            for (qint32 i = 0; i < m_maskTable.width(); i++)
            {
                oxygine::spColorRectSprite pixel = oxygine::spColorRectSprite::create();
                pixel->setSize(16, 16);
                QColor color = m_maskTable.pixelColor(i, 0);
                pixel->setColor(color.red(), color.green(), color.blue(), 255);
                pixel->setPosition(xStep * 22 + 5, y);
                oxygine::Actor* pActor = m_PixelsSelector.get();
                pixel->addClickListener([this, pActor, xStep, y, i](oxygine::Event* pEvent)
                {
                    pEvent->stopPropagation();
                    pActor->setPosition(xStep * 22 - 2 + 5, y - 2);
                    m_currentPixel = i;
                });

                m_pPixelPanel->addItem(pixel);
                m_Pixels.append(pixel);
                xStep++;
                if (xStep * 22 + 60 + 5 > Settings::getWidth() - 100)
                {
                    xStep = 0;
                    y += 22;
                }
                m_pPixelPanel->setContentHeigth(y + 40);
            }
        }
    }
}

void DialogCOStyle::addCOStyle(QString style, bool select)
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
    oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
    pBox->setResAnim(pAnim);
    pAnim = pCOSpriteManager->oxygine::Resources::getResAnim(m_currentCOID + style + "+nrm", oxygine::ep_ignore_error);
    if (pAnim != nullptr)
    {
        float scale = (m_pCOPanel->getHeight() - 100) / pAnim->getHeight();
        pBox->setSize(scale * pAnim->getWidth() + 20, scale * pAnim->getHeight() + 40);
        pBox->setPosition(m_boxWidth, 10);
        m_boxWidth += pBox->getWidth() + 10;
        pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
        m_pCOPanel->addItem(pBox);
        // add some event handling :)
        qint32 index = m_pCOBoxes.size();
        auto* pPtrBox = pBox.get();
        pBox->addEventListener(oxygine::TouchEvent::CLICK, [this, pPtrBox, index](oxygine::Event*)
        {
            for (qint32 i = 0; i < m_pCOBoxes.size(); i++)
            {
                if (m_pCOBoxes[i].get() != pPtrBox)
                {
                    m_pCOBoxes[i]->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
                }
            }
            pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
            emit sigCOStyleChanged(index);
        });
        if (select)
        {
            for (qint32 i = 0; i < m_pCOBoxes.size(); i++)
            {
                if (m_pCOBoxes[i].get() != pPtrBox)
                {
                    m_pCOBoxes[i]->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
                }
            }
            pBox->setAddColor(QColor(32, 200, 32, 0));
        }
        oxygine::spSprite pCO = oxygine::spSprite::create();
        pCO->setResAnim(pAnim);
        pCO->setScale(scale);
        pCO->setPosition(10, 10);
        pBox->addChild(pCO);
        m_pCOPanel->setContentWidth(pBox->getX() + pBox->getWidth() + 50);

        m_pCOSprites.append(pCO);
        m_pCOBoxes.append(pBox);
        m_pResAnims.append(oxygine::spResAnim());
    }
}

void DialogCOStyle::updateSprites()
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pCOSpriteManager->oxygine::Resources::getResAnim((m_currentCOID + m_Styles[m_CurrentIndex] + "+nrm"));
    float scale = pAnim->getScaleFactor();
    m_pResAnims[m_CurrentIndex] = SpriteCreator::createAnim(m_ResFilePath + "+nrm.png", m_colorTable, m_maskTable, m_useColorBox, pAnim->getColumns(), pAnim->getRows(), scale, false);
    m_pCOSprites[m_CurrentIndex]->setResAnim(m_pResAnims[m_CurrentIndex].get());
}
