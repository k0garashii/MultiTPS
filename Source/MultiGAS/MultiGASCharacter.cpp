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
#include "CombatPlayerState.h"
#include "MultiGAS.h"

FGenericTeamId AMultiGASCharacter::GetGenericTeamId() const
{
	ACombatPlayerState* CombatPlayerState = GetPlayerState<ACombatPlayerState>();
	if (CombatPlayerState)
	{
		return FGenericTeamId(CombatPlayerState->GetTeamId());
	}
	return FGenericTeamId::NoTeam;
}
void AMultiGASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerWidgetClass)
	{
		PlayerUI = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

		if (PlayerUI)
		{
			UE_LOG(LogTemp, Log, TEXT("Player UI Created"));
			PlayerUI->AddToViewport();
		}
	}

	if (WeaponRef)
		Weapon = WeaponRef->GetDefaultObject<AWeapon>();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitMaxHealth();
	InitHealth();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Stats->GetHealthAttribute()).AddUObject(this, &AMultiGASCharacter::OnHealthChanged);
}

void AMultiGASCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	DeltaTime = DeltaSeconds;
}

AMultiGASCharacter::AMultiGASCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->FieldOfView = DefaultFOV;
	FollowCamera->bUsePawnControlRotation = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	Stats = CreateDefaultSubobject<UPlayerStats>(TEXT("PlayerStats"));
}

void AMultiGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMultiGASCharacter::Move);
		EIC->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMultiGASCharacter::Look);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMultiGASCharacter::Look);
		EIC->BindAction(AimAction, ETriggerEvent::Ongoing, this, & AMultiGASCharacter::Aim);
		EIC->BindAction(AimAction, ETriggerEvent::None, this, & AMultiGASCharacter::StopAiming);
		EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, & AMultiGASCharacter::DoShoot);
	}
	else
		UE_LOG(LogMultiGAS, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
}

void AMultiGASCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (PlayerUI)
		PlayerUI->UpdateHealthBar(GetHealth(), GetMaxHealth());
}

void AMultiGASCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMultiGASCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
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
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AMultiGASCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMultiGASCharacter::DoJumpStart()
{
	Jump();
}

void AMultiGASCharacter::DoJumpEnd()
{
	StopJumping();
}

void AMultiGASCharacter::DoShoot()
{
	if (Weapon)
		Weapon->Shoot(this);
}

void AMultiGASCharacter::DoAim()
{
	float TargetFOV = FMath::FInterpTo(FollowCamera->FieldOfView, AimFOV, DeltaTime, 10.f);
	FollowCamera->SetFieldOfView(TargetFOV);
	GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
	
}

void AMultiGASCharacter::DoStopAiming()
{
	float TargetFOV = FMath::FInterpTo(FollowCamera->FieldOfView, DefaultFOV, DeltaTime, 10.f);
	FollowCamera->SetFieldOfView(TargetFOV);
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}
