var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["ac", "ac"],
                     ["bd", "bd"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];

    this.animationData = [["ac", [Qt.point(33, 21), Qt.point(30, 27), Qt.point(30, 22)]],
                          ["bd", [Qt.point(21, 22), Qt.point(23, 42), Qt.point(23, 37)]],
                          ["bh", [Qt.point(27, 24), Qt.point(28, 41), Qt.point(28, 36)]],
                          ["bm", [Qt.point(27, 20), Qt.point(30, 41), Qt.point(30, 36)]],
                          ["ge", [Qt.point(26, 23), Qt.point(30, 42), Qt.point(30, 37)]],
                          ["ma", [Qt.point(19, 33), Qt.point(45, 26), Qt.point(45, 21)]],
                          ["os", [Qt.point(21, 24), Qt.point(23, 44), Qt.point(23, 39)]],
                          ["pf", [Qt.point(22, 24), Qt.point(26, 47), Qt.point(26, 42)]],
                          ["ti", [Qt.point(25, 30), Qt.point(31, 46), Qt.point(31, 41)]],
                          ["yc", [Qt.point(26, 20), Qt.point(32, 40), Qt.point(32, 35)]],];

    this.getArmyName = function(unit)
    {
        var player = unit.getOwner();
        return Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_LIGHT_TANK.armyData);
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount());
        var armyName = BATTLEANIMATION_LIGHT_TANK.getArmyName(unit);
        var startX = -80;
        BATTLEANIMATION_LIGHT_TANK.loadSprite(sprite, unit, defender, weapon, "+move", -1, Qt.point(startX, 5), Qt.point(65, 0), 600);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(startX - 25, 7),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("tank_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, loops = -1, startPos = Qt.point(-15, 5), movement = Qt.point(0, 0), moveTime = 0)
    {
        var armyName = BATTLEANIMATION_LIGHT_TANK.getArmyName(unit);
        var weaponId = "";
        if (weapon === 1)
        {
            weaponId = "+mg";
        }
        sprite.loadMovingSpriteV2("light_tank+" + armyName + weaponId + ending + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount(), startPos, movement, moveTime, false, loops);
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_LIGHT_TANK.loadSprite(sprite, unit, defender, weapon, "+stop", 1);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount(), Qt.point(-30, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_LIGHT_TANK.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var armyName = BATTLEANIMATION_LIGHT_TANK.getArmyName(unit);
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount());
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_LIGHT_TANK.animationData);
        if (weapon === 0)
        {
            BATTLEANIMATION_LIGHT_TANK.loadSprite(sprite, unit, defender, weapon, "+fire", 1);
            offset = data[0];
            sprite.loadSprite("light_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("tank_shot.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            BATTLEANIMATION_LIGHT_TANK.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                offset = data[1];
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            else
            {
                offset = data[2];
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount());
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_LIGHT_TANK = new Constructor();
