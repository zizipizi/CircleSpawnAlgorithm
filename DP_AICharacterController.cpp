// Fill out your copyright notice in the Description page of Project Settings.


#include "DP_AICharacterController.h"
#include "DP_AICharacter.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "DP_MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DP_Pickup.h"
#include "GameFramework/CharacterMovementComponent.h"

ADP_AICharacterController::ADP_AICharacterController()
{
	PrimaryActorTick.bCanEverTick = true;

}


void ADP_AICharacterController::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> PickupActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADP_Pickup::StaticClass(), PickupActor);
	if (PickupActor.Num() != 0)
	{
		Pickup = PickupActor[0];
		UE_LOG(LogTemp, Warning, TEXT("PICKUP FINDEDFromController, %s"), *PickupActor[0]->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PICKUP NOTFINDED"));
	}
}

void ADP_AICharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsThrowed)
	{
		if (bIsPickedUp)
		{
			if (bIsCachedUp)
			{
				if (bHaveGoalLocation)
				{
					EPathFollowingRequestResult::Type moveRes = MoveToLocation(CharacterLocation, 1.0f, false, false);
					if (moveRes == EPathFollowingRequestResult::AlreadyAtGoal)
					{
						bIsThrowed = false;
						Cast<ADP_TestTaskGameModeBase>(GetWorld()->GetAuthGameMode())->IncrementAllPositionsInACircle();
						bIsPickedUp = false;
						bHaveGoalLocation = false;
						bIsCachedUp = false;
						UE_LOG(LogTemp, Warning, TEXT("AlreadyAtGoal"));

					}
				}
				else
				{
					Cast<ADP_TestTaskGameModeBase>(GetWorld()->GetAuthGameMode())->FindGoalLocation();
				}	
			}
			else
			{
				if (CheckDistanceToPlayer())
				{
					UE_LOG(LogTemp, Warning, TEXT("CHECKDISTANCE"));
					Cast<ADP_MainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetCharacterMovement()->DisableMovement();
					Cast<ADP_TestTaskGameModeBase>(GetWorld()->GetAuthGameMode())->CharacterCachedUp();
				}	
				else
				{
					MoveToCharacterLocation();
				}
			}
		}			
		else 
		{
			MoveToPickupLocation();
		}
	}
	else
	{
		FaceToMainCharacter();
	}

}

void ADP_AICharacterController::MoveToPickupLocation()
{
		Pickup = Cast<ADP_Pickup>(Pickup);
		MoveToLocation(FVector(Pickup->GetActorLocation().X, Pickup->GetActorLocation().Y, 0.0f), 1.0f, true, true);
}

void ADP_AICharacterController::MoveToCharacterLocation()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	MoveToLocation(PlayerLocation, 1.0f ,false, true);
}

void ADP_AICharacterController::FaceToMainCharacter()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	AController::GetPawn()->SetActorRotation(FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(AController::GetPawn()->GetActorLocation(), PlayerLocation).Yaw,0.0f));
}

bool ADP_AICharacterController::CheckDistanceToPlayer()
{

	FVector playerLocation = Cast<ADP_MainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetActorLocation();
	FVector AICharacterLocation = AController::GetPawn()->GetActorLocation();
	float Distance = UKismetMathLibrary::Vector_Distance(playerLocation, AICharacterLocation);
	if (Distance < 200.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

EPathFollowingRequestResult::Type ADP_AICharacterController::MoveToCircle(FVector location)
{
	MoveToLocation(location, 1.0, false, true);
	return EPathFollowingRequestResult::Type();
}