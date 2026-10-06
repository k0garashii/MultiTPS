#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "GASStats/WeaponStats.h"
#include "Weapon/Mag.h"
#include "Weapon.generated.h"

class AMultiGASCharacter;

UCLASS()
class MULTIGAS_API AWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWeapon();
	void Shoot(AMultiGASCharacter* Character) const;
	void ApplyHitEffectsToTarget(AActor* Source, AActor* Target, int Damages = 0) const;
	void ApplyGameplayCue(AActor* Source, const FHitResult& HitResult) const;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	int GetAmmo() const { return FMath::RoundToInt(Stats->GetAmmo()); }
	int GetMaxAmmo() const { return FMath::RoundToInt(Stats->GetMaxAmmo()); }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	void InitAmmo() const { Stats->SetAmmo(InitialAmmo); }
	void InitMaxAmmo() const { Stats->SetMaxAmmo(InitialMaxAmmo); }
	void OnAmmoChanged(const FOnAttributeChangeData& Data);
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	int InitialAmmo = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	int InitialMaxAmmo = 30;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	UMag* Mag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	FGameplayTag ImpactCueTag;
	
private:
	UPROPERTY()
	UWeaponStats* Stats = nullptr;
};
