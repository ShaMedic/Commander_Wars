var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Ship Missile");
    };
    this.damageTable = [["BATTLESHIP", 38],
                        ["CANNONBOAT", 85],
                        ["CRUISER", 55],
                        ["DESTROYER", 25],
                        ["SUBMARINE", 95],
                        ["LANDER", 40],
                        ["BLACK_BOAT", 85],
                        ["AIRCRAFTCARRIER", 38],
                        ["WATERMINE", 400]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_ANTI_SHIP_CRUISER_MISSILE.damageTable, "WEAPON_ANTI_SHIP_CRUISER_MISSILE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_CRUISER_MISSILE = new Constructor();
