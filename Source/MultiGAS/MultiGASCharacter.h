#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "GASStats/PlayerStats.h"
#include "Logging/LogMacros.h"
#include "MultiGASCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class AWeapon;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class AMultiGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

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
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	int GetHealth() const { return FMath::RoundToInt(Stats->GetHealth()); }
	int GetMaxHealth() const { return FMath::RoundToInt(Stats->GetMaxHealth()); }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

private:
	void InitializeEffects();
	void InitHealth() const { Stats->SetHealth(InitialHealth); }
	void InitMaxHealth() const { Stats->SetMaxHealth(InitialMaxHealth); }
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	int InitialHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	int InitialMaxHealth = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

protected:
	UPROPERTY(EditAnywhere, Category="Weapon")
	AWeapon* Weapon = nullptr;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	UPlayerStats* Stats = nullptr;
};

