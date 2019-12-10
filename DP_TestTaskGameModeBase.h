// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DP_AICharacterController.h"
#include "DP_AICharacter.h"
#include "DP_TestTaskGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DP_TESTTASK_API ADP_TestTaskGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		ADP_TestTaskGameModeBase();

protected:
	void BeginPlay() override;
	UFUNCTION()
	FVector CalculateCircle(AActor* actor, int32 index, int32 numberOfCharacters, float radius);

	int32 numberOfCharacters;
	float radius;
	void SpawnBots();

	TSubclassOf<APawn> PawnToSpawn;
	TArray<APawn*> myCharacters;
	TArray<AController*> myControllers;
	TArray<FVector> finalAILocation;
	int32 aiCharacterIndex;


public:
	void FindMinMaxForAI();
	void FindPlaceInACircle();
	void ReturnToPositions();
	void PickupThrowed();
	void PickupPicked();
	void CharacterCachedUp();
	void FindGoalLocation();
	void IncrementAllPositionsInACircle();
	void ResetPosition();




};
