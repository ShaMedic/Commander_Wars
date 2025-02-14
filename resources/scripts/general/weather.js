var WEATHER =
{
    getWeatherName : function()
    {

        return "";
    },

    getWeatherTerrainSprite : function(weather, map)
    {
        // only called when the weather is active
        return "";
    },

    getWeatherSymbol : function(weather, map)
    {
        return "";
    },

    getOffensiveModifier : function(weather, map)
    {
        // only called when the weather is active
        return 0;
    },

    getDefensiveModifier : function(weather, map)
    {
        // only called when the weather is active
        return 0;
    },

    getMovementCostModifier : function(weather, unit, terrain, map)
    {
        // only called when the weather is active
        return 0;
    },

    getFirerangeModifier : function(weather, map)
    {
        // only called when the weather is active
        return 0;
    },

    getMinFirerangeModifier : function(weather, map)
    {
        // only called when the weather is active
        return 0;
    },

    getMovementpointModifier : function(weather, unit, terrain, map)
    {
        return 0;
    },

    getVisionrangeModifier : function(weather, map)
    {
        // only called when the weather is active
        return 0;
    },

    getMovementFuelCostModifier: function(weather, unit, fuelCost, map)
    {
        // only called when the weather is active
        return 0;
    },

    activate : function(weather, map)
    {
        // called when the weather starts
    },

    deactivate : function(weather, map)
    {
        // called when the weather ends
    },

    getDefaultWeatherChance : function()
    {
        return 0;
    },
};
