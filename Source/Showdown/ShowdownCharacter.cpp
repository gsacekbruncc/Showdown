// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShowdownCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Showdown.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AShowdownCharacter::AShowdownCharacter()
{
	LaserBeamNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserBeamNiagara"));
	LaserBeamNiagara->SetupAttachment(RootComponent);
	LaserBeamNiagara->SetAutoActivate(false);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;



	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AShowdownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShowdownCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AShowdownCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShowdownCharacter::Look);
		//EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Started, this, &AShowdownCharacter::StartFreeLook);
		//EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Completed, this, &AShowdownCharacter::StopFreeLook);
		EnhancedInputComponent->BindAction(AimDownSightsAction, ETriggerEvent::Started, this, &AShowdownCharacter::StartAimDownSights);
		EnhancedInputComponent->BindAction(AimDownSightsAction, ETriggerEvent::Completed, this, &AShowdownCharacter::StopAimDownSights);

	}
	else
	{
		UE_LOG(LogShowdown, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AShowdownCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AShowdownCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	const bool bHasMouseInput = !LookAxisVector.IsNearlyZero();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AShowdownCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		FRotator Rotation;

		if (bFreeLookActive)
		{
			Rotation = StoredControlRotation;
		}
		else
		{
			Rotation = GetController()->GetControlRotation();
		}
		
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AShowdownCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AShowdownCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AShowdownCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AShowdownCharacter::StartAimDownSights()
{
	if (!bIsJumping)
	{
		LaserBeamNiagara->Activate();
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		bAimDownSightsActive = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	
}
void AShowdownCharacter::StopAimDownSights()
{
	LaserBeamNiagara->Deactivate();
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	bAimDownSightsActive = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AShowdownCharacter::SetIsJumping(bool IsJumping)
{
	bIsJumping = IsJumping;
}

void AShowdownCharacter::StartADSCamera(float DeltaTime)
{
	FRotator NewCameraRotation = FMath::RInterpTo(CameraBoom->GetRelativeRotation(), ADSTargetBoomRotation, DeltaTime, 10.f);
	float NewCameraArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, ADSTargetArmLength, DeltaTime, 10.f);

	CameraBoom->SetRelativeRotation(NewCameraRotation);
	CameraBoom->TargetArmLength = NewCameraArmLength;
}

void AShowdownCharacter::StopADSCamera(float DeltaTime)
{
	FRotator NewCameraRotation = FMath::RInterpTo(CameraBoom->GetRelativeRotation(), DefaultTargetBoomRotation, DeltaTime, 10.f);
	float NewCameraArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, DefaultTargetArmLength, DeltaTime, 10.f);

	CameraBoom->SetRelativeRotation(NewCameraRotation);
	CameraBoom->TargetArmLength = NewCameraArmLength;
}

void AShowdownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Handles ADS
	if (bAimDownSightsActive && !bIsJumping && GetController())
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = GetController()->GetControlRotation();
		TargetRotation.Pitch = 0.f;
		TargetRotation.Roll = 0.f;

		FRotator NewActorRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.f);
		SetActorRotation(NewActorRotation);
		
		FVector Start = FVector::ZeroVector;
		FVector End = FVector(1000.0f, 0.0f, 0.0f);

		LaserBeamNiagara->SetVectorParameter(TEXT("BeamStart"), Start);
		LaserBeamNiagara->SetVectorParameter(TEXT("BeamEnd"), End);

		StartADSCamera(DeltaTime);

	}
	else if(bIsJumping)
	{
		StopAimDownSights();
		StopADSCamera(DeltaTime);
	}
	else
	{
		StopADSCamera(DeltaTime);
	}

	bool bMouseMoved = false;
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		float DX = 0.f, DY = 0.f;
		PC->GetInputMouseDelta(DX, DY);

		bMouseMoved = !FMath::IsNearlyZero(DX, 1.f);

	}
	
	// For old free look feature
	// Return camera to stored rotation if we stopped freelooking but haven't returned to the camera yet, and stop returning if the mouse moves
	/*if (bReturnCamera && GetController())
	{
		if (!bFreeLookActiveBeforeADS)
		{
			FRotator CurrentRotation = GetController()->GetControlRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, ReturnTargetRotation, DeltaTime, 7.f);
			GetController()->SetControlRotation(NewRotation);
			if (bMouseMoved)
			{
				bReturnCamera = false;
			}
		}
		
	}*/
}

//Feature removed
//void AShowdownCharacter::StartFreeLook()
//{
//	if (!bAimDownSightsActive)
//	{
//		bFreeLookActiveBeforeADS = true;
//	}
//	bFreeLookActive = true;
//	bReturnCamera = false;
//	StoredControlRotation = GetController()->GetControlRotation();
//}

//Feature removed
//void AShowdownCharacter::StopFreeLook()
//{
//	bFreeLookActiveBeforeADS = false;
//	bFreeLookActive = false;
//	bReturnCamera = true;
//	ReturnTargetRotation = StoredControlRotation;
//	if (!bAimDownSightsActive)
//	{
//		bFreeLookActive = false;
//		bReturnCamera = true;
//		ReturnTargetRotation = StoredControlRotation;
//	}
//	else
//	{
//		bFreeLookActive = false;
//		bReturnCamera = false;
//		StoredControlRotation = GetController()->GetControlRotation();
//
//	}
//}

