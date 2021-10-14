CO_SASHA.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_SASHA.activateSuperpower = function(co, powerMode)
{
    CO_SASHA.activatePower(co, powerMode);
};
CO_SASHA.getSuperPowerDescription = function()
{
    return CO_SASHA.getPowerDescription();
};
CO_SASHA.getSuperPowerName = function()
{
    return CO_SASHA.getPowerName();
};
CO_SASHA.getBonusIncome = function(co, building, income)
{
    if (co.getIsCO0() === true)
    {
        return income * 0.1;
    }
    return 0;
};

CO_SASHA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};

CO_SASHA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
    }
    return 0;
};

CO_SASHA.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action)
{
};
