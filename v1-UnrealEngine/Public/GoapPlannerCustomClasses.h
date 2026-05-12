#pragma once

#include "CoreMinimal.h"

class ACPP_GoapPlanner; // Forward declaration to avoid circular dependency


struct WorldState {
    //lahko je 0-false, 1-true ali 2-N/A
    int playerInSight = 2;
    int playerInSwordRange = 2;
    int playerInBowRange = 2;
    int playerUsingSword = 2;
    int playerUsingShield = 2;
    int swordEquipped = 2;
    int bowEquipped = 2;
    int onTheHighGround = 2;
    int playerDead = 2;
    int doorOpened = 2;
    int inDoorRange = 2;
};


class Action {
public:
    ACPP_GoapPlanner* currentClassInstance;
    FString name;

    WorldState preconditions;
    WorldState results;

    Action(FString name, ACPP_GoapPlanner* goapPlannerInstance);

    virtual int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) = 0;
    virtual void triggerBlueprintEvent() = 0;

    virtual ~Action();
};


class ActionEquipSword : public Action {
public:
    ActionEquipSword(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionEquipSword() override;
};


class ActionEquipBow : public Action {
public:
    ActionEquipBow(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionEquipBow() override;
};


class ActionSwordAttack : public Action {
public:
    ActionSwordAttack(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionSwordAttack() override;
};


class ActionShieldBlock : public Action {
public:
    ActionShieldBlock(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode);
    void triggerBlueprintEvent() override;

    ~ActionShieldBlock() override;
};


class ActionBowAttack : public Action {
public:
    ActionBowAttack(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionBowAttack() override;
};


class ActionMoveToPlayerSwordRange : public Action {
public:
    ActionMoveToPlayerSwordRange(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionMoveToPlayerSwordRange() override;
};


class ActionMoveToPlayerBowRange : public Action {
public:
    ActionMoveToPlayerBowRange(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionMoveToPlayerBowRange() override;
};


class ActionMoveToPlayerBowRangeWhenDoorClosed : public Action {
    //ta akcija je zato, da ne grejo na zacetku vsi proti tarci (torej po NavMeshu do zaprtih vrat)
public:
    ActionMoveToPlayerBowRangeWhenDoorClosed(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionMoveToPlayerBowRangeWhenDoorClosed() override;
};


class ActionMoveToHighGround : public Action {
public:
    ActionMoveToHighGround(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionMoveToHighGround() override;
};


class ActionMoveToDoorRange : public Action {
public:
    ActionMoveToDoorRange(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionMoveToDoorRange() override;
};


class ActionBreakDoor : public Action {
public:
    ActionBreakDoor(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent();

    ~ActionBreakDoor() override;
};


//ta ne gre v actionsList
class ActionDoNothing : public Action {
public:
    ActionDoNothing(ACPP_GoapPlanner* goapPlannerInstance);

    int getPerformCost(WorldState* startpointWorldState, bool startpointEqualsDijkstraStartNode) override;
    void triggerBlueprintEvent() override;

    ~ActionDoNothing() override;
};


struct WorldStateGraphConnection {
public:
    Action* connectingAction;
    int endpointNodeIndex;
};


class WorldStateGraphNode {
public:
    WorldState worldState;
    TArray<WorldStateGraphConnection> adjacentVerticesLinks;

    WorldStateGraphNode* dijkstraPreviousNode;
    Action* dijkstraPreviousNodeConnectingAction;
    int dijkstraShortestPathFromStart;
    bool dijkstraVisited;
    bool dijkstraDiscovered;
    int nodeIndexInBinaryHeapArray;

    WorldStateGraphNode();
};


class PriorityQueue {
private:
    TArray<WorldStateGraphNode*> minBinaryHeapArray;
public:
    void fixHeapAtIndex(int heapArrayIndex);
    bool heapIsEmpty();
    WorldStateGraphNode* getMinDistanceNode();
    void insertNewNode(WorldStateGraphNode* newNode);
    void deleteRootNode();

    PriorityQueue();
};