var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_CANNON");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(2);
        unit.setMaxRange(3);
        unit.setVision(1);

    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("artillery", false);
        unit.loadSprite("artillery+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1);
        animation.loadSprite(unitID + "+walk", false, 1);
        animation.setSound("movetank.wav", -2);
        return animation;
    };
    this.getBaseCost = function()
    {
        return 6000;
    };
    this.getName = function()
    {
        return qsTr("Artillery");
    };
}

Constructor.prototype = UNIT;
var ARTILLERY = new Constructor();
