#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASStats/BaseSet.h"
#include "WeaponStats.generated.h"

UCLASS()
class MULTIGAS_API UWeaponStats : public UBaseSet
{
	GENERATED_BODY()
	
public:
	UWeaponStats() = default;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Ammo;
	ATTRIBUTE_ACCESSORS(UWeaponStats, Ammo);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS(UWeaponStats, MaxAmmo);
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
private:
	void ClampValue(const FGameplayAttribute& Attribute, float& Value) const;
};
