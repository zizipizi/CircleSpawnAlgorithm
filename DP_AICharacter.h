// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DP_Pickup.h"
#include "DP_AICharacter.generated.h"

UCLASS()
class DP_TESTTASK_API ADP_AICharacter : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	ADP_AICharacter();

	UPROPERTY(EditAnywhere)
	USceneComponent* Handle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




	ADP_Pickup* PickupActor;

	void ThrowPickupToCharacter();
};
