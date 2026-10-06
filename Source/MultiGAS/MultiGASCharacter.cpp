// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiGASCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MultiGAS.h"

void AMultiGASCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	DeltaTime = DeltaSeconds;
}

AMultiGASCharacter::AMultiGASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
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
	CameraBoom->TargetArmLength = 150.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->FieldOfView = DefaultFOV;
	FollowCamera->bUsePawnControlRotation = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMultiGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMultiGASCharacter::Move);
		EIC->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMultiGASCharacter::Look);

		// Looking
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMultiGASCharacter::Look);

		//Aiming
		EIC->BindAction(AimAction, ETriggerEvent::Ongoing, this, & AMultiGASCharacter::Aim);
		EIC->BindAction(AimAction, ETriggerEvent::None, this, & AMultiGASCharacter::StopAiming);
	}
	else
	{
		UE_LOG(LogMultiGAS, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMultiGASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMultiGASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMultiGASCharacter::Aim(const FInputActionValue& Value)
{
	bool bIsAiming = Value.Get<bool>();

	if (bIsAiming)
		DoAim();
}

void AMultiGASCharacter::StopAiming(const FInputActionValue& Value)
{
	bool bIsAiming = Value.Get<bool>();

	if (!bIsAiming)
		DoStopAiming();
}

void AMultiGASCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
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

void AMultiGASCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMultiGASCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AMultiGASCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AMultiGASCharacter::DoAim()
{
	float TargetFOV = FMath::FInterpTo(FollowCamera->FieldOfView, AimFOV, DeltaTime, 10.f);
	FollowCamera->SetFieldOfView(TargetFOV);
}

void AMultiGASCharacter::DoStopAiming()
{
	float TargetFOV = FMath::FInterpTo(FollowCamera->FieldOfView, DefaultFOV, DeltaTime, 10.f);
	FollowCamera->SetFieldOfView(TargetFOV);
	//FollowCamera->SetFieldOfView(DefaultFOV);
}
