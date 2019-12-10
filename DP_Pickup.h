// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_Pickup.generated.h"

UCLASS()
class DP_TESTTASK_API ADP_Pickup : public AActor
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PickupMesh;
		UPROPERTY(EditAnywhere)
		class USphereComponent* SphereCollision;
	
public:	
	// Sets default values for this actor's properties
	ADP_Pickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bIsPickedUp;
	bool bAllowToPickup;
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
