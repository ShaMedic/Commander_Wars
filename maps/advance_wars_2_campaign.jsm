var Constructor = function()
{
    this.getDescription = function()
    {
        return qsTr("This is the Advance Wars 2 Normal Campaign.");
    };

    this.getAuthor = function()
    {
        return "Robosturm";
    };

    this.getCampaignName = function()
    {
        return qsTr("Advance Wars 2 Normal Campaign");
    };

    this.getBasicBuildList = function()
    {
        return ["APC", "ARTILLERY", "BATTLESHIP", "BOMBER", "CRUISER", "FIGHTER", "FLAK",
                "HEAVY_TANK", "INFANTRY", "K_HELI", "LANDER", "LIGHT_TANK", "MECH",
                "MISSILE", "RECON", "ROCKETTHROWER", "SUBMARINE", "T_HELI"]
    }
    this.getOSBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
         var campaignVariables = map.getCampaign().getVariables();
        var osNeotanks = campaignVariables.createVariable("osNeotanks");
        if (osNeotanks.readDataBool() === true)
        {
            ret.push("NEOTANK");
        }
        return ret;
    }
    this.getBMBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
        var campaignVariables = map.getCampaign().getVariables();
        var bmNeotanks = campaignVariables.createVariable("bmNeotanks");
        if (bmNeotanks.readDataBool() === true)
        {
            ret.push("NEOTANK");
        }
        return ret;
    }
    this.getYCBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
         var campaignVariables = map.getCampaign().getVariables();
        var ycNeotanks = campaignVariables.createVariable("ycNeotanks");
        if (ycNeotanks.readDataBool() === true)
        {
            ret.push("NEOTANK");
        }
        return ret;
    }
    this.getGEBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
         var campaignVariables = map.getCampaign().getVariables();
        var geNeotanks = campaignVariables.createVariable("geNeotanks");
        if (geNeotanks.readDataBool() === true)
        {
            ret.push("NEOTANK");
        }
        return ret;
    }
    this.getBHBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
        ret.push("NEOTANK");
        return ret;
    }


    this.getCurrentCampaignMaps = function(campaign)
    {
        // set campaign folder
        var ret = ["maps/advance_wars_2_campaign.camp/"]
        // get campaign variables
        // get campaign variables
        var variables = campaign.getVariables();
        // read all  map variables

        // orange star maps
        var CleanupWon = variables.createVariable("Cleanup");
        var Border_SkirmishWon = variables.createVariable("Border Skirmish");
        var Orange_DawnWon = variables.createVariable("Orange Dawn");
        var Flak_AttackWon = variables.createVariable("Flak Attack");
        var Lash_OutWon = variables.createVariable("Lash Out");
        var Andy_s_TimeWon = variables.createVariable("Andy's Time");
        var osLabFound = variables.createVariable("osLabFound");
        var Test_of_TimeWon = variables.createVariable("Test of Time");
        var LiberationWon = variables.createVariable("Liberation");
        if (CleanupWon.readDataBool() === false)
        {
            ret.push("Cleanup.map");
        }
        if (CleanupWon.readDataBool() === true &&
            Border_SkirmishWon.readDataBool() === false)
        {
            ret.push("Border Skirmish.map");
        }
        if (Border_SkirmishWon.readDataBool() === true &&
            Orange_DawnWon.readDataBool() === false)
        {
            ret.push("Orange Dawn.map");
        }
        if (Orange_DawnWon.readDataBool() === true &&
            Flak_AttackWon.readDataBool() === false)
        {
            ret.push("Flak Attack.map");
        }
        if (Orange_DawnWon.readDataBool() === true &&
            Lash_OutWon.readDataBool() === false)
        {
            ret.push("Lash Out.map");
        }

        if (Orange_DawnWon.readDataBool() === true &&
            Andy_s_TimeWon.readDataBool() === false)
        {
            ret.push("Andy's Time.map");
        }
        // check for os lab enable
        if (osLabFound.readDataBool() === true &&
            Test_of_TimeWon.readDataBool() === false &&
            LiberationWon.readDataBool() === false)
        {
            ret.push("Test of Time.map");
        }
        // check for os factory map
        if (Flak_AttackWon.readDataBool() === true &&
            Lash_OutWon.readDataBool() === true &&
            Andy_s_TimeWon.readDataBool() === true &&
            LiberationWon.readDataBool() === false)
        {
            ret.push("Liberation.map");
        }
        if (LiberationWon.readDataBool() === true)
        {
            // blue moon
            var toyboxWon = variables.createVariable("Toy Box");
            var reclamationWon = variables.createVariable("Reclamation");
            var tanksWon = variables.createVariable("Tanks!!!");
            var natureWalkWon = variables.createVariable("Nature Walk");
            var twoWeekTestWon = variables.createVariable("Two Week Test");
            var tminus15Won = variables.createVariable("T Minus 15");
            var neotanksWon = variables.createVariable("Neotanks?!");

            var bmLabFound = variables.createVariable("bmLabFound");
            var blueMoonCount = 0;
            if (toyboxWon.readDataBool() === false)
            {
                ret.push("Toy Box.map");
            }
            else
            {
                blueMoonCount += 1;
            }
            if (reclamationWon.readDataBool() === false)
            {
                ret.push("Reclamation.map");
            }
            else
            {
                blueMoonCount += 1;
            }
            if (tanksWon.readDataBool() === false)
            {
                ret.push("Tanks!!!.map");
            }
            else
            {
                blueMoonCount += 1;
            }
            if (blueMoonCount >= 2)
            {
                // enable the last three missions
                if (natureWalkWon.readDataBool() === false)
                {
                    ret.push("Nature Walk.map");
                }
                else
                {
                    blueMoonCount += 1;
                }
                if (twoWeekTestWon.readDataBool() === false)
                {
                    ret.push("Two Week Test.map");
                }
                else
                {
                    blueMoonCount += 1;
                }
                if (tminus15Won.readDataBool() === false)
                {
                    ret.push("T Minus 15.map");
                }
                else
                {
                    blueMoonCount += 1;
                }
            }
            if (bmLabFound.readDataBool() === true &&
                neotanksWon.readDataBool() === false)
            {
                ret.push("Neotanks?!.map");
            }

            if (blueMoonCount >= 4)
            {
                // enable factory mission
            }

            // yellow comet

        }
        ret.push("Neotanks?!.map");

        return ret;
    };
	
	this.mapFinished = function(campaign, map, result)
    {
        // get campaign variables
		var variables = campaign.getVariables();
        // create a campaign variable if it doesn't exist else we get the existing one
        var mapVar = variables.createVariable(map.getMapName());
        // write the map result won or loose to it.
        mapVar.writeDataBool(result);
    };

    this.getCampaignFinished = function(campaign)
    {

        // not all maps are won so you didn't won the campaign
        return false;
    };

    this.getSelectableCOs = function(campaign, map, player, coIndex)
    {
        if (map.getMapName() === "Liberation" && coIndex === 0)
        {
            return ["CO_ANDY", "CO_MAX", "CO_SAMI"];
        }
        if (map.getMapName() === "Neotanks?!" && coIndex === 0)
        {
            return ["CO_COLIN", "CO_OLAF", "CO_GRIT"];
        }
        // return the co's for a certain map a player [player] and the first or second co [coIndex]
        // make no co selectable
        return [""];
    };
};

Constructor.prototype = BASECAMPAIGN;
var campaignScript = new Constructor();