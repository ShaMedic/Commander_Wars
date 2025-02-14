#include "network/networkgamedata.h"
#include "network/JsonKeys.h"

#include "coreengine/filesupport.h"

bool NetworkGameData::hasOpenPlayers()
{
    return (m_players < m_maxPlayers);
}

const QString &NetworkGameData::getSlaveAddress() const
{
    return m_slaveAddress;
}

void NetworkGameData::setSlaveAddress(const QString &newAddress)
{
    m_slaveAddress = newAddress;
}

quint16 NetworkGameData::getSlavePort() const
{
    return m_slavePort;
}

void NetworkGameData::setSlavePort(quint16 newPort)
{
    m_slavePort = newPort;
}

QJsonObject NetworkGameData::toJson() const
{
    QJsonObject obj;
    obj.insert(JsonKeys::JSONKEY_VERSION, getVersion());
    obj.insert(JsonKeys::JSONKEY_JOINEDPLAYERS, static_cast<qint64>(m_players));
    obj.insert(JsonKeys::JSONKEY_MAXPLAYERS, static_cast<qint64>(m_maxPlayers));
    obj.insert(JsonKeys::JSONKEY_GAMEDESCRIPTION, m_description);
    obj.insert(JsonKeys::JSONKEY_MAPNAME, m_mapName);
    obj.insert(JsonKeys::JSONKEY_SLAVENAME, m_slaveName);
    obj.insert(JsonKeys::JSONKEY_HASPASSWORD, m_locked);
    QJsonObject mods;
    for (qint32 i = 0; i < m_Mods.size(); ++i)
    {
        mods.insert(JsonKeys::JSONKEY_MOD + QString::number(i), m_Mods[i]);
    }
    obj.insert(JsonKeys::JSONKEY_USEDMODS, mods);
    return obj;
}

void NetworkGameData::fromJson(const QJsonObject & obj)
{
    m_players = obj.value(JsonKeys::JSONKEY_JOINEDPLAYERS).toInteger();
    m_maxPlayers = obj.value(JsonKeys::JSONKEY_MAXPLAYERS).toInteger();
    QJsonObject mods = obj.value(JsonKeys::JSONKEY_USEDMODS).toObject();
    for (const auto & mod : mods)
    {
        m_Mods.append(mod.toString());
    }
    m_description = obj.value(JsonKeys::JSONKEY_GAMEDESCRIPTION).toString();
    m_mapName = obj.value(JsonKeys::JSONKEY_MAPNAME).toString();
    m_slaveName = obj.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    m_locked = obj.value(JsonKeys::JSONKEY_HASPASSWORD).toBool();
}

QString NetworkGameData::getMapName() const
{
    return m_mapName;
}

void NetworkGameData::setMapName(const QString &mapName)
{
    m_mapName = mapName;
}

QString NetworkGameData::getDescription() const
{
    return m_description;
}

void NetworkGameData::setDescription(const QString &value)
{
    m_description = value;
}

QStringList NetworkGameData::getMods() const
{
    return m_Mods;
}

void NetworkGameData::setMods(const QStringList &Mods)
{
    m_Mods = Mods;
}

qint32 NetworkGameData::getMaxPlayers() const
{
    return m_maxPlayers;
}

void NetworkGameData::setMaxPlayers(const qint32 &maxPlayers)
{
    m_maxPlayers = maxPlayers;
}

qint32 NetworkGameData::getPlayers() const
{
    return m_players;
}

void NetworkGameData::setPlayers(const qint32 &players)
{
    m_players = players;
}

QString NetworkGameData::getSlaveName() const
{
    return m_slaveName;
}

void NetworkGameData::setSlaveName(const QString &slaveName)
{
    m_slaveName = slaveName;
}

bool NetworkGameData::getLaunched() const
{
    return m_launched;
}

void NetworkGameData::setLaunched(bool launched)
{
    m_launched = launched;
}

bool NetworkGameData::getLocked() const
{
    return m_locked;
}

void NetworkGameData::setLocked(bool locked)
{
    m_locked = locked;
}
