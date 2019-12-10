// Fill out your copyright notice in the Description page of Project Settings.


#include "DP_Pickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DP_MainCharacter.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ADP_Pickup::ADP_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/DP_TestTask/Meshes/Sphere"));

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetStaticMesh(Mesh.Object);
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ADP_Pickup::OnOverlap);

	RootComponent = PickupMesh;
	SphereCollision->SetupAttachment(PickupMesh);
}

// Called when the game starts or when spawned
void ADP_Pickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADP_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADP_Pickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (bAllowToPickup)
	{
		if (Cast<ADP_AICharacter>(OtherActor))
		{
			ADP_AICharacter* pickedCharacter = Cast<ADP_AICharacter>(OtherActor);
			PickupMesh->SetSimulatePhysics(false);
			PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AttachToComponent(pickedCharacter->Handle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			pickedCharacter->PickupActor = this;
			bAllowToPickup = false;
			Cast<ADP_TestTaskGameModeBase>(GetWorld()->GetAuthGameMode())->PickupPicked();
			UE_LOG(LogTemp, Warning, TEXT("RightSOL"));
		}
	}
}

