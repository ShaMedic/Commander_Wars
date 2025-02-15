#include "mapselectionview.h"

#include "game/gamemap.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/cospritemanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/userdata.h"
#include "coreengine/globalutils.h"
#include "coreengine/console.h"

#include "objects/base/moveinbutton.h"

#include "game/gamemap.h"
#include "game/gamerecording/gamerecorder.h"

MapSelectionView::MapSelectionView(qint32 mapInfoHeight)
{
    setObjectName("MapSelectionView");
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 0;
    if (Settings::getWidth() / 2 > 400)
    {
        width = 400;
    }
    else if (Settings::getWidth() / 2 < 300)
    {
        width = 300;
    }
    else
    {
        width = Settings::getWidth() / 2;
    }

    m_pMapSelection = spMapSelection::create(Settings::getHeight() - 40, width, "");
    m_pMapSelection->setPosition(10, 10);
    addChild(m_pMapSelection);
    m_pMinimap = spMinimap::create();
    m_pMinimap->setPosition(0, 0);
    m_pMinimap->setScale(2.0f);

    constexpr qint32 buildingInfoHeight = 135;
    QSize size(Settings::getWidth() - width - 80,
               Settings::getHeight() / 2 - buildingInfoHeight);
    if (Settings::getSmallScreenDevice())
    {
        size.setHeight(Settings::getHeight() - buildingInfoHeight - 15);
    }
    m_MinimapPanel = spPanel::create(true, size, size);
    m_MinimapPanel->setPosition(width + 50, 10);
    m_MinimapPanel->addItem(m_pMinimap);
    addChild(m_MinimapPanel);
    if (Settings::getSmallScreenDevice())
    {
        size = QSize(Settings::getWidth() - 100, Settings::getHeight() - 60);
    }
    else
    {
        size = QSize(Settings::getWidth() - width - 100, Settings::getHeight() / 2 - 60);
    }
    if (mapInfoHeight > 0)
    {
        size.setHeight(mapInfoHeight);
    }
    // map info text
    m_MapInfo = spPanel::create(true, size, size);
    if (Settings::getSmallScreenDevice())
    {
        m_MapInfo->setPosition(Settings::getWidth() - 1, 10);
        m_MapInfo->addChild(spMoveInButton::create(m_MapInfo.get(), m_MapInfo->getScaledWidth()));
    }
    else
    {
        m_MapInfo->setPosition(width + 50, Settings::getHeight() / 2 - 100);
    }
    addChild(m_MapInfo);
    qint32 y = 10;

    oxygine::spTextField pTextfield = oxygine::spTextField::create();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, y);
    pTextfield->setHtmlText(tr("Name: "));
    m_MapInfo->addItem(pTextfield);
    m_MapName = oxygine::spTextField::create();
    m_MapName->setStyle(style);
    m_MapName->setPosition(150, y);
    m_MapInfo->addItem(m_MapName);
    y += 40;

    pTextfield = oxygine::spTextField::create();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, y);
    pTextfield->setHtmlText(tr("Author: "));
    m_MapInfo->addItem(pTextfield);
    m_MapAuthor = oxygine::spTextField::create();
    m_MapAuthor->setStyle(style);
    m_MapAuthor->setPosition(150, y);
    m_MapInfo->addItem(m_MapAuthor);
    y += 40;

    pTextfield = oxygine::spTextField::create();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, y);
    pTextfield->setHtmlText(tr("Player: "));
    m_MapInfo->addItem(pTextfield);
    m_MapPlayerCount = oxygine::spTextField::create();
    m_MapPlayerCount->setStyle(style);
    m_MapPlayerCount->setPosition(150, y);
    m_MapInfo->addItem(m_MapPlayerCount);
    y += 40;

    pTextfield = oxygine::spTextField::create();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, y);
    pTextfield->setHtmlText(tr("Description "));
    m_MapInfo->addItem(pTextfield);
    y += 40;

    style.multiline = true;
    m_MapDescription = oxygine::spTextField::create();
    m_MapDescription->setStyle(style);
    m_MapDescription->setWidth(m_MapInfo->getContentWidth() - 80);
    m_MapDescription->setPosition(10, y);
    m_MapInfo->addItem(m_MapDescription);
    y += 40;

    m_pVictoryInfo = oxygine::spActor::create();
    m_pVictoryInfo->setPosition(10, y);
    m_MapInfo->addItem(m_pVictoryInfo);
    loadMapVictoryInfo();
    y += 55 * Userdata::MAX_VICTORY_INFO_PER_MAP;

    m_MapTags = oxygine::spTextField::create();
    m_MapTags->setStyle(style);
    m_MapTags->setWidth(m_MapInfo->getContentWidth() - 80);
    m_MapTags->setPosition(10, y);
    m_MapInfo->addItem(m_MapTags);
    y += 40;

    // building count
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("mapSelectionBuildingInfo");
    m_pBuildingBackground = oxygine::spBox9Sprite::create();
    m_pBuildingBackground->setResAnim(pAnim);
    m_pBuildingBackground->setSize(Settings::getWidth() - width - 100, GameMap::getImageSize() * 1.2f + 32);
    m_pBuildingBackground->setPosition(m_MapInfo->getX(),
                                       m_MapInfo->getY() + m_MapInfo->getHeight() + 20);
    oxygine::TextStyle styleMain16 = oxygine::TextStyle(FontManager::getMainFont16());
    styleMain16.color = FontManager::getFontColor();
    styleMain16.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    styleMain16.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    styleMain16.multiline = false;

    m_contentSlider = oxygine::spSlidingActor::create();
    m_contentSlider->setSize(m_pBuildingBackground->getWidth() - 20, 100);
    m_content = oxygine::spActor::create();
    m_content->setSize(pBuildingSpriteManager->getCount()* (GameMap::getImageSize() + 12), 100);
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        spBuilding building = spBuilding::create(pBuildingSpriteManager->getID(i), nullptr);
        building->updateBuildingSprites(false);
        building->setVisible(false);
        m_content->addChild(building);
        m_BuildingCountSprites.push_back(building);
        oxygine::spTextField pText = oxygine::spTextField::create();
        pText->setHtmlText("0");
        pText->setPosition(2 + i * (GameMap::getImageSize() + 12), 12 + GameMap::getImageSize() * 1.2f);
        pText->setStyle(styleMain16);
        pText->setVisible(false);
        m_content->addChild(pText);
        m_BuildingCountTexts.push_back(pText);
    }
    m_contentSlider->setX(10);
    m_contentSlider->setContent(m_content);
    m_pBuildingBackground->addChild(m_contentSlider);
    addChild(m_pBuildingBackground);

    oxygine::spButton pButtonTop = oxygine::spButton::create();
    pButtonTop->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    pButtonTop->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    oxygine::Sprite* ptr = pButtonTop.get();
    pButtonTop->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    pButtonTop->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButtonTop->setFlippedX(true);
    auto* pContent = m_content.get();
    auto* pContentSlider = m_contentSlider.get();
    pButtonTop->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        if (pContent->getWidth() > pContentSlider->getWidth())
        {
            pContent->setX(pContent->getX() - GameMap::getImageSize());
            if (pContent->getX() + pContent->getWidth() < pContentSlider->getWidth())
            {
                pContent->setX(pContentSlider->getWidth() - pContent->getWidth());
            }
        }
    });
    pButtonTop->setPosition(m_MapInfo->getX() - 25, m_MapInfo->getY() + m_MapInfo->getHeight() + 30);
    addChild(pButtonTop);

    pButtonTop = oxygine::spButton::create();
    pButtonTop->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    pButtonTop->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    auto* ptr2 = pButtonTop.get();
    pButtonTop->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        ptr2->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    pButtonTop->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        ptr2->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButtonTop->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        pContent->setX(pContent->getX() + GameMap::getImageSize());
        if (pContent->getX() > 0)
        {
            pContent->setX(0);
        }
    });
    pButtonTop->setPosition(m_MapInfo->getX() + m_contentSlider->getWidth() + 25, m_MapInfo->getY() + m_MapInfo->getHeight() + 30);
    addChild(pButtonTop);
}

void MapSelectionView::loadCurrentMap()
{
    loadMap(m_currentMapFile, false);
}

void MapSelectionView::loadMap(QFileInfo info, bool fast)
{
    CONSOLE_PRINT("MapSelectionView::loadMap " + info.filePath(), Console::eDEBUG);
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        m_BuildingCountTexts[i]->setVisible(false);
        m_BuildingCountSprites[i]->setVisible(false);
    }
    if (info.exists())
    {
        if (info.isFile() &&
            (info != m_currentMapFile || !fast) &&
            (info.fileName().endsWith(".map") ||
             info.fileName().endsWith(".msav")))
        {
            m_CurrentLoadedCampaign = nullptr;
            if (m_pCurrentMap.get() != nullptr)
            {
                m_pCurrentMap->detach();
                m_pCurrentMap = nullptr;
            }
            bool savegame = info.fileName().endsWith(".msav");
            QString file = info.absoluteFilePath();
            m_pCurrentMap = spGameMap::create(file, true, fast, savegame);
            m_pCurrentMap->setMapPath(GlobalUtils::makePathRelative(file, false));
            m_pCurrentMap->getGameScript()->init();
            m_pMinimap->clear();
            m_pMinimap->updateMinimap(m_pCurrentMap.get());
            m_MinimapPanel->setContentWidth(m_pMinimap->getScaledWidth() + 50);
            m_MinimapPanel->setContentHeigth(m_pMinimap->getScaledHeight() + 50);
            if (m_pCurrentMap.get() != nullptr)
            {
                m_MapName->setHtmlText(m_pCurrentMap->getMapName());
                m_MapAuthor->setHtmlText(m_pCurrentMap->getMapAuthor());
                m_MapPlayerCount->setHtmlText(QString::number(m_pCurrentMap->getPlayerCount()));
                m_MapDescription->setHtmlText(m_pCurrentMap->getMapDescription());
                m_MapTags->setHtmlText(m_pCurrentMap->getMapTagsText());
            }
            m_pVictoryInfo->setY(m_MapDescription->getY() + m_MapDescription->getTextRect().getHeight() + 10);
            m_pVictoryInfo->setVisible(true);
            m_MapTags->setY(m_pVictoryInfo->getY() + 55 * Userdata::MAX_VICTORY_INFO_PER_MAP);

            m_MapPlayerCount->setVisible(true);
            m_currentMapFile = info;
            loadMapVictoryInfo();
            qint32 pos = 0;
            if (m_pCurrentMap.get() != nullptr)
            {
                for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
                {
                    qint32 count = m_pCurrentMap->getBuildingCount(pBuildingSpriteManager->getID(i));
                    if (count > 0)
                    {
                        m_BuildingCountTexts[i]->setHtmlText(QString::number(count));
                        m_BuildingCountTexts[i]->setVisible(true);
                        m_BuildingCountTexts[i]->setPosition(2 + pos * (GameMap::getImageSize() + 12), 12 + GameMap::getImageSize() * 1.2f);
                        spBuilding building = m_BuildingCountSprites[i];
                        building->scaleAndShowOnSingleTile();
                        building->setX(building->oxygine::Actor::getX() + pos * (GameMap::getImageSize() + 12));
                        building->setY(building->oxygine::Actor::getY() + 5 + GameMap::getImageSize() / 2);
                        building->setVisible(true);
                        pos++;
                    }
                }
            }
            m_content->setSize(pos * (GameMap::getImageSize() + 12), 100);
            m_contentSlider->snap();
        }
        else if (info.isFile() && info.fileName().endsWith(".jsm"))
        {
            if (m_pCurrentMap.get() != nullptr)
            {
                m_pCurrentMap->detach();
                m_currentMapFile = QFileInfo();
                m_pCurrentMap = nullptr;
            }
            m_pMinimap->updateMinimap(nullptr);
            m_CurrentLoadedCampaign = nullptr;
            m_CurrentLoadedCampaign = spCampaign::create(info.absoluteFilePath());
            m_MapDescription->setHtmlText(m_CurrentLoadedCampaign->getDescription());
            m_MapAuthor->setHtmlText(m_CurrentLoadedCampaign->getAuthor());
            m_MapPlayerCount->setVisible(false);
            m_MapName->setHtmlText(m_CurrentLoadedCampaign->getName());
            m_pVictoryInfo->setVisible(false);
        }
    }
    else
    {
        if (m_pCurrentMap.get() != nullptr)
        {
            m_pCurrentMap->detach();
            m_pCurrentMap = nullptr;
        }
        m_CurrentLoadedCampaign = nullptr;
        m_currentMapFile = info;
    }
    qint32 maxWidth = m_MapDescription->getX() + m_MapDescription->getTextRect().getWidth();
    if (maxWidth < m_MapAuthor->getX() + m_MapAuthor->getTextRect().getWidth())
    {
        maxWidth = m_MapAuthor->getX() + m_MapAuthor->getTextRect().getWidth();
    }
    if (maxWidth < m_MapName->getX() + m_MapName->getTextRect().getWidth())
    {
        maxWidth = m_MapName->getX() + m_MapName->getTextRect().getWidth();
    }
    m_MapInfo->setContentWidth(maxWidth + 30);
    m_MapInfo->setContentHeigth(m_MapDescription->getY() + m_MapDescription->getTextRect().getHeight() + 30 + Userdata::MAX_VICTORY_INFO_PER_MAP * 55 + m_MapTags->getTextRect().getHeight());
}

void MapSelectionView::loadMapVictoryInfo()
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    m_pVictoryInfo->removeChildren();
    qint32 posY = 0;
    for (qint32 i = 0; i < Userdata::MAX_VICTORY_INFO_PER_MAP; ++i)
    {
        oxygine::spTextField pText = oxygine::spTextField::create();
        pText->setHtmlText(QString::number(i + 1) + ".");
        pText->setPosition(10, posY + 10);
        pText->setStyle(style);
        m_pVictoryInfo->addChild(pText);
        if (m_pCurrentMap.get() != nullptr)
        {
            auto info = Userdata::getInstance()->getVictoryForMap(m_pCurrentMap->getMapPath());
            if (info != nullptr && i < info->co1.size())
            {
                QString co1 = info->co1[i];
                QString co2 = info->co2[i];
                qint32 score = info->score[i];
                oxygine::ResAnim* pAnim = nullptr;
                COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
                oxygine::spSprite pSprite = oxygine::spSprite::create();
                pSprite->setPosition(110, posY + 10);
                auto rank = GameRecorder::getRank(score);
                pAnim = GameRecorder::getRankAnim(rank);
                pSprite->setResAnim(pAnim);
                m_pVictoryInfo->addChild(pSprite);
                pSprite = oxygine::spSprite::create();
                if (!co1.isEmpty())
                {
                    pAnim = pCOSpriteManager->getResAnim(co1 + "+info", oxygine::ep_ignore_error);
                }
                else
                {
                    pAnim = pCOSpriteManager->getResAnim("no_co+info", oxygine::ep_ignore_error);
                }
                if (pAnim == nullptr)
                {
                    pAnim = pCOSpriteManager->getResAnim("no_co+info", oxygine::ep_ignore_error);
                }
                if (pAnim != nullptr)
                {
                    pSprite->setResAnim(pAnim);
                    pSprite->setScale(2.0f * pAnim->getWidth() / 32.0f);
                    pSprite->setPosition(150, posY);
                    m_pVictoryInfo->addChild(pSprite);
                    if (!co2.isEmpty())
                    {
                        pAnim = pCOSpriteManager->getResAnim(co2 + "+info", oxygine::ep_ignore_error);
                    }
                    else
                    {
                        pAnim = pCOSpriteManager->getResAnim("no_co+info", oxygine::ep_ignore_error);
                    }
                    if (pAnim == nullptr)
                    {
                        pAnim = pCOSpriteManager->getResAnim("no_co+info", oxygine::ep_ignore_error);
                    }
                    pSprite = oxygine::spSprite::create();
                    if (pAnim != nullptr)
                    {
                        pSprite->setScale(2.0f * pAnim->getWidth() / 32.0f);
                        pSprite->setResAnim(pAnim);
                        pSprite->setPosition(150, posY + pAnim->getHeight() * 2.0f);
                    }
                    m_pVictoryInfo->addChild(pSprite);
                    pText->setHtmlText(QString::number(i + 1) + ". " + QString::number(score));
                }
            }
        }
        posY += 55;
    }
}

spGameMap MapSelectionView::getCurrentMap() const
{
    return m_pCurrentMap;
}

void MapSelectionView::setCurrentMap(spGameMap newCurrentMap)
{
    m_pCurrentMap = newCurrentMap;
}

spPanel MapSelectionView::getMapInfo() const
{
    return m_MapInfo;
}

oxygine::spBox9Sprite MapSelectionView::getBuildingBackground() const
{
    return m_pBuildingBackground;
}

spPanel MapSelectionView::getMinimapPanel() const
{
    return m_MinimapPanel;
}

const QFileInfo &MapSelectionView::getCurrentMapFile() const
{
    return m_currentMapFile;
}

spMapSelection MapSelectionView::getMapSelection() const
{
    return m_pMapSelection;
}

void MapSelectionView::updateMapData()
{
    m_MapName->setHtmlText(m_pCurrentMap->getMapName());
    m_MapAuthor->setHtmlText(m_pCurrentMap->getMapAuthor());
    m_MapPlayerCount->setHtmlText(QString::number(m_pCurrentMap->getPlayerCount()));
    m_MapDescription->setHtmlText(m_pCurrentMap->getMapDescription());
    m_MapTags->setHtmlText(m_pCurrentMap->getMapTagsText());
    m_pVictoryInfo->setY(m_MapDescription->getY() + m_MapDescription->getTextRect().getHeight() + 10);
    m_MapTags->setY(m_pVictoryInfo->getY() + 55 * Userdata::MAX_VICTORY_INFO_PER_MAP);
}
