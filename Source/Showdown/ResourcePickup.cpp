// Fill out your copyright notice in the Description page of Project Settings.
#include "ResourcePickup.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
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

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Root);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AResourcePickup::OnOverlapBegin);


}



// Called when the game starts or when spawned
void AResourcePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AResourcePickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!HasAuthority())
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

	switch (PickupType)
	{
		case EPickupType::Health:
			PlayerCharacter->RestoreHP(RestoreAmount);
			break;
		case EPickupType::Shield:
			PlayerCharacter->RestoreShield(RestoreAmount);
			break;
		case EPickupType::Ammo:
			PlayerCharacter->RestoreAmmo(RestoreAmount);
			break;

	}
	Destroy();
}


