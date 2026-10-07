#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Weapon/Weapon.h"
#include "MultiGASCharacter.generated.h"

class UPlayerStats;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class AMultiGASCharacter : public ACharacter,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMultiGASCharacter();	
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoAim();
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoStopAiming();
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	// IGenericTeamAgentInterface
	UFUNCTION(BlueprintCallable, Category = "Team")
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void Tick(float DeltaSeconds) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	void StopAiming(const FInputActionValue& Value);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float AimFOV = 30.0f;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float DefaultFOV = 90.0f;
	
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeapon> Weapon = nullptr;
	
private: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY()
	UPlayerStats* Stats = nullptr;
	
	float DeltaTime = 0.f;
};

