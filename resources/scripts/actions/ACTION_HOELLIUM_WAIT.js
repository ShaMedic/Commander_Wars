var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
		var targetUnit = action.getMovementTarget();
        if ((unit.getHasMoved() === true) ||
            (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0))
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) ||
            (targetUnit === null) || (targetUnit !== null) && (unit.getOwner().isEnemyUnit(targetUnit )))
        {
            return true;
        }
        else
        {
			
            return false;
        }
    };
    this.getActionText = function(map)
    {
        return qsTr("Wait");
    };
    this.getIcon = function(map)
    {
        return "wait";
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var targetUnit = action.getMovementTarget();
        if ((targetUnit !== null) && (targetUnit !== unit))
        {
            targetUnit.killUnit();
        }
        Global[unit.getUnitID()].doWalkingAnimation(action, map);
        // move unit to target position
        unit.moveUnitAction(action);
        // disable unit commandments for this turn
        unit.setHasMoved(true);
    };
    this.getDescription = function()
    {
        return qsTr("Orders a oozium to wait at the current position and end all actions for this turn. Ooziums can wait on enemy units in doing so the enemy unit gets destroyed by the oozium.");
    };
    this.getRequiresEmptyField = function(action)
    {
        return false;
    };
}

Constructor.prototype = ACTION;
var ACTION_HOELLIUM_WAIT = new Constructor();
