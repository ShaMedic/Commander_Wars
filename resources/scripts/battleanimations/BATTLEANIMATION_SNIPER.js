var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "ma"))
        {
            armyName = "os";
        }
        var offset = Qt.point(-35, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-5, 5);
        }
        sprite.loadSpriteV2("sniper+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                          BATTLEANIMATION_SNIPER.getMaxUnitCount(), offset);
    };

    this.getPositionOffset = function(sprite, unit, terrain, unitIdx)
    {
        if (terrain !== null)
        {
            if (terrain.getID() === "MOUNTAIN")
            {
                if (unitIdx >= 4)
                {
                    return Qt.point(-20 * (6 - unitIdx), 0);
                }
            }
        }
        return Qt.point(0, 0);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_SNIPER.getMaxUnitCount());
        BATTLEANIMATION_SNIPER.loadStandingAnimation(sprite, unit, defender, weapon);
        // mg
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        var offset = Qt.point(31, 31);
        if (armyName === "ma")
        {
            offset = Qt.point(31, 12);
        }
        sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                        1, 1, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_SNIPER.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_SNIPER.getMaxUnitCount());
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 800 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_SNIPER.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_SNIPER = new Constructor();
