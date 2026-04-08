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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LaserBeam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* LaserBeamNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAcess = "true"))
	UNiagaraSystem* LaserBeamNiagaraSystem;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAcess = "true"))
	bool bAimDownSightsActive = false;

	bool bIsJumping = false;

	bool bFreeLookActive = false;
	bool bReturnCamera = false;
	bool bFreeLookActiveBeforeADS = false;

	float ADSTargetArmLength = 565.0;
	float DefaultTargetArmLength = 1167.0;
	float CameraInterpSpeed;
	
	FRotator StoredControlRotation;
	FRotator ReturnTargetRotation;

	FRotator ADSTargetBoomRotation = FRotator(-34.000000, 0.000000, 0.000000);
	FRotator DefaultTargetBoomRotation = FRotator(-44.000000, 0.000000, 0.000000);
	
public:

	/** Constructor */
	AShowdownCharacter();	

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
	bool isAimDownSightsActive() const { return bAimDownSightsActive; }

	UFUNCTION(BlueprintCallable, Category = "Jump")
	void SetIsJumping(bool bIsJumping);

	void StartFreeLook();
	void StopFreeLook();
	void StartAimDownSights();
	void StopAimDownSights();
	void StartADSCamera(float DeltaTime);
	void StopADSCamera(float DeltaTime);

	

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

