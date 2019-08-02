var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESERT_PIPELINE.getName());
    };
    this.getName = function()
    {
        return qsTr("Desert Pipeline");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PIPELINE,WELD,DESTROYEDWELD,PIPESTATION,SNOW_PIPELINE,SNOW_DESTROYEDWELD,SNOW_WELD,DESERT_PIPELINE,DESERT_DESTROYEDWELD,DESERT_WELD", false, false, GameEnums.Directions_Direct, true, true);
        var x = terrain.getX();
        var y = terrain.getY();
        if (map.onMap(x, y + 1))
        {
            var building = map.getTerrain(x, y + 1).getBuilding();
            if (building !== null &&
                building.getBuildingID() === "ZBLACKHOLE_FACTORY" &&
                building.getX() - 1 === x && building.getY() - 4 === y)
            {
                if (surroundings.indexOf("+W") >= 0)
                {
                    surroundings = surroundings.replace("+W", "+S+W");
                }
                else
                {
                    surroundings += "+S";
                }
            }
        }

        if (surroundings === "")
        {
            terrain.loadBaseSprite("desert_pipeline+E+W");
        }
        else
        {
            terrain.loadBaseSprite("desert_pipeline" + surroundings);
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_pipeline";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_pipeline";
    };
    this.getDescription = function()
    {
        return qsTr("Black Hole Pipeline which can't be crossed by most units. It reduces the firerange of indirect units by 1.");
    };
};
Constructor.prototype = TERRAIN;
var DESERT_PIPELINE = new Constructor();