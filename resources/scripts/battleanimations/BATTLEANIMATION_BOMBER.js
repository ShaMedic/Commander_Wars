var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "ma"],];

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BOMBER.armyData);
        var offset = Qt.point(20, 40);
        var movement = Qt.point(-85, -1);
        var moveTime = 850;
        if (armyName === "ma")
        {
            offset = Qt.point(0, 40);
            movement = Qt.point(0, 0);
            moveTime = 0;
        }
        sprite.loadMovingSpriteV2("bomber+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_BOMBER.getMaxUnitCount(), offset, movement, moveTime);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon, alive = true)
    {
        BATTLEANIMATION_BOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        }
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
    {
        // get army name
        var player = unit.getOwner();        
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BOMBER.armyData);
        var offset = Qt.point(-65, 40);
        if (armyName === "ma")
        {
            offset = Qt.point(0, 40);
        }
        sprite.loadMovingSpriteV2("bomber+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_BOMBER.getMaxUnitCount(), offset, movement, moveTime);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BOMBER.armyData);
        BATTLEANIMATION_BOMBER.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        var startPoint = Qt.point(70, 60);
        if (armyName === "ma")
        {
            startPoint = Qt.point(60, 50);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("bombs_projectile", false, startPoint,
                                          Qt.point(0, -80), 400, false,
                                          1, 1, -1, i * 150);
            sprite.loadSound("falling_bomb.wav", 1, i * 150);
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 1250;
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadSprite("cannon_heavy_hit",  false, 5, Qt.point(0, 60),
                          1, 1.0, 0, 300);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("bomb_falling", false, 5, Qt.point(0, 150),
                                Qt.point(0, -130), 400, true,
                                1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("impact_explosion.wav", 1, 300 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 860;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 2000;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, -140), 1800, false);
        sprite.loadSound("airunit_dying.wav", 1);
    };

};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BOMBER = new Constructor();
