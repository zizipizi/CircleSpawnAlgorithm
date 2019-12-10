// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DP_TestTaskGameModeBase.h"
#include "DP_MainCharacter.generated.h"

UCLASS()
class DP_TESTTASK_API ADP_MainCharacter : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;
		UPROPERTY(EditAnywhere)
		USceneComponent* Handle;

public:
	// Sets default values for this character's properties
	ADP_MainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float traceDistance;
	float baseTurnRate;
	float baseLookUpRate;
	bool isGrabbed;
	AActor* PickupActor;

	ADP_TestTaskGameModeBase* Gamemode;


	void MoveForward(float scaleValue);
	void MoveRight(float scaleValue);
	void Turn(float scaleValue);
	void LookUp(float scaleValue);
	void GrabItem();
	void ThrowItem();

};
