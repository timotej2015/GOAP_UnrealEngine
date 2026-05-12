// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GoapPlannerCustomClasses.h"
#include "CPP_GoapPlanner.generated.h"



UCLASS()
class KARANTANIJA_API ACPP_GoapPlanner : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_GoapPlanner();

	bool isAnEnemy;
	WorldStateGraphNode* thisActorsDijkstraStartNode;

	UPROPERTY(BlueprintReadOnly, Category = "Goal Oriented Action Planning") int GOAP_MAX_INT_VALUE = 2147483647;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPThisSoldierToPlayerDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPThisSoldierToDoorDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPThisSoldierToHighGroundDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPDoorToHighGroundDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPDoorToPlayerDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPHighGroundToPlayerDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPBowAttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPEquipSwordCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPEquipBowCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPSwordAttackCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPShieldBlockCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPBowAttackCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Oriented Action Planning")	int GOAPBreakDoorCost;


	UFUNCTION(BlueprintCallable, Category = "Goal Oriented Action Planning") void GOAPExecuteDijkstra(bool playerInSight, bool playerInSwordRange, bool playerInBowRange, 
		bool playerUsingSword, bool playerUsingShield, bool swordEquipped, bool bowEquipped, bool onTheHighGround, bool playerDead, bool doorOpened, bool inDoorRange);
	UFUNCTION(BlueprintCallable, Category = "Goal Oriented Action Planning") int GOAPGetKillPlayerCost(int goalCostMultiplier);
	UFUNCTION(BlueprintCallable, Category = "Goal Oriented Action Planning") void GOAPTriggerNextActionBlueprintEvent();


	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPEquipSwordEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPEquipBowEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPSwordAttackEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPShieldBlockEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPBowAttackEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPMoveToPlayerSwordRangeEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPMoveToPlayerBowRangeEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPMoveToHighGroundEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPMoveToDoorRangeEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPBreakDoorEvent();
	UFUNCTION(BlueprintImplementableEvent, Category = "Goal Oriented Action Planning") void GOAPDoNothingEvent();

private:
	const int WORLDSTATE_NUMBER_OF_PARAMETERS = 11;

	TArray<Action*> actionsList;
	TArray<WorldStateGraphNode> adjacencyList;

	int bestGoalIndexInAdjacencyList;
	int bestGoalExecutionCost;
	Action* defaultActionToExecute;
	Action* nextActionToExecute;


	int calculatePower(int base, int exponent);
	void setWorldStateParameterValueByIndex(WorldState* worldState, int parameterIndex, int value);
	int getWorldStateParameterValueByIndex(WorldState* worldState, int parameterIndex);

	void setDefaultActionsCosts();	
	void fillActionsList();
	void fillAdjacencyList(int propertyIndex, WorldState currentState);
	void FindWorldStateStartpointsForActions(Action* actionListIndex, int propertyIndex, WorldState currentState);
	void fillActionsAdjacencyIndexes();
	void prepareDijkstra();
	int findEndpointNodeIndex(WorldStateGraphNode* currentNode, Action* connectingAction);
	void performDijkstra();
	void evaluateGoal(int propertyIndex, WorldState currentState, WorldState* goapGoal);
	void outputBestPath();
	void initializeGoap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};