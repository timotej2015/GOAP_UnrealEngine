#pragma once
#include <stdio.h>
typedef unsigned long long WorldState;

#define MAX_ACTIONS 32
#define MAX_GOALS 16
#define MAX_NODES 2048
#define MAX_OPEN 2048

class GOAP_Actor;
class GOAP_Planner;

// ---------------- Action ----------------
class Action {
public:
    const char* name;
    WorldState preconditions{};
    WorldState effectsSet{};
    WorldState effectsClear{};

    Action(const char* n) : name(n) {}
    virtual void Execute() = 0;
    virtual float GetCost(WorldState* startpoint, bool startpointEqualsStartNode) = 0;
    virtual ~Action() {}
};

// ---------------- GOAP Goal ----------------
struct GOAP_Goal {
    const char* name;
    WorldState goalSet;
    WorldState goalClear;
    float weight;
    float costToReach = -1;
};

// ---------------- BST Node ----------------
class Nodes_BST {
public:
    struct Node {
        WorldState state;
        float g, f;
        int parent;
        int action;
        bool closed;
        int heapIndex;
        int left, right;
    };

    Node bstNodes[MAX_NODES];
    int count = 0;
    int root = -1;

    void Init();
    int Insert(const WorldState& state, float g, float f, int parent, int action);
    int Find(const WorldState& state);
    Node* Get(int index);
private:

};

// ---------------- Priority Queue ----------------
class PriorityQueue_MBH {
public:
    void Init(Nodes_BST::Node* nodeArray);
    bool IsEmpty() const;
    void Push(int node);
    int Pop();
private:
    int heap[MAX_OPEN] = {};
    int count = 0;
    Nodes_BST::Node* nodes = nullptr;
    void Swap(int a, int b);
    void BubbleUp(int i);
    void BubbleDown(int i);
};

// ---------------- GOAP Planner ----------------
class GOAP_Planner {
public:
    GOAP_Planner(GOAP_Actor* newActor);
    int Planner(const WorldState& start);
    void PrintPlan(int node);

private:
    GOAP_Actor* actor;
    Nodes_BST bst;
    PriorityQueue_MBH priorityQueue;
    int bestGoalIndex = -1;
    float Heuristic(const WorldState& state);
    bool CanExecute(const WorldState& state, Action* action);
    WorldState ApplyAction(WorldState state, Action* action);
    int AddNode(const WorldState& state, float g, int parent, int action);
    int FindState(const WorldState& state);
    int IsGoal(const WorldState& state);
    bool AllGoalsFound();
};

// ---------------- GOAP Actor ----------------
class GOAP_Actor {
public:
    GOAP_Actor();
    virtual ~GOAP_Actor();

    WorldState currentState;
    Action* actions[MAX_ACTIONS];
    int actionCount;
    GOAP_Goal goals[MAX_GOALS];
    int goalCount;

    void AddAction(Action* action);
    void AddGoal(const GOAP_Goal& goal);
    void SetWorldState(const WorldState& state);

    virtual void GeneratePlan();
};