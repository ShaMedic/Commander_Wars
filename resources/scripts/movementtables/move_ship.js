var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Ship");
    };
    this.movementpointsTable = [["HARBOUR", 1],
                                ["TEMPORARY_HARBOUR", 1],
                                ["OILRIG", 1],
                                ["SEA", 1],
                                ["LAKE", 1],
                                ["FOG", 1],
                                ["ROUGH_SEA", 2],
                                ["REAF", 2],
                                ["TELEPORTTILE", 0]];

    this.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false, map)
    {
        var shipBridges = true;
        if (map !== null)
        {
            shipBridges = map.getGameRules().getShipBridges();
        }
        if (shipBridges &&
            terrain.getID() === "BRIDGE" &&
            (terrain.getBaseTerrainID() === "SEA" || terrain.getBaseTerrainID() === "LAKE"))
        {
            return 1;
        }
        return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_SHIP.movementpointsTable);
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_SHIP = new Constructor();
