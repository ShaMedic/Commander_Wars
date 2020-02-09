#include "resource_management/terrainmanager.h"

#include <QFileInfo>

#include <QDirIterator>

#include "coreengine/mainapp.h"

TerrainManager* TerrainManager::m_pInstance = nullptr;

TerrainManager::TerrainManager() : QObject()
{
    // load terrain
    Mainapp* pMainapp = Mainapp::getInstance();
    this->moveToThread(pMainapp->getWorkerthread());
    oxygine::Resources::loadXML("resources/images/terrain/res.xml");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        if (QFile::exists(pMainapp->getSettings()->getMods().at(i) + "/images/terrain/res.xml"))
        {
            oxygine::Resources::loadXML(pMainapp->getSettings()->getMods().at(i) + "/images/terrain/res.xml");
        }
    }
}

void TerrainManager::reset()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (qint32 i = 0; i < m_loadedTerrains.size(); i++)
    {
        pInterpreter->deleteObject(m_loadedTerrains[i]);
    }
    m_loadedTerrains.clear();
}

void TerrainManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/terrain");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/terrain");
    }
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path =  QCoreApplication::applicationDirPath() + "/" + searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator* dirIter = new QDirIterator(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter->hasNext())
        {
            dirIter->next();
            QString file = dirIter->fileInfo().fileName().split(".").at(0);
            if (!m_loadedTerrains.contains(file.toUpper()))
            {
                loadTerrain(file.toUpper());
            }
        }
    }
    m_loadedTerrains.sort();
}

qint32 TerrainManager::getTerrainGroup(qint32 i)
{
    if ((i >= 0) && (i < m_loadedTerrains.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getTerrainGroup";
        QJSValue ret = pInterpreter->doFunction(m_loadedTerrains[i], function1);
        if (ret.isNumber())
        {
            return ret.toInt();
        }
    }
    return 0;
}

bool TerrainManager::existsTerrain(QString TerrainID)
{
    for (qint32 i = 0; i < m_loadedTerrains.size(); i++)
    {
        if (m_loadedTerrains[i] == TerrainID)
        {
            return true;
        }
    }
    return false;
}

bool TerrainManager::loadTerrain(QString TerrainID)
{
    Mainapp* pMainapp = Mainapp::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/terrain");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/terrain");
    }
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + TerrainID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pInterpreter->openScript(file);
            if (!bRet)
            {
                m_loadedTerrains.append(TerrainID);
            }
            bRet = true;
        }
    }
    return bRet;
}

TerrainManager* TerrainManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new TerrainManager();
    }
    return m_pInstance;
}

QString TerrainManager::getTerrainName(qint32 position)
{
    if ((position >= 0) && (position < m_loadedTerrains.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(m_loadedTerrains[position], "getName");
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}

QString TerrainManager::getTerrainName(QString terrainId)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value = pInterpreter->doFunction(terrainId, "getName");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}

QStringList TerrainManager::getTerrainsSorted()
{
    QStringList sortedTerrains;
    QVector<qint32> terrainGroups;
    for (qint32 i = 0; i < getTerrainCount(); i++)
    {
        qint32 terrainGroup = getTerrainGroup(i);
        if (!terrainGroups.contains(terrainGroup))
        {
            terrainGroups.append(terrainGroup);
        }
    }
    for (qint32 i2 = 0; i2 < terrainGroups.size(); i2++)
    {
        for (qint32 i = 0; i < getTerrainCount(); i++)
        {
            if (getTerrainGroup(i) == terrainGroups[i2])
            {
                sortedTerrains.append(getTerrainID(i));
            }
        }
    }
    return sortedTerrains;
}
