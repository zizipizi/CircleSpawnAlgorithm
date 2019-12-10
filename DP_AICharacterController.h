// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DP_AICharacterController.generated.h"

/**
 * 
 */
UCLASS()
class DP_TESTTASK_API ADP_AICharacterController : public AAIController
{
	GENERATED_BODY()

		ADP_AICharacterController();


		void BeginPlay() override;
		void Tick(float DeltaTime) override;

		//AActor* Pickup;

public:

		bool bIsThrowed;
		bool bIsPickedUp;
		bool bIsCachedUp;
		bool bHaveGoalLocation;
		bool bIsAtGoalLocation; 

		FVector CharacterLocation;

		AActor* Pickup;

		void MoveToCharacterLocation();
		void MoveToPickupLocation();
		void FaceToMainCharacter();
		EPathFollowingRequestResult::Type MoveToCircle(FVector location);

		bool CheckDistanceToPlayer();
	
};
