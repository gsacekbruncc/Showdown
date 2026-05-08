// Fill out your copyright notice in the Description page of Project Settings.
#include "ResourcePickup.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "ShowdownCharacter.h"



// Sets default values
AResourcePickup::AResourcePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(Root);

	PickupMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh2"));
	PickupMesh2->SetupAttachment(PickupMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Root);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AResourcePickup::OnOverlapBegin);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComponent"));
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 90.0f, 0.0f);

}

// Called when the game starts or when spawned
void AResourcePickup::BeginPlay()
{
	Super::BeginPlay();
}

void AResourcePickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AResourcePickup, IsActive);
}

void AResourcePickup::OnRep_IsActive()
{
	SetActorHiddenInGame(!IsActive);
	SetActorEnableCollision(IsActive);
}

void AResourcePickup::SetPickupActive(bool NewActive)
{
	IsActive = NewActive;

	SetActorHiddenInGame(!IsActive);
	SetActorEnableCollision(IsActive);
}

void AResourcePickup::ReactivatePickup()
{
	if (!HasAuthority())
	{
		return;
	}

	SetPickupActive(true);
}

void AResourcePickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!HasAuthority())
	{
		return;
	}

	if (!IsActive)
	{
		return;
	}

	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	AShowdownCharacter* PlayerCharacter = Cast<AShowdownCharacter>(OtherActor);
	if (!PlayerCharacter)
	{
		return;
	}

	bool bPickedUp = false;

	switch (PickupType)
	{
		case EPickupType::Health:
			if (PlayerCharacter->GetHPRemaining() < PlayerCharacter->GetHPCapacity())
			{
				PlayerCharacter->RestoreHP(RestoreAmount);
				PlayerCharacter->ClientPlayHealthUpAudio();
				bPickedUp = true;
			}
			break;

		case EPickupType::Shield:
			if (PlayerCharacter->GetShieldRemaining() < PlayerCharacter->GetShieldCapacity())
			{
				PlayerCharacter->RestoreShield(RestoreAmount);
				PlayerCharacter->ClientPlayShieldUpAudio();
				bPickedUp = true;
			}
			break;

		case EPickupType::Ammo:
			if (PlayerCharacter->GetAmmoCapacity() < PlayerCharacter->GetMaxAmmoCapacity())
			{
				PlayerCharacter->RestoreAmmo(RestoreAmount);
				PlayerCharacter->ClientPlayAmmoUpAudio();
				bPickedUp = true;
			}
			break;
	}

	if (bPickedUp)
	{
		SetPickupActive(false);

		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AResourcePickup::ReactivatePickup, RespawnTime, false);
	}
}


