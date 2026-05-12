#include "GoapPlannerCustomClasses.h"
#include "CPP_GoapPlanner.h"



Action::Action(FString name, ACPP_GoapPlanner* goapPlannerInstance) {
    this->currentClassInstance = goapPlannerInstance;
    this->name = name;
}

Action::~Action() {}



ActionEquipSword::ActionEquipSword(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Equip Sword")), goapPlannerInstance) {
    this->preconditions.swordEquipped = 0;
    this->preconditions.bowEquipped = 1;

    this->results.swordEquipped = 1;
    this->results.bowEquipped = 0;
}

int ActionEquipSword::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    return this->currentClassInstance->GOAPEquipSwordCost;
}

void ActionEquipSword::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPEquipSwordEvent();
}

ActionEquipSword::~ActionEquipSword() {}



ActionEquipBow::ActionEquipBow(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Equip Bow")), goapPlannerInstance) {
    this->preconditions.swordEquipped = 1;
    this->preconditions.bowEquipped = 0;

    this->results.swordEquipped = 0;
    this->results.bowEquipped = 1;
}

int ActionEquipBow::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    return this->currentClassInstance->GOAPEquipBowCost;
}

void ActionEquipBow::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPEquipBowEvent();
}

ActionEquipBow::~ActionEquipBow() {}



ActionSwordAttack::ActionSwordAttack(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Sword Attack")), goapPlannerInstance) {
    this->preconditions.swordEquipped = 1;
    this->preconditions.bowEquipped = 0;
    this->preconditions.playerInSight = 1;
    this->preconditions.playerInSwordRange = 1;
    this->preconditions.playerUsingShield = 0;
    this->preconditions.playerUsingSword = 0;

    this->results.playerDead = 1;
}

int ActionSwordAttack::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    return this->currentClassInstance->GOAPSwordAttackCost;
}

void ActionSwordAttack::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPSwordAttackEvent();
}

ActionSwordAttack::~ActionSwordAttack() {}



ActionShieldBlock::ActionShieldBlock(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Shield Block")), goapPlannerInstance) {
    this->preconditions.swordEquipped = 1;
    this->preconditions.bowEquipped = 0;
    this->preconditions.playerInSight = 1;
    this->preconditions.playerInSwordRange = 1;
    this->preconditions.playerUsingSword = 1;

    this->results.playerUsingSword = 0;
}

int ActionShieldBlock::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    return this->currentClassInstance->GOAPShieldBlockCost;
}

void ActionShieldBlock::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPShieldBlockEvent();
}

ActionShieldBlock::~ActionShieldBlock() {}



ActionBowAttack::ActionBowAttack(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Bow Attack")), goapPlannerInstance) {
    this->preconditions.swordEquipped = 0;
    this->preconditions.bowEquipped = 1;
    this->preconditions.playerInSight = 1;
    this->preconditions.playerInBowRange = 1;
    this->preconditions.playerUsingShield = 0;

    this->results.playerDead = 1;
}

int ActionBowAttack::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    return this->currentClassInstance->GOAPBowAttackCost;
}

void ActionBowAttack::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPBowAttackEvent();
}

ActionBowAttack::~ActionBowAttack() {}



ActionMoveToPlayerSwordRange::ActionMoveToPlayerSwordRange(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Move to Player's Sword Range")), goapPlannerInstance) {
    this->preconditions.swordEquipped = 1;
    this->preconditions.bowEquipped = 0;
    this->preconditions.playerInSwordRange = 0;
    this->preconditions.doorOpened = 1;

    this->results.playerInSwordRange = 1;
    this->results.playerInBowRange = 1;
    this->results.playerInSight = 1;
}

int ActionMoveToPlayerSwordRange::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    if (startpointEqualsDijkstraStartNode) {
        return this->currentClassInstance->GOAPThisSoldierToPlayerDistance;
    }

    if (startpointWorldState->inDoorRange == 1) {
        return this->currentClassInstance->GOAPDoorToPlayerDistance;
    }
    else if (startpointWorldState->onTheHighGround == 1) {
        return this->currentClassInstance->GOAPHighGroundToPlayerDistance;
    }
    else {
        return this->currentClassInstance->GOAPThisSoldierToPlayerDistance;
    }
}

void ActionMoveToPlayerSwordRange::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPMoveToPlayerSwordRangeEvent();
}

ActionMoveToPlayerSwordRange::~ActionMoveToPlayerSwordRange() {}



ActionMoveToPlayerBowRange::ActionMoveToPlayerBowRange(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Move to Player's Bow Range")), goapPlannerInstance) {
    if (this->currentClassInstance->isAnEnemy == false) {
        this->preconditions.doorOpened = 1;
    }
    this->preconditions.swordEquipped = 0;
    this->preconditions.bowEquipped = 1;
    this->preconditions.playerInBowRange = 0;

    this->results.playerInBowRange = 1;
    this->results.playerInSight = 1;
}

int ActionMoveToPlayerBowRange::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    if (startpointEqualsDijkstraStartNode) {
        return this->currentClassInstance->GOAPThisSoldierToPlayerDistance - this->currentClassInstance->GOAPBowAttackRange;
    }

    if (startpointWorldState->inDoorRange == 1) {
        return this->currentClassInstance->GOAPDoorToPlayerDistance - this->currentClassInstance->GOAPBowAttackRange;
    }
    else if (startpointWorldState->onTheHighGround == 1) {
        //return this->currentClassInstance->GOAPHighGroundToPlayerDistance - this->currentClassInstance->GOAPBowAttackRange;
        //vojaku obljubimo, da bo pot do tarce, ce gre najprej na HighGround enaka 0 (kar seveda ni res)
        return 0;
    }
    else {
        return this->currentClassInstance->GOAPThisSoldierToPlayerDistance - this->currentClassInstance->GOAPBowAttackRange;
    }
}

void ActionMoveToPlayerBowRange::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPMoveToPlayerBowRangeEvent();
}

ActionMoveToPlayerBowRange::~ActionMoveToPlayerBowRange() {}



ActionMoveToPlayerBowRangeWhenDoorClosed::ActionMoveToPlayerBowRangeWhenDoorClosed(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Move to Player's Bow Range When Door Closed")), goapPlannerInstance) {
    this->preconditions.doorOpened = 0;
    this->preconditions.playerInBowRange = 0;
    this->preconditions.swordEquipped = 1;
    this->preconditions.bowEquipped = 0;

    this->results.playerInBowRange = 1;
    this->results.playerInSight = 1;
}

int ActionMoveToPlayerBowRangeWhenDoorClosed::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    if (startpointEqualsDijkstraStartNode) {
        return this->currentClassInstance->GOAPThisSoldierToHighGroundDistance;
    }

    if (startpointWorldState->inDoorRange == 1) {
        return this->currentClassInstance->GOAPDoorToHighGroundDistance;
    }
    else if ((startpointWorldState->playerInBowRange == 1) || (startpointWorldState->playerInSwordRange == 1)) {
        return this->currentClassInstance->GOAPHighGroundToPlayerDistance;
    }
    else {
        return this->currentClassInstance->GOAPThisSoldierToHighGroundDistance;
    }
}

void ActionMoveToPlayerBowRangeWhenDoorClosed::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPMoveToHighGroundEvent();
}

ActionMoveToPlayerBowRangeWhenDoorClosed::~ActionMoveToPlayerBowRangeWhenDoorClosed() {}



ActionMoveToHighGround::ActionMoveToHighGround(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Move to the High Ground")), goapPlannerInstance) {
    if (currentClassInstance->isAnEnemy) {
        this->preconditions.doorOpened = 1;
    }
    this->preconditions.onTheHighGround = 0;
    this->preconditions.swordEquipped = 1;
    this->preconditions.bowEquipped = 0;

    this->results.onTheHighGround = 1;
    this->results.playerInSight = 1;
}

int ActionMoveToHighGround::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    if (startpointEqualsDijkstraStartNode) {
        return this->currentClassInstance->GOAPThisSoldierToHighGroundDistance;
    }

    if (startpointWorldState->inDoorRange == 1) {
        return this->currentClassInstance->GOAPDoorToHighGroundDistance;
    }
    else if ((startpointWorldState->playerInBowRange == 1) || (startpointWorldState->playerInSwordRange == 1)) {
        return this->currentClassInstance->GOAPHighGroundToPlayerDistance;
    }
    else {
        return this->currentClassInstance->GOAPThisSoldierToHighGroundDistance;
    }
}

void ActionMoveToHighGround::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPMoveToHighGroundEvent();
}

ActionMoveToHighGround::~ActionMoveToHighGround() {}



ActionMoveToDoorRange::ActionMoveToDoorRange(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Move to the Door Range")), goapPlannerInstance) {
    this->preconditions.swordEquipped = 1;
    this->preconditions.bowEquipped = 0;
    this->preconditions.inDoorRange = 0;

    this->results.inDoorRange = 1;
}

int ActionMoveToDoorRange::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    if (startpointEqualsDijkstraStartNode) {
        return this->currentClassInstance->GOAPThisSoldierToDoorDistance;
    }

    if (startpointWorldState->onTheHighGround == 1) {
        return this->currentClassInstance->GOAPDoorToHighGroundDistance;
    }
    else if ((startpointWorldState->playerInBowRange == 1) || (startpointWorldState->playerInSwordRange == 1)) {
        return this->currentClassInstance->GOAPDoorToPlayerDistance;
    }
    else {
        return this->currentClassInstance->GOAPThisSoldierToDoorDistance;
    }
}

void ActionMoveToDoorRange::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPMoveToDoorRangeEvent();
}

ActionMoveToDoorRange::~ActionMoveToDoorRange() {}



ActionBreakDoor::ActionBreakDoor(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Break the Door")), goapPlannerInstance) {
    this->preconditions.swordEquipped = 1;
    this->preconditions.inDoorRange = 1;
    this->preconditions.doorOpened = 0;

    if (this->currentClassInstance->isAnEnemy == true) {
        //s tem onemogocimo, da bi se branilci nabrali pred vrata (kar bi bilo sicer logicno, zabavno pa ne)
        this->results.doorOpened = 1;
    }
    else {
        this->results.doorOpened = 0;
    }
}

int ActionBreakDoor::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    return this->currentClassInstance->GOAPBreakDoorCost;
}

void ActionBreakDoor::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPBreakDoorEvent();
}

ActionBreakDoor::~ActionBreakDoor() {}



ActionDoNothing::ActionDoNothing(ACPP_GoapPlanner* goapPlannerInstance) : Action(FString(TEXT("Do Nothing")), goapPlannerInstance) {
}

int ActionDoNothing::getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) {
    return 1;
}

void ActionDoNothing::triggerBlueprintEvent() {
    this->currentClassInstance->GOAPDoNothingEvent();
}

ActionDoNothing::~ActionDoNothing() {}



WorldStateGraphNode::WorldStateGraphNode()
{
    this->adjacentVerticesLinks.Empty();

    this->dijkstraPreviousNode = nullptr;
    this->dijkstraPreviousNodeConnectingAction = nullptr;
    this->dijkstraShortestPathFromStart = 0;
    this->dijkstraVisited = false;
    this->dijkstraDiscovered = false;
    this->nodeIndexInBinaryHeapArray = 0;
}



PriorityQueue::PriorityQueue()
{
    this->minBinaryHeapArray.Empty();
}

void PriorityQueue::fixHeapAtIndex(int heapArrayIndex)
{
    //metoda poskrbi, da so vrednosti dijkstraShortestPathFromStart pri starsih vedno manjse kot pri otrocih
    //navzdol ni treba preverjat, ker lahko Dijkstra vozliscem vrednost le zmanjsuje
    while (true) {
        int parentNodeIndex = (heapArrayIndex - 1) / 2;
        if (this->minBinaryHeapArray[heapArrayIndex]->dijkstraShortestPathFromStart < this->minBinaryHeapArray[parentNodeIndex]->dijkstraShortestPathFromStart) {
            //v starsu je dijkstraShortestPathFromStart vecja, torej zamenjaj starsa in otroka
            WorldStateGraphNode* swapStorage = this->minBinaryHeapArray[parentNodeIndex];
            this->minBinaryHeapArray[parentNodeIndex] = this->minBinaryHeapArray[heapArrayIndex];
            this->minBinaryHeapArray[heapArrayIndex] = swapStorage;

            this->minBinaryHeapArray[parentNodeIndex]->nodeIndexInBinaryHeapArray = parentNodeIndex;
            this->minBinaryHeapArray[heapArrayIndex]->nodeIndexInBinaryHeapArray = heapArrayIndex;
        }
        else {
            break;
        }
    }
}

bool PriorityQueue::heapIsEmpty()
{
    return (this->minBinaryHeapArray.Num() == 0);
}

WorldStateGraphNode* PriorityQueue::getMinDistanceNode()
{
    return (this->minBinaryHeapArray[0]);
}

void PriorityQueue::insertNewNode(WorldStateGraphNode* newNode)
{
    this->minBinaryHeapArray.Add(newNode);
    fixHeapAtIndex(this->minBinaryHeapArray.Num() - 1);
}

void PriorityQueue::deleteRootNode()
{
    //odstrani root in ga zamenjaj z najbolj desnim elementom v arrayu
    this->minBinaryHeapArray[0] = this->minBinaryHeapArray[this->minBinaryHeapArray.Num() - 1];
    this->minBinaryHeapArray.RemoveAt(this->minBinaryHeapArray.Num() - 1);

    //za starsa poglej ce imata otroka vecji dijkstraShortestPathFromStart. Ce temu ni tako, ga zamenjaj z manjsim izmed obeh otrok.
    int currentParentNodeIndex = 0;
    while (true) {
        if (heapIsEmpty()) {
            break;
        }

        int currentMinValue = this->minBinaryHeapArray[currentParentNodeIndex]->dijkstraShortestPathFromStart;
        int nodeWithMinValueIndex = currentParentNodeIndex;

        //poglej, katero izmed 3 vozlisc (stars in 2 otroka) ima najmanjsi dijkstraShortestPathFromStart
        bool terminateWhileLoop = false;
        for (int i = 1; i < 3; i++) {
            int childIndex = 2 * currentParentNodeIndex + i;
            if (childIndex >= minBinaryHeapArray.Num()) {
                //smo na dnu heapa na tem mestu pa ni otroka, torej nehaj preiskovati
                terminateWhileLoop = true;
                break;
            }
            if (this->minBinaryHeapArray[childIndex]->dijkstraShortestPathFromStart < currentMinValue) {
                currentMinValue = this->minBinaryHeapArray[childIndex]->dijkstraShortestPathFromStart;
                nodeWithMinValueIndex = childIndex;
            }
        }

        //ce ima kateri od otrok manjso vrednost od starsa, ju zamenjaj in nadaljuj zanko. Sicer zakljuci zanko.
        if (nodeWithMinValueIndex == currentParentNodeIndex) {
            break;
        }

        WorldStateGraphNode* swapStorage = this->minBinaryHeapArray[currentParentNodeIndex];
        this->minBinaryHeapArray[currentParentNodeIndex] = this->minBinaryHeapArray[nodeWithMinValueIndex];
        this->minBinaryHeapArray[nodeWithMinValueIndex] = swapStorage;

        this->minBinaryHeapArray[currentParentNodeIndex]->nodeIndexInBinaryHeapArray = currentParentNodeIndex;
        this->minBinaryHeapArray[nodeWithMinValueIndex]->nodeIndexInBinaryHeapArray = nodeWithMinValueIndex;

        currentParentNodeIndex = nodeWithMinValueIndex;

        if (terminateWhileLoop) {
            break;
        }
    }
}