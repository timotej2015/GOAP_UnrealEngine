#include "GOAP_Core.h"

// ---------------- Nodes_BST ----------------
void Nodes_BST::Init()
{
    count = 0;
    root = -1;
}

int Nodes_BST::Insert(const WorldState& state, float g, float f, int parent, int action)
{
    int newIdx = count++;
    bstNodes[newIdx] = { state, g, f, parent, action, false, -1, -1, -1 };

    if (root == -1)
    {
        root = newIdx;
        return newIdx;
    }

    int current = root;
    while (true)
    {
        if (state < bstNodes[current].state)
        {
            if (bstNodes[current].left == -1) { bstNodes[current].left = newIdx; break; }
            current = bstNodes[current].left;
        }
        else if (state > bstNodes[current].state)
        {
            if (bstNodes[current].right == -1) { bstNodes[current].right = newIdx; break; }
            current = bstNodes[current].right;
        }
        else
        {
            // že obstaja
            count--;
            return current;
        }
    }
    return newIdx;
}

int Nodes_BST::Find(const WorldState& state)
{
    int current = root;
    while (current != -1)
    {
        if (bstNodes[current].state == state) return current;
        else if (state < bstNodes[current].state) current = bstNodes[current].left;
        else current = bstNodes[current].right;
    }
    return -1;
}

Nodes_BST::Node* Nodes_BST::Get(int index)
{
    return &bstNodes[index];
}






// ---------------- PriorityQueue_MBH ----------------
void PriorityQueue_MBH::Init(Nodes_BST::Node* nodeArray)
{
    nodes = nodeArray;
    count = 0;
}

bool PriorityQueue_MBH::IsEmpty() const {
    return count == 0;
}

void PriorityQueue_MBH::Swap(int a, int b)
{
    int t = heap[a];
    heap[a] = heap[b];
    heap[b] = t;

    nodes[heap[a]].heapIndex = a;
    nodes[heap[b]].heapIndex = b;
}

void PriorityQueue_MBH::BubbleUp(int i)
{
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (nodes[heap[parent]].f <= nodes[heap[i]].f) break;
        Swap(parent, i);
        i = parent;
    }
}

void PriorityQueue_MBH::BubbleDown(int i)
{
    while (true)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < count && nodes[heap[left]].f < nodes[heap[smallest]].f) smallest = left;
        if (right < count && nodes[heap[right]].f < nodes[heap[smallest]].f) smallest = right;
        if (smallest == i) break;

        Swap(i, smallest);
        i = smallest;
    }
}

void PriorityQueue_MBH::Push(int node)
{
    if (nodes[node].heapIndex != -1)
    {
        BubbleUp(nodes[node].heapIndex);
        return;
    }

    int i = count++;
    heap[i] = node;
    nodes[node].heapIndex = i;
    BubbleUp(i);
}

int PriorityQueue_MBH::Pop()
{
    int result = heap[0];
    nodes[result].heapIndex = -1;

    heap[0] = heap[--count];
    if (count > 0)
    {
        nodes[heap[0]].heapIndex = 0;
        BubbleDown(0);
    }

    return result;
}






// ---------------- GOAP_Planner ----------------
GOAP_Planner::GOAP_Planner(GOAP_Actor* newActor)
{
    actor = newActor;
    bst.Init();
    priorityQueue.Init(nullptr);
}

float GOAP_Planner::Heuristic(const WorldState& state)
{
    // metoda oceni pricakovan strosek poti od state do cilja
    return 0;
}

bool GOAP_Planner::CanExecute(const WorldState& state, Action* action)
{
    // metoda preverja, ce se iz podanega stanja sveta da izvesti podano akcijo
    return (state & action->preconditions) == action->preconditions;
}

WorldState GOAP_Planner::ApplyAction(WorldState state, Action* action)
{
    // metoda vrne stanje sveta, kamor iz podanega stanja vodi podana akcija
    state |= action->effectsSet;    // prizge bite, ki jih akcija nastavi na 1
    state &= ~action->effectsClear; // ugasne bite, ki jih akcija nastavi na 0
    return state;                   // novo stanje sveta po izvedbi akcije
}

int GOAP_Planner::AddNode(const WorldState& state, float g, int parent, int action)
{
    float f = g + Heuristic(state);
    int idx = bst.Insert(state, g, f, parent, action);
    return idx;
}

int GOAP_Planner::FindState(const WorldState& state)
{
    // metoda preverja, ce je bilo dano vozlisce ze najdeno. S tem se izognemo vracanju na isto vozlisce
    return bst.Find(state);
}

int GOAP_Planner::IsGoal(const WorldState& state)
{
    for (int i = 0; i < actor->goalCount; i++)
    {
        if (((state & actor->goals[i].goalSet) == actor->goals[i].goalSet) &&
            ((~state & actor->goals[i].goalClear) == actor->goals[i].goalClear))
        {
            return i; // index cilja
        }
    }
    return -1; // noben cilj
}

bool GOAP_Planner::AllGoalsFound() {
    for (int i = 0; i < actor->goalCount; i++) {
        if (actor->goals[i].costToReach == -1) {
            return false;
        }
    }
    return true;
}

int GOAP_Planner::Planner(const WorldState& start)
{
    bst.Init();
    priorityQueue.Init(bst.bstNodes);

    // reset costToReach za vse cilje
    for (int i = 0; i < actor->goalCount; i++)
        actor->goals[i].costToReach = -1.0f;

    int startNode = AddNode(start, 0, -1, -1);
    priorityQueue.Push(startNode);

    struct GoalRecord
    {
        int node;
        int goalIndex;
        float weightedCost;
    };

    GoalRecord bestGoal = { -1, -1, 1e30f };    // 1e30f je 10^30

    while (!priorityQueue.IsEmpty() && !AllGoalsFound())
    {
        // vzamemo najbolj ugodno vozlišče iz prednostne vrste
        int current = priorityQueue.Pop();
        auto node = bst.Get(current);

        // če je vozlišče že obdelano (closed), ga preskočimo
        if (node->closed) continue;
        node->closed = true;

        // preverimo, če je smo prišli v enega izmed ciljev
        int goalIdx = IsGoal(node->state);
        if (goalIdx != -1 && actor->goals[goalIdx].costToReach < 0.0f)
        {
            // zabeležimo strošek poti do tega cilja
            float costToReachGoal = node->g * actor->goals[goalIdx].weight;
            actor->goals[goalIdx].costToReach = costToReachGoal;

            if (costToReachGoal < bestGoal.weightedCost)
            {
                bestGoal = { current, goalIdx, costToReachGoal };
                bestGoalIndex = goalIdx;
            }
        }

        for (int a = 0; a < actor->actionCount; a++)
        {
            // poišči sosede vozlišča, kamor vodijo razpoložljive akcije
            Action* action = actor->actions[a];
            if (!CanExecute(node->state, action))
                continue;

            WorldState newState = ApplyAction(node->state, action);
            bool startEqualsStartNode = (node->parent == -1);
            float newCost = node->g + action->GetCost(&node->state, startEqualsStartNode);

            // preveri, če je bilo to vozlišče že najdeno
            int existing = FindState(newState);

            if (existing != -1)
            {
                auto existingNode = bst.Get(existing);
                if (existingNode->closed) continue;

                if (newCost < existingNode->g)
                {
                    existingNode->g = newCost;
                    existingNode->f = newCost + Heuristic(newState);
                    existingNode->parent = current;
                    existingNode->action = a;
                    priorityQueue.Push(existing);
                }
                continue;
            }

            int newNode = AddNode(newState, newCost, current, a);
            priorityQueue.Push(newNode);
        }
    }

    return bestGoal.node; // najbolj optimalni cilj med doseženimi
}

void GOAP_Planner::PrintPlan(int node)
{
    if (node == -1) {
        printf("PLAN NOT FOUND\n");
        return;
    }

    // 1. Najprej sestavimo pot od cilja do začetka
    int path[64];
    int count = 0;
    while (node != -1)
    {
        path[count++] = node;
        node = bst.Get(node)->parent;
    }

    // 2. Izračunaj skupni strošek in adjusted cost za best goal
    int bestNodeIndex = path[0]; // zadnji element poti (cilj)
    float totalCost = bst.Get(bestNodeIndex)->g;
    float adjustedCost = actor->goals[bestGoalIndex].costToReach;

    printf(
        "BEST GOAL: %s\n"
        "PLAN COST: %.2f\n"
        "ADJUSTED PLAN COST: %.2f\n\n",
        actor->goals[bestGoalIndex].name,
        totalCost,
        adjustedCost
    );

    // 3. Izpišemo pot od začetka do cilja
    int step = 1;
    for (int i = count - 1; i >= 0; i--)
    {
        auto n = bst.Get(path[i]);
        int actionIndex = n->action;

        if (actionIndex != -1)
        {
            const char* actionName = actor->actions[actionIndex]->name;
            float actionCost = actor->actions[actionIndex]->GetCost(nullptr, false);
            float cumulativeCost = n->g;

            printf("Action %d: %s | Cost: %.2f | Cumulative Cost: %.2f\n",
                step, actionName, actionCost, cumulativeCost);

            step++;
        }
    }
}




// ---------------- GOAP_Actor ----------------
GOAP_Actor::GOAP_Actor()
{
    currentState = 0;
    actionCount = 0;
    goalCount = 0;
}

GOAP_Actor::~GOAP_Actor()
{
    for (int i = 0; i < actionCount; i++)
        delete actions[i];
}

void GOAP_Actor::AddAction(Action* action)
{
    if (actionCount < MAX_ACTIONS)
    {
        actions[actionCount++] = action;
    }
}

void GOAP_Actor::AddGoal(const GOAP_Goal& goal)
{
    if (goalCount < MAX_GOALS)
    {
        goals[goalCount++] = goal;
    }
}

void GOAP_Actor::SetWorldState(const WorldState& state)
{
    currentState = state;
}

void GOAP_Actor::GeneratePlan()
{
    GOAP_Planner planner(this);
    int bestNode = planner.Planner(currentState);
    planner.PrintPlan(bestNode);
}