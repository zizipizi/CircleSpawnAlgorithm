// Fill out your copyright notice in the Description page of Project Settings.


#include "DP_AICharacter.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DP_Pickup.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DP_AICharacterController.h"

ADP_AICharacter::ADP_AICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ADP_AICharacterController::StaticClass();

	Handle = CreateDefaultSubobject<USceneComponent>(TEXT("Handle"));
	Handle->SetupAttachment(GetCapsuleComponent());

	this->GetCharacterMovement()->MaxWalkSpeed = 800.0f;

	PickupActor = NULL;
}

void ADP_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADP_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADP_AICharacter::ThrowPickupToCharacter()
{
	if (PickupActor != nullptr)
	{
		PickupActor->bAllowToPickup = false;
		PickupActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		TArray<UStaticMeshComponent*> Components;
		PickupActor->GetComponents<UStaticMeshComponent>(Components);
		UStaticMeshComponent* smComponent = Components[0];
		smComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		smComponent->SetSimulatePhysics(true);
		smComponent->AddImpulse(GetActorForwardVector() * 1000);
		PickupActor = NULL;
	}
}