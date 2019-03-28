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

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(1);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(1);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("hoellium" , false);
        unit.loadSprite("hoellium" +"+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_HOELLIUM";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_HOELLIUM_WAIT";
    };
    this.getBaseCost = function()
    {
        return 10000;
    };
    this.getName = function()
    {
        return qsTr("Oozium");
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
        animation.setSound("moveheavytank.wav", -2);
        return animation;
    };
}

Constructor.prototype = UNIT;
var HOELLIUM = new Constructor();
