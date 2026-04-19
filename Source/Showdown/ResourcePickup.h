// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourcePickup.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Health UMETA(DisplayName = "Health"),
	Ammo UMETA(DisplayName = "Ammo"),
	Shield UMETA(DisplayName = "Shield")
};


UCLASS()
class SHOWDOWN_API AResourcePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourcePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RestoreAmount;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	EPickupType PickupType;

	

public:	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
