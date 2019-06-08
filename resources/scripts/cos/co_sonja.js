var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(2);
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/sonja.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);

        audio.clearPlayList();
        CO_SONJA.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);

        audio.clearPlayList();
        CO_SONJA.loadCOMusic(co);
        audio.playRandom();
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "YC";
    };
	
    this.getBonusMisfortune = function(co, unit, posX, posY)
    {
        return 5;
    };

    this.getVisionrangeModifier = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return 2;
            default:
                return 1;
        }
    };
    this.getEnemyTerrainDefenseModifier = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return -3;
            case GameEnums.PowerMode_Power:
                return -2;
            default:
                return -1;
        }
    };

    this.getPerfectVision = function(co)
    {
        // return true if forest and reafs etc are disabled in fog of war see sonja
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return true;
            case GameEnums.PowerMode_Power:
                return true;
            default:
                return false;
        }
    };

    this.getFirstStrike = function(co, unit, posX, posY)
    {
        // sonja's troops get first strike during counter attacks
        // meaning her troops attack before they loose their hp
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return true;
            case GameEnums.PowerMode_Power:
                return false;
            default:
                return false;
        }
    };

    this.getHpHidden = function(co, unit, posX, posY)
    {
        // are the hp hidden of this unit?
        return true;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Kanbei's cool and collected daughter who likes to plan before acting. She excels in gathering information. Sees All, Knows All");
    };
    this.getHits = function()
    {
        return qsTr("Computers");
    };
    this.getMiss = function()
    {
        return qsTr("Bugs");
    };
    this.getCODescription = function()
    {
        return qsTr("Keeps HP intel hidden from foes. Reduces enemy terrain defensive cover by one. However, she suffers from chronic bad luck.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Reduces enemy terrain defensive cover by two. Allows all units to see into woods and reefs.");
    };
    this.getPowerName = function()
    {
        return qsTr("Enhanced Vision");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Reduces enemy terrain defensive cover by three. Allows all units to see into woods and reefs. Units always strike first, even during counterattacks.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Counter Break");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("I see right through your plans!"),
                qsTr("I'll show you there's more to combat than power alone!"),
                qsTr("Number of enemy troops...location... Got it! This is so easy!"),
                qsTr("Get me some fresh intel, stat!"),
                qsTr("You can't hide from me!"),
                qsTr("Just like I planned...")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("My strategy was merely better... That is all."),
                qsTr("I must review my strategy."),
                qsTr("Perfect planning wins the day!")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("... I'm sorry, Father... I've broken my promise..."),
                qsTr("I have learned much from this...")];
    };
    this.getName = function()
    {
        return qsTr("Sonja");
    };
}

Constructor.prototype = CO;
var CO_SONJA = new Constructor();
