#ifndef BUILDINGSPRITEMANAGER_H
#define BUILDINGSPRITEMANAGER_H

#include <qobject.h>
#include "resource_management/ressourcemanagement.h"

class BuildingSpriteManager : public QObject, public RessourceManagement<BuildingSpriteManager>
{
    Q_OBJECT
public slots:
    void removeRessource(QString id);
    /**
     * @brief getLoadedRessources
     * @return
     */
    QStringList getLoadedBuildings()
    {
        return m_loadedRessources;
    }
    /**
     * @brief getCount
     * @return
     */
    qint32 getBuildingCount()
    {
        return m_loadedRessources.size();
    }
protected:
    friend RessourceManagement<BuildingSpriteManager>;
    BuildingSpriteManager();
private:
    virtual ~BuildingSpriteManager() = default;
};

Q_DECLARE_INTERFACE(BuildingSpriteManager, "BuildingSpriteManager");

#endif // BUILDINGSPRITEMANAGER_H
