#pragma once
#include "GOAP_Core.h"

// ---------------------------------------------
// WorldState bits specifični za Soldier NPC
// ---------------------------------------------
enum SoldierWorldStateBits : WorldState
{
    SwordEquipped = 1ULL << 0,
    BowEquipped = 1ULL << 1,
    EnemyInSight = 1ULL << 2,
    EnemyDead = 1ULL << 3,
    EnemyUsingSword = 1ULL << 4,
    EnemyInSwordRange = 1ULL << 5,
    EnemyInBowRange = 1ULL << 6,
    OnTheHighGround = 1ULL << 7,
    InDoorRange = 1ULL << 8,
    DoorOpened = 1ULL << 9
};

// ---------------------------------------------
// Concrete Actions za Soldier NPC
// ---------------------------------------------
class ActionEquipSword : public Action
{
public:
    ActionEquipSword() : Action("Equip Sword")
    {
        preconditions = BowEquipped;
        effectsSet = SwordEquipped;
        effectsClear = BowEquipped;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 2.0f; }
};

class ActionEquipBow : public Action
{
public:
    ActionEquipBow() : Action("Equip Bow")
    {
        preconditions = SwordEquipped;
        effectsSet = BowEquipped;
        effectsClear = SwordEquipped;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 2.0f; }
};

class ActionSwordAttack : public Action
{
public:
    ActionSwordAttack() : Action("Sword Attack")
    {
        preconditions = SwordEquipped | EnemyInSight | EnemyInSwordRange;
        effectsSet = EnemyDead;
        effectsClear = EnemyUsingSword;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 3.0f; }
};

class ActionShieldBlock : public Action
{
public:
    ActionShieldBlock() : Action("Shield Block")
    {
        preconditions = SwordEquipped | EnemyInSight | EnemyInSwordRange | EnemyUsingSword;
        effectsSet = 0;
        effectsClear = EnemyUsingSword;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 1.5f; }
};

class ActionBowAttack : public Action
{
public:
    ActionBowAttack() : Action("Bow Attack")
    {
        preconditions = BowEquipped | EnemyInSight | EnemyInBowRange;
        effectsSet = EnemyDead;
        effectsClear = 0;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 3.0f; }
};

class ActionMoveToPlayerSwordRange : public Action
{
public:
    ActionMoveToPlayerSwordRange() : Action("Move to Player's Sword Range")
    {
        preconditions = SwordEquipped;
        effectsSet = EnemyInSwordRange | EnemyInBowRange | EnemyInSight;
        effectsClear = 0;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 1.0f; }
};

class ActionMoveToPlayerBowRange : public Action
{
public:
    ActionMoveToPlayerBowRange() : Action("Move to Player's Bow Range")
    {
        preconditions = BowEquipped;
        effectsSet = EnemyInBowRange | EnemyInSight;
        effectsClear = 0;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 1.0f; }
};

class ActionMoveToHighGround : public Action
{
public:
    ActionMoveToHighGround() : Action("Move to High Ground")
    {
        preconditions = 0;
        effectsSet = OnTheHighGround | EnemyInSight;
        effectsClear = 0;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 2.0f; }
};

class ActionMoveToDoorRange : public Action
{
public:
    ActionMoveToDoorRange() : Action("Move to Door Range")
    {
        preconditions = SwordEquipped;
        effectsSet = InDoorRange;
        effectsClear = 0;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 1.0f; }
};

class ActionBreakDoor : public Action
{
public:
    ActionBreakDoor() : Action("Break the Door")
    {
        preconditions = SwordEquipped | InDoorRange;
        effectsSet = DoorOpened;
        effectsClear = 0;
    }
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 2.0f; }
};

class ActionDoNothing : public Action
{
public:
    ActionDoNothing() : Action("Do Nothing") {}
    void Execute() override {}
    float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) override { return 1.0f; }
};

// ---------------------------------------------
// SoldierActor: Deduje iz GOAP_Actor in inicializira akcije in cilje
// ---------------------------------------------
class SoldierActor : public GOAP_Actor
{
public:
    SoldierActor()
    {
        // dodamo vse akcije
        AddAction(new ActionEquipSword());
        AddAction(new ActionEquipBow());
        AddAction(new ActionSwordAttack());
        AddAction(new ActionShieldBlock());
        AddAction(new ActionBowAttack());
        AddAction(new ActionMoveToPlayerSwordRange());
        AddAction(new ActionMoveToPlayerBowRange());
        AddAction(new ActionMoveToHighGround());
        AddAction(new ActionMoveToDoorRange());
        AddAction(new ActionBreakDoor());
        AddAction(new ActionDoNothing());

        // dodamo cilje
        GOAP_Goal g0 = { "Enemy dead", EnemyDead, 0, 100.0f };
        GOAP_Goal g1 = { "On the high ground", OnTheHighGround, 0, 50.0f };
        GOAP_Goal g2 = { "Door opened", DoorOpened, 0, 10.0f };

        AddGoal(g0);
        AddGoal(g1);
        AddGoal(g2);

        // začetno stanje
        SetWorldState(SwordEquipped | EnemyInSight);
    }
};