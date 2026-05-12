// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GoapPlanner.h"


int ACPP_GoapPlanner::calculatePower(int base, int exponent) {
    int result = 1;

    for (int i = 0; i < exponent; i++) {
        result = result * base;
    }

    return result;
}


void ACPP_GoapPlanner::setWorldStateParameterValueByIndex(WorldState* worldState, int parameterIndex, int value) {
    switch (parameterIndex) {
    case 0:
        worldState->playerInSight = value;
        break;
    case 1:
        worldState->playerInSwordRange = value;
        break;
    case 2:
        worldState->playerInBowRange = value;
        break;
    case 3:
        worldState->playerUsingSword = value;
        break;
    case 4:
        worldState->playerUsingShield = value;
        break;
    case 5:
        worldState->swordEquipped = value;
        break;
    case 6:
        worldState->bowEquipped = value;
        break;
    case 7:
        worldState->onTheHighGround = value;
        break;
    case 8:
        worldState->playerDead = value;
        break;
    case 9:
        worldState->doorOpened = value;
        break;
    case 10:
        worldState->inDoorRange = value;
        break;
    default:
        //neveljaven parameterIndex
        break;
    }
}

int ACPP_GoapPlanner::getWorldStateParameterValueByIndex(WorldState* worldState, int parameterIndex) {
    switch (parameterIndex) {
    case 0:
        return(worldState->playerInSight);
    case 1:
        return(worldState->playerInSwordRange);
    case 2:
        return(worldState->playerInBowRange);
    case 3:
        return(worldState->playerUsingSword);
    case 4:
        return(worldState->playerUsingShield);
    case 5:
        return(worldState->swordEquipped);
    case 6:
        return(worldState->bowEquipped);
    case 7:
        return(worldState->onTheHighGround);
    case 8:
        return(worldState->playerDead);
    case 9:
        return(worldState->doorOpened);
    default:
        //case 10 oziroma neveljaven parameterIndex
        return(worldState->inDoorRange);
    }
}





void ACPP_GoapPlanner::setDefaultActionsCosts() {
    GOAPThisSoldierToPlayerDistance = 1;
    GOAPThisSoldierToDoorDistance = 1;
    GOAPThisSoldierToHighGroundDistance = 1;
    GOAPDoorToHighGroundDistance = 1;
    GOAPDoorToPlayerDistance = 1;
    GOAPHighGroundToPlayerDistance = 1;
    GOAPBowAttackRange = 1;
    GOAPEquipSwordCost = 1;
    GOAPEquipBowCost = 1;
    GOAPSwordAttackCost = 1;
    GOAPShieldBlockCost = 1;
    GOAPBowAttackCost = 1;
    GOAPBreakDoorCost = 1;
}


void ACPP_GoapPlanner::fillActionsList() {
    FString currentClassName = this->GetName();
    if (currentClassName.Contains("enemy", ESearchCase::IgnoreCase)) {
        isAnEnemy = true;
    }
    else {
        isAnEnemy = false;
    }

    defaultActionToExecute = new ActionDoNothing(this);

    actionsList.Empty();

    actionsList.Add(new ActionEquipSword(this));
    actionsList.Add(new ActionEquipBow(this));
    actionsList.Add(new ActionSwordAttack(this));
    actionsList.Add(new ActionShieldBlock(this));
    actionsList.Add(new ActionBowAttack(this));
    actionsList.Add(new ActionMoveToPlayerSwordRange(this));
    actionsList.Add(new ActionMoveToPlayerBowRange(this));
    actionsList.Add(new ActionMoveToPlayerBowRangeWhenDoorClosed(this));
    actionsList.Add(new ActionMoveToHighGround(this));
    actionsList.Add(new ActionMoveToDoorRange(this));
    actionsList.Add(new ActionBreakDoor(this));
}


void ACPP_GoapPlanner::fillAdjacencyList(int propertyIndex, WorldState currentState) {
    if (propertyIndex == 0) {
        adjacencyList.Empty();
    }

    if (propertyIndex == WORLDSTATE_NUMBER_OF_PARAMETERS) {
        WorldStateGraphNode newGraphNode = WorldStateGraphNode();
        newGraphNode.worldState = currentState;

        adjacencyList.Add(newGraphNode);
    }
    else {
        setWorldStateParameterValueByIndex(&currentState, propertyIndex, 0);
        fillAdjacencyList(propertyIndex + 1, currentState);

        setWorldStateParameterValueByIndex(&currentState, propertyIndex, 1);
        fillAdjacencyList(propertyIndex + 1, currentState);
    }
}


int ACPP_GoapPlanner::findEndpointNodeIndex(WorldStateGraphNode* currentNode, Action* connectingAction) {
    WorldState neighbourNodeWorldState = currentNode->worldState;

    for (int i = 0; i < WORLDSTATE_NUMBER_OF_PARAMETERS; i++) {
        int propertyValue = getWorldStateParameterValueByIndex(&connectingAction->results, i);
        if (propertyValue < 2) {
            setWorldStateParameterValueByIndex(&neighbourNodeWorldState, i, propertyValue);
        }
    }

    int neighbourNodeIndex = 0;
    for (int i = 0; i < WORLDSTATE_NUMBER_OF_PARAMETERS; i++) {
        neighbourNodeIndex += calculatePower(2, (WORLDSTATE_NUMBER_OF_PARAMETERS - i - 1)) * getWorldStateParameterValueByIndex(&neighbourNodeWorldState, i);
    }

    return neighbourNodeIndex;
}


void ACPP_GoapPlanner::FindWorldStateStartpointsForActions(Action* actionFromActionList, int propertyIndex, WorldState currentState) {
    int currentParameterValue = getWorldStateParameterValueByIndex(&actionFromActionList->preconditions, propertyIndex);

    if (propertyIndex == WORLDSTATE_NUMBER_OF_PARAMETERS) {
        //pretvori iz binary v numericen zapis indeksa
        int numericNodeIndex = 0;
        for (int i = 0; i < WORLDSTATE_NUMBER_OF_PARAMETERS; i++) {
            numericNodeIndex += (calculatePower(2, (WORLDSTATE_NUMBER_OF_PARAMETERS - i - 1))) * getWorldStateParameterValueByIndex(&currentState, i);
        }

        //nasli smo mozen startpoint, poisci se endpoint, kamor se povezuje akcija
        WorldStateGraphConnection newConnection;
        newConnection.connectingAction = actionFromActionList;
        newConnection.endpointNodeIndex = findEndpointNodeIndex(&adjacencyList[numericNodeIndex], actionFromActionList);
        adjacencyList[numericNodeIndex].adjacentVerticesLinks.Add(newConnection);
    }
    else if (currentParameterValue < 2) {
        setWorldStateParameterValueByIndex(&currentState, propertyIndex, currentParameterValue);
        FindWorldStateStartpointsForActions(actionFromActionList, propertyIndex + 1, currentState);
    }
    else {
        setWorldStateParameterValueByIndex(&currentState, propertyIndex, 0);
        FindWorldStateStartpointsForActions(actionFromActionList, propertyIndex + 1, currentState);

        setWorldStateParameterValueByIndex(&currentState, propertyIndex, 1);
        FindWorldStateStartpointsForActions(actionFromActionList, propertyIndex + 1, currentState);
    }
}


void ACPP_GoapPlanner::fillActionsAdjacencyIndexes() {
    WorldState currentState;
    for (int i = 0; i < actionsList.Num(); i++) {
        FindWorldStateStartpointsForActions(actionsList[i], 0, currentState);
    }
}


void ACPP_GoapPlanner::prepareDijkstra() {
    for (int i = 0; i < adjacencyList.Num(); i++) {
        adjacencyList[i].dijkstraShortestPathFromStart = GOAP_MAX_INT_VALUE;
        adjacencyList[i].dijkstraPreviousNode = nullptr;
        adjacencyList[i].dijkstraVisited = false;
        adjacencyList[i].dijkstraDiscovered = false;
        adjacencyList[i].nodeIndexInBinaryHeapArray = 0;
    }

    thisActorsDijkstraStartNode->dijkstraShortestPathFromStart = 0;
    thisActorsDijkstraStartNode->dijkstraPreviousNode = thisActorsDijkstraStartNode;
}


void ACPP_GoapPlanner::performDijkstra() {
    //ustvari priority queue s samo zacetnim vozliscem
    PriorityQueue priorityQueue = PriorityQueue();
    priorityQueue.insertNewNode(thisActorsDijkstraStartNode);

    while (true) {
        //najdi neobiskano vozlisce z najmanjso razdaljo od zacetnega vozlisca
        if (priorityQueue.heapIsEmpty()) {
            //obiskali smo vsa vozlisca, ki se jih da obiskati
            break;
        }

        WorldStateGraphNode* currentNode = priorityQueue.getMinDistanceNode();

        //obisci neobiskane sosede od currentNode
        for (int i = 0; i < currentNode->adjacentVerticesLinks.Num(); i++) {
            Action* connectingAction = currentNode->adjacentVerticesLinks[i].connectingAction;
            int neighbourNodeIndex = currentNode->adjacentVerticesLinks[i].endpointNodeIndex;

            if (adjacencyList[neighbourNodeIndex].dijkstraVisited == false) {
                int newDistanceToNeighbour = currentNode->dijkstraShortestPathFromStart + connectingAction->getPerformCost(&currentNode->worldState, (currentNode == thisActorsDijkstraStartNode));

                if (newDistanceToNeighbour < adjacencyList[neighbourNodeIndex].dijkstraShortestPathFromStart) {
                    adjacencyList[neighbourNodeIndex].dijkstraShortestPathFromStart = newDistanceToNeighbour;
                    adjacencyList[neighbourNodeIndex].dijkstraPreviousNodeConnectingAction = connectingAction;
                    adjacencyList[neighbourNodeIndex].dijkstraPreviousNode = currentNode;
                    if (adjacencyList[neighbourNodeIndex].dijkstraDiscovered == true) {
                        priorityQueue.fixHeapAtIndex(adjacencyList[neighbourNodeIndex].nodeIndexInBinaryHeapArray);
                    }
                }

                if (adjacencyList[neighbourNodeIndex].dijkstraDiscovered == false) {
                    adjacencyList[neighbourNodeIndex].dijkstraDiscovered = true;
                    priorityQueue.insertNewNode(&adjacencyList[neighbourNodeIndex]);
                }
            }
        }

        //nastavi currentNode na visited
        priorityQueue.deleteRootNode();
        currentNode->dijkstraVisited = true;
    }
}


void ACPP_GoapPlanner::evaluateGoal(int propertyIndex, WorldState currentState, WorldState* goapGoal) {
    int goapGoalPropertyValueAtPropertyIndex = getWorldStateParameterValueByIndex(goapGoal, propertyIndex);

    //rekurzivno poisci vse tocke v adjacencyList, ki ustrezajo danemu cilju
    if (propertyIndex == 0) {
        bestGoalIndexInAdjacencyList = -1;  //ce bo po izvedbi rekurzije ostalo -1, potem cilj iz danega zacetka ni dosegljiv
        bestGoalExecutionCost = GOAP_MAX_INT_VALUE;
    }
    if (propertyIndex == WORLDSTATE_NUMBER_OF_PARAMETERS) {
        //pretvori iz binary v numericen zapis indeksa
        int numericNodeIndex = 0;
        for (int i = 0; i < WORLDSTATE_NUMBER_OF_PARAMETERS; i++) {
            numericNodeIndex += calculatePower(2, (WORLDSTATE_NUMBER_OF_PARAMETERS - i - 1)) * getWorldStateParameterValueByIndex(&currentState, i);
        }

        //na indeksu numericNodeIndex ovrednoti cilj
        int executionCost = adjacencyList[numericNodeIndex].dijkstraShortestPathFromStart;
        if (executionCost < bestGoalExecutionCost) {
            bestGoalIndexInAdjacencyList = numericNodeIndex;
            bestGoalExecutionCost = executionCost;
        }
    }
    else if (goapGoalPropertyValueAtPropertyIndex < 2) {
        setWorldStateParameterValueByIndex(&currentState, propertyIndex, goapGoalPropertyValueAtPropertyIndex);
        evaluateGoal(propertyIndex + 1, currentState, goapGoal);
    }
    else {
        setWorldStateParameterValueByIndex(&currentState, propertyIndex, 0);
        evaluateGoal(propertyIndex + 1, currentState, goapGoal);

        setWorldStateParameterValueByIndex(&currentState, propertyIndex, 1);
        evaluateGoal(propertyIndex + 1, currentState, goapGoal);
    }
}


void ACPP_GoapPlanner::outputBestPath() {
    nextActionToExecute = defaultActionToExecute;

    UE_LOG(LogTemp, Warning, TEXT("%s - BEST GOAL EXECUTION COST: %i"), *this->GetName(), bestGoalExecutionCost);

    if (bestGoalIndexInAdjacencyList == -1) {
        UE_LOG(LogTemp, Warning, TEXT("Cannot reach the selected goal from current world state"));
    }
    else {
        WorldStateGraphNode* nodeToOutput = &adjacencyList[bestGoalIndexInAdjacencyList];
        while (true) {
            if (nodeToOutput == nodeToOutput->dijkstraPreviousNode) {
                break;
            }

            nextActionToExecute = nodeToOutput->dijkstraPreviousNodeConnectingAction;
            FString outputActionName = nodeToOutput->dijkstraPreviousNodeConnectingAction->name;
            int outputActionPerformCost = nodeToOutput->dijkstraPreviousNodeConnectingAction->getPerformCost(&nodeToOutput->dijkstraPreviousNode->worldState, (nodeToOutput->dijkstraPreviousNode == thisActorsDijkstraStartNode));
            UE_LOG(LogTemp, Warning, TEXT("%s - %i"), *outputActionName, outputActionPerformCost);

            nodeToOutput = nodeToOutput->dijkstraPreviousNode;
        }
    }
}





void ACPP_GoapPlanner::initializeGoap() {
    setDefaultActionsCosts();
    fillActionsList();
    WorldState newWorldState;
    fillAdjacencyList(0, newWorldState);
    fillActionsAdjacencyIndexes();

    int length = adjacencyList.Num();
    UE_LOG(LogTemp, Warning, TEXT("%s GOAP INITIALIZED - LENGTH OF ADJACENCY LIST: %i"), *this->GetName(), length);
}


void ACPP_GoapPlanner::GOAPExecuteDijkstra(bool playerInSight, bool playerInSwordRange, bool playerInBowRange, bool playerUsingSword, bool playerUsingShield, bool swordEquipped,
    bool bowEquipped, bool onTheHighGround, bool playerDead, bool doorOpened, bool inDoorRange) {
    WorldState currentWorldState;
    currentWorldState.playerInSight = playerInSight ? 1 : 0;
    currentWorldState.playerInSwordRange = playerInSwordRange ? 1 : 0;
    currentWorldState.playerInBowRange = playerInBowRange ? 1 : 0;
    currentWorldState.playerUsingSword = playerUsingSword ? 1 : 0;
    currentWorldState.playerUsingShield = playerUsingShield ? 1 : 0;
    currentWorldState.swordEquipped = swordEquipped ? 1 : 0;
    currentWorldState.bowEquipped = bowEquipped ? 1 : 0;
    currentWorldState.onTheHighGround = onTheHighGround ? 1 : 0;
    currentWorldState.playerDead = playerDead ? 1 : 0;
    currentWorldState.doorOpened = doorOpened ? 1 : 0;
    currentWorldState.inDoorRange = inDoorRange ? 1 : 0;

    //doloci vozlisce v adjacencyList, ki ustreza currentWorldState
    int startNodeIndex = 0;
    for (int i = 0; i < WORLDSTATE_NUMBER_OF_PARAMETERS; i++) {
        startNodeIndex += (calculatePower(2, (WORLDSTATE_NUMBER_OF_PARAMETERS - i - 1))) * getWorldStateParameterValueByIndex(&currentWorldState, i);
    }
    thisActorsDijkstraStartNode = &adjacencyList[startNodeIndex];

    prepareDijkstra();
    performDijkstra();
}


int ACPP_GoapPlanner::GOAPGetKillPlayerCost(int goalCostMultiplier) {
    WorldState defaultWorldState;
    WorldState goapGoal;
    goapGoal.playerInSight = 2;
    goapGoal.playerInSwordRange = 2;
    goapGoal.playerInBowRange = 2;
    goapGoal.playerUsingSword = 2;
    goapGoal.playerUsingShield = 2;
    goapGoal.swordEquipped = 2;
    goapGoal.bowEquipped = 2;
    goapGoal.onTheHighGround = 2;
    goapGoal.playerDead = 1;
    goapGoal.doorOpened = 2;
    goapGoal.inDoorRange = 2;

    evaluateGoal(0, defaultWorldState, &goapGoal);
    outputBestPath();

    if (bestGoalExecutionCost == GOAP_MAX_INT_VALUE) {
        return bestGoalExecutionCost;
    }
    else {
        return (bestGoalExecutionCost * goalCostMultiplier);
    }
}


void ACPP_GoapPlanner::GOAPTriggerNextActionBlueprintEvent() {
    nextActionToExecute->triggerBlueprintEvent();

    //izpis v igri
    /*
    if (GEngine) {
        FString instanceName = this->GetName();
        FString actionName = nextActionToExecute->name;
        FString message = FString::Printf(TEXT("%s - %s - %i"), *instanceName, *actionName, bestGoalExecutionCost);
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, message);
    }
    */
}





// Sets default values
ACPP_GoapPlanner::ACPP_GoapPlanner()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    initializeGoap();
}

// Called when the game starts or when spawned
void ACPP_GoapPlanner::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ACPP_GoapPlanner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_GoapPlanner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Pocisti pomnilnik https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-actor-lifecycle
void ACPP_GoapPlanner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (int i = 0; i < actionsList.Num(); i++) {
        if (actionsList[i] != nullptr) {
            delete actionsList[i];
            actionsList[i] = nullptr;
        }
    }
    actionsList.Empty();

    if (defaultActionToExecute != nullptr) {
        delete defaultActionToExecute;
        defaultActionToExecute = nullptr;
    }

    UE_LOG(LogTemp, Warning, TEXT("GOAP MEMORY DEALLOCATED"));

    Super::EndPlay(EndPlayReason);
}

