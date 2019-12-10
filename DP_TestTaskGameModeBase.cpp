// Fill out your copyright notice in the Description page of Project Settings.


#include "DP_TestTaskGameModeBase.h"
#include "UObject//ConstructorHelpers.h"
#include "DP_MainCharacter.h"
#include "DP_AICharacter.h"
#include "DP_AICharacterController.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "DP_HUD.h"
#include "GameFramework/CharacterMovementComponent.h"

ADP_TestTaskGameModeBase::ADP_TestTaskGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> MainCharacter(TEXT("/Game/DP_TestTask/BaseGame/Blueprints/DP_MainCharacter"));
	static ConstructorHelpers::FClassFinder<ACharacter> AIPawn(TEXT("/Game/DP_TestTask/BaseGame/Blueprints/DP_AICharacter"));



	DefaultPawnClass = MainCharacter.Class;
	PawnToSpawn = AIPawn.Class;
	HUDClass = ADP_HUD::StaticClass();

	radius = 200.0f;
	numberOfCharacters = 10;
	aiCharacterIndex = 1;
}


void ADP_TestTaskGameModeBase::BeginPlay()
{
	SpawnBots();
}

void ADP_TestTaskGameModeBase::FindMinMaxForAI()
{
	TArray<float> vectorValues;
	float minValue;
	int indexOfMinValue;
	for (int i = 0; i < numberOfCharacters; i++)
	{
		vectorValues.AddUnique(UKismetMathLibrary::Vector_Distance(myCharacters[i]->GetActorLocation(), finalAILocation[i]));
		UKismetMathLibrary::MinOfFloatArray(vectorValues, indexOfMinValue, minValue);
	}
}

FVector ADP_TestTaskGameModeBase::CalculateCircle(AActor* actor, int32 index, int32 numberOfCharacters, float radius)
{
	FVector loc;
	loc.X = ((UKismetMathLibrary::DegCos((360 / numberOfCharacters) * index))*radius);
	loc.Y = ((UKismetMathLibrary::DegSin((360 / numberOfCharacters) * index))*radius);
	loc.Z = 0.0f;

	return actor->GetActorLocation() + loc;
}

void ADP_TestTaskGameModeBase::SpawnBots()
{
	FActorSpawnParameters SpawnParams;

	APawn* Player = Cast<ADP_MainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FVector playerLocation = Player->GetActorLocation();
	for (int i = 0; i < numberOfCharacters; i++)
	{
		FVector spawnLocation = CalculateCircle(Player, i, numberOfCharacters, radius);
		myCharacters.AddUnique(GetWorld()->SpawnActor<APawn>(PawnToSpawn, spawnLocation, UKismetMathLibrary::FindLookAtRotation(spawnLocation, playerLocation)));
		UE_LOG(LogTemp, Warning, TEXT("CurPawn = %s"), *myCharacters[i]->GetName());
		myControllers.AddUnique(myCharacters[i]->GetController());
	}
}

void ADP_TestTaskGameModeBase::FindPlaceInACircle()
{
	APawn* Player = Cast<ADP_MainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ADP_AICharacterController* controller;

	for (int i = 0; i < numberOfCharacters; i++)
	{
		controller = Cast<ADP_AICharacterController>(myControllers[i]);
		FVector Dest = CalculateCircle(Player, i, numberOfCharacters, radius);
		finalAILocation.AddUnique(Dest);
		controller->CharacterLocation = Dest;
		controller->bHaveGoalLocation = true;
	}
}

void ADP_TestTaskGameModeBase::ReturnToPositions()
{
	APawn* Player = Cast<ADP_MainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ADP_AICharacterController* controller;
	FVector playerLocation = Player->GetActorLocation();
	for (int i = 0; i < numberOfCharacters; i++)
	{
		controller = Cast<ADP_AICharacterController>(myControllers[i]);
		controller->bIsThrowed = false;
		controller->bIsPickedUp = false;
		controller->bIsAtGoalLocation = false;
		controller->bHaveGoalLocation = false;
		controller->bIsCachedUp = false;
	}
	Cast<ADP_MainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ADP_TestTaskGameModeBase::PickupThrowed()
{
	ADP_AICharacterController* controller;
	for (int i = 0; i < numberOfCharacters; i++)
	{
		controller = Cast<ADP_AICharacterController>(myControllers[i]);
		controller->bIsThrowed = true;
	}
}

void ADP_TestTaskGameModeBase::PickupPicked()
{
	ADP_AICharacterController* controller;
	for (int i = 0; i < numberOfCharacters; i++)
	{
		controller = Cast<ADP_AICharacterController>(myControllers[i]);
		controller->bIsPickedUp = true;
	}
}

void ADP_TestTaskGameModeBase::CharacterCachedUp()
{
	ADP_AICharacterController* controller;
	for (int i = 0; i < numberOfCharacters; i++)
	{
		controller = Cast<ADP_AICharacterController>(myControllers[i]);
		controller->bIsCachedUp = true;
	}
}

void ADP_TestTaskGameModeBase::FindGoalLocation()
{
	FindPlaceInACircle();
}

void ADP_TestTaskGameModeBase::IncrementAllPositionsInACircle()
{
	if (aiCharacterIndex == numberOfCharacters)
	{
		ADP_AICharacter* character;
		for (int i = 0; i < numberOfCharacters; i++)
		{
			character = Cast<ADP_AICharacter>(myCharacters[i]);
			character->ThrowPickupToCharacter();
			Cast<ADP_MainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		aiCharacterIndex = 1;
	}
	else
	{
		aiCharacterIndex = aiCharacterIndex + 1;
	}
	UE_LOG(LogTemp, Warning, TEXT("INT =, %d"), aiCharacterIndex);
}