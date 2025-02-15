var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(0);
        unit.setMaxAmmo1(0);
        unit.setWeapon1ID("");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setVisionHigh(999);
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(7);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(3);
    };
    this.getBaseCost = function()
    {
        return 20000;
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("transportplane+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
    this.getName = function()
    {
        return qsTr("Transportplane");
    };
    this.createExplosionAnimation = function(x, y, unit, map)
    {
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("explosion+air", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.setSound("explosion+air.wav");
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveair.wav", -2);
        return animation;
    };
    this.transportList = ["ANTITANKCANNON", "APC", "ARTILLERY", "FLAK", "FLARE",
                          "HEAVY_HOVERCRAFT", "HEAVY_TANK", "HOVERCRAFT", "HOVERFLAK",
                          "INFANTRY", "LIGHT_TANK", "MECH", "MEGATANK", "MISSILE",
                          "MOTORBIKE", "NEOTANK", "RECON", "ROCKETTHROWER", "SNIPER",
                          "ZCOUNIT_AT_CYCLE", "ZCOUNIT_CRYSTAL_TANK", "ZCOUNIT_INTEL_TRUCK",
                          "ZCOUNIT_IRON_SHIELD_GENERATOR", "ZCOUNIT_NEOSPIDER_TANK",
                          "ZCOUNIT_REPAIR_TANK", "ZCOUNIT_ROYAL_GUARD",
                          "ZCOUNIT_SIEGE_CANNON", "ZCOUNIT_SMUGGLER",
                          "ZCOUNIT_TANK_HUNTER", "ZCOUNIT_COMMANDO", "ZCOUNIT_PARTISAN",
                          "ZCOUNIT_LOGIC_TRUCK", "ZCOUNIT_CHAPERON", "ZCOUNIT_HOT_TANK",
                          "ZCOUNIT_AUTO_TANK", "ZCOUNIT_RANGER"];
    this.useTerrainDefense = function()
    {
        return false;
    };
    this.useTerrainHide = function()
    {
        return false;
    };
    this.startOfTurn = function(unit, map)
    {
        if (unit.getTerrain() !== null)
        {
            // pay unit upkeep
            var fuelCosts = 5 + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), 5);
            if (fuelCosts < 0)
            {
                fuelCosts = 0;
            }
            unit.setFuel(unit.getFuel() - fuelCosts);
        }
        UNIT.transporterRefilling(unit, map);
    };
    this.getTerrainAnimationBase = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return "base_" + weatherModifier + "air";
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return "";
    };

    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Air unit. </r><div c='#00ff00'>Carries X 2 </div><r>ground units. No attack capability. Units can only be loaded and unloaded on roads or airports.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Air;
    };
}

Constructor.prototype = UNIT;
var TRANSPORTPLANE = new Constructor();
