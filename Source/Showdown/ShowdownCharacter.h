// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ShowdownCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UStaticMeshComponent;
class UNiagaraComponent;
class UNiagaraSystem;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AShowdownCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** FreeLook Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FreeLookAction;
	
	/** ADS Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimDownSightsAction;

	/** Fire Weapon Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* UnlockPitchAction;

	/** Reload Weapon Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LaserBeam;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* LaserBeamOrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* LaserBeamNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAcess = "true"))
	UNiagaraSystem* LaserBeamNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAcess = "true"))
	FRotator ADSTargetBoomRotation = FRotator(-34.000000, 0.000000, 0.000000);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAcess = "true"))
	FRotator DefaultTargetBoomRotation = FRotator(-44.000000, 0.000000, 0.000000);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* HealthUpAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* ShieldUpAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* AmmoUpAudio;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAcess = "true"))
	bool bIsJumping = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAcess = "true"))
	bool bAimDownSightsActive = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAcess = "true"))
	bool bIsReloading = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAcess = "true"))
	bool CanReload = false;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAcess = "true"))
	bool CanFire = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAcess = "true"))
	bool PitchLocked = true;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAcess = "true"))
	int AmmoRemaining = 30;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAcess = "true"))
	int AmmoCapacity = 30;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAcess = "true"))
	int MaxAmmoCapacity = 90;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAcess = "true"))
	int MagazineCapacity = 30;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAcess = "true"))
	float HPRemaining = 100;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAcess = "true"))
	float HPCapacity = 100;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAcess = "true"))
	float ShieldRemaining = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAcess = "true"))
	float ShieldCapacity = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAcess = "true"))
	float ADSTargetArmLength = 565.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAcess = "true"))
	float DefaultTargetArmLength = 1167.0;

	




	

	bool LocalPitchLocked = true;
	
	bool bIsFiring = false;
	bool bFreeLookActive = false;
	bool bReturnCamera = false; 
	bool bFreeLookActiveBeforeADS = false;

	int AmmoRequested;
	int AmmoTransfered;
	int AmmoCapacityRemaining;
	int StartingAmmoCapacity = 30;

	/*float ADSTargetArmLength = 565.0;
	float DefaultTargetArmLength = 1167.0;*/
	float CameraInterpSpeed;
	
	FRotator StoredControlRotation;
	FRotator ReturnTargetRotation;

	/*FRotator ADSTargetBoomRotation = FRotator(-34.000000, 0.000000, 0.000000);
	FRotator DefaultTargetBoomRotation = FRotator(-44.000000, 0.000000, 0.000000);*/
	
public:

	/** Constructor */
	AShowdownCharacter();	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	UFUNCTION(BlueprintPure, Category="State")
	bool GetIsAimDownSightsActive() const { return bAimDownSightsActive; }

	UFUNCTION(BlueprintCallable, Category = "Jump")
	void SetIsJumping(bool Jumping);

	UFUNCTION(BlueprintCallable, Category = "Fire")
	bool GetIsFiring() const { return bIsFiring; }

	UFUNCTION(BlueprintCallable, Category = "Fire")
	bool GetIsReloading() const { return bIsReloading; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Reload();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void ServerReload();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopReloading();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void ServerStopReloading();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Fire();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void ServerFire();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void ServerAimDownSights();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Combat")
	void ServerStopAimDownSights();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Status")
	void ServerApplyPointDamage(float DamageAmount);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Status")
	void ServerRespawn();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRestoreHP(float amount);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRestoreShield(float amount);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRestoreAmmo(float amount);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerUnlockPitch();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerLockPitch();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientUpdateAmmo(int NewAmmoRemaining, int NewAmmoCapacity, bool NewIsReloading);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Client_PickUpAmmo(int NewAmmoRemaining, int NewAmmoCapacity, bool NewIsReloading);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientUpdateHP(float NewHPRemaining, float NewHPCapacity);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Client_PickUpHP(float NewHPRemaining, float NewHPCapacity);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientUpdateShield(float NewShieldRemaining, float NewShieldCapacity);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Client_PickUpShield(float NewShieldRemaining, float NewShieldCapacity);

	UFUNCTION()
	float GetHPRemaining() const { return HPRemaining; }

	UFUNCTION()
	float GetHPCapacity() const { return HPCapacity; }

	UFUNCTION()
	float GetShieldRemaining() const { return ShieldRemaining; }

	UFUNCTION()
	float GetShieldCapacity() const { return ShieldCapacity; }

	UFUNCTION()
	int GetAmmoRemaining() const { return AmmoRemaining; }
	
	UFUNCTION()
	int GetAmmoCapacity() const { return AmmoCapacity; }

	UFUNCTION()
	int GetMaxAmmoCapacity() const { return MaxAmmoCapacity; }

	UFUNCTION(BlueprintImplementableEvent)
	void PlayHealthUpAudio();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayShieldUpAudio();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAmmoUpAudio();

	UFUNCTION(Client, Unreliable)
	void ClientPlayHealthUpAudio();

	UFUNCTION(Client, Unreliable)
	void ClientPlayShieldUpAudio();

	UFUNCTION(Client, Unreliable)
	void ClientPlayAmmoUpAudio();

	void LockPitch();
	void UnlockPitch();

	void RestoreHP(float amount);
	void RestoreShield(float amount);
	void RestoreAmmo(float amount);
	
	void ApplyPointDamage(float DamageAmount);
	
	void AimDownSights();
	void StopAimDownSights();
	
	void StartADSCamera(float DeltaTime);
	void StopADSCamera(float DeltaTime);

	

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

