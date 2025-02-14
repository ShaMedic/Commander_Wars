var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt"];
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power10_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power10_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_RATTIGAN.rattiganDamage(co, 1, dialogAnimation, map);
    };

    this.rattiganDamage = function(co, value, animation2, map)
    {
        var player = co.getOwner();
        var counter = 0;
        var playerCounter = map.getPlayerCount();
        var animation = null;
        var animations = [];

        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                var units = enemyPlayer.getUnits();
                units.randomize();
                for (i = 0; i < units.size(); i++)
                {
                    var unit = units.at(i);

                    animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    var delay = globals.randInt(135, 265);
                    if (animations.length < 5)
                    {
                        delay *= i;
                    }
                    animation.setSound("power4.wav", 1, delay);
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        animation2.queueAnimation(animation);
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        animations[counter].queueAnimation(animation);
                        animations[counter] = animation;
                        counter++;
                        if (counter >= animations.length)
                        {
                            counter = 0;
                        }
                    }
                    animation.writeDataInt32(unit.getX());
                    animation.writeDataInt32(unit.getY());
                    animation.writeDataInt32(value);
                    animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
                }
                units.remove();
            }
        }
    };

    this.loadCOMusic = function(co, map)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/rattigan.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "YC";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        var count = CO_RATTIGAN.getUnitCount(co, defPosX, defPosY, map);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (count > 0)
                {
                    return 40 + count * 5 + 10;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (count > 0)
                {
                    return 20 + count * 5 + 10;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (count > 0)
                    {
                        return 50;
                    }
                    return 0;
                }
                break;
        }
        if (count > 0)
        {
            return 5;
        }
        else
        {
            return -10;
        }
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        var count = CO_RATTIGAN.getUnitCount(co, defPosX, defPosY, map);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (count > 0)
                {
                    return 30;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };

    this.getUnitCount = function(co, x, y, map)
    {
        if (x >= 0 && y >= 0)
        {
            var count = 0;
            var fields = globals.getCircle(1, 2);
            var size = fields.size();
            for (var i = 0; i < size; i++)
            {
                var point = fields.at(i);
                if (map.onMap(x + point.x, y + point.y))
                {
                    var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                    if ((unit !== null) &&
                            (co.getOwner().isEnemyUnit(unit)))
                    {
                        count++;
                    }
                }
            }
            fields.remove();
        }
        return count;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_AUTO_TANK"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A dutiful CO of the Yellow Comet army addled by a childhood phobia. Blames Sensei for Yellow Comet's lack of former glory.");
    };
    this.getHits = function(co)
    {
        return qsTr("Yellow Comet");
    };
    this.getMiss = function(co)
    {
        return qsTr("Infestations");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Rattigan's troops are trained to be extremely capable at dealing with large regiments of units, but are taken off guard by lone units.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nAuto Tank\n") +
               qsTr("\nGlobal Effect: \nUnits have increased firepower against large regiments of units and reduced firepower against lone units.") +
               qsTr("\n\nCO Zone Effect: \nUnits have increased firepower against large regiments of units.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Receives a small offensive boost when attacking enemies in groups. Movement is increased by one space.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Raid");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Receives a firepower and defense boost for engaging enemies in groups. Enemy units suffer one HP of damage.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("The Sweep");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("All those enemy men... heh, perfect!"),
                qsTr("G-gah! They're everywhere!"),
                qsTr("Hrumph! This'll learn yeh!"),
                qsTr("Heh, could Sensei do THIS?"),
                qsTr("Who the heck taught you to fight?"),
                qsTr("Stay on your toes! I'm talking to you!!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("With all of those troops, you'd think they'd last longer..."),
                qsTr("A-aughk!! Uhm… I mean… congratulations, men."),
                qsTr("With Yellow Comet on the line, I can't lose.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("What? You had enough troops..."),
                qsTr("Sensei that's you're fault!")];
    };
    this.getName = function()
    {
        return qsTr("Rattigan");
    };
}

Constructor.prototype = CO;
var CO_RATTIGAN = new Constructor();
