// Fill out your copyright notice in the Description page of Project Settings.


#include "DP_MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet//GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "DP_Pickup.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ADP_MainCharacter::ADP_MainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Handle = CreateDefaultSubobject<USceneComponent>(TEXT("Handle"));

	Camera->SetupAttachment(GetCapsuleComponent());
	Handle->SetupAttachment(Camera);
	Camera->bUsePawnControlRotation = true;
	traceDistance = 300.0f;

	PickupActor = NULL;
	isGrabbed = false;

	baseLookUpRate = 45.0f;
	baseTurnRate = 45.0f;
	traceDistance = 300.0f;
}

// Called when the game starts or when spawned
void ADP_MainCharacter::BeginPlay()
{
	Super::BeginPlay();

	Gamemode = Cast<ADP_TestTaskGameModeBase>(GetWorld()->GetAuthGameMode());


}

// Called every frame
void ADP_MainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADP_MainCharacter::MoveForward(float scaleValue)
{
	AddMovementInput(GetActorForwardVector(), scaleValue);
}

void ADP_MainCharacter::MoveRight(float scaleValue)
{
	AddMovementInput(GetActorRightVector(), scaleValue);
}

void ADP_MainCharacter::Turn(float scaleValue)
{
	AddControllerYawInput(scaleValue * baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADP_MainCharacter::LookUp(float scaleValue)
{
	AddControllerPitchInput(scaleValue * baseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADP_MainCharacter::GrabItem()
{
	FHitResult HitResult(ForceInit);

	FVector startTrace = Camera->GetComponentLocation();
	FVector endTrace = Camera->GetForwardVector() * traceDistance + startTrace;

	FCollisionQueryParams TraceParams;

	if (!isGrabbed)
	{
		bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, startTrace, endTrace, ECC_Visibility, TraceParams);
		DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Red, true);
		if (bIsHit) 
		{
			PickupActor = Cast<ADP_Pickup>(HitResult.GetActor());
			if (PickupActor != nullptr)
			{
				isGrabbed = true;
				TArray<UStaticMeshComponent*> Components;
				PickupActor->GetComponents<UStaticMeshComponent>(Components);
				UStaticMeshComponent* Component = Components[0];
				Component->SetSimulatePhysics(false);
				Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				PickupActor->AttachToComponent(Handle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);			
				//UE_LOG(LogTemp, Warning, TEXT("HIT"));
			}		
		}
	}
}

void ADP_MainCharacter::ThrowItem()
{
	if (isGrabbed)
	{
		if (PickupActor != nullptr)
		{
			isGrabbed = false;
			PickupActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PickupActor->SetActorEnableCollision(true);
			TArray<UStaticMeshComponent*> Components;
			PickupActor->GetComponents<UStaticMeshComponent>(Components);
			UStaticMeshComponent* Component = Components[0];
			Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Component->SetSimulatePhysics(true);
			Component->AddImpulse(Camera->GetForwardVector() * 1500, NAME_None, true);		
			Gamemode->PickupThrowed();
			Cast<ADP_Pickup>(PickupActor)->bAllowToPickup = true;			
			//GetCharacterMovement()->DisableMovement();
			//UE_LOG(LogTemp, Warning, TEXT("Throwed"));
		}

	}
}


// Called to bind functionality to input
void ADP_MainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent)

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &ADP_MainCharacter::GrabItem);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &ADP_MainCharacter::ThrowItem);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADP_MainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADP_MainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ADP_MainCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUP", this, &ADP_MainCharacter::LookUp);

}

