#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseSet.h"
#include "PlayerStats.generated.h"

UCLASS()
class MULTIGAS_API UPlayerStats : public UBaseSet
{
	GENERATED_BODY()
	
public:
	UPlayerStats() = default;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPlayerStats, Health);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPlayerStats, MaxHealth);
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
private:
	void ClampValue(const FGameplayAttribute& Attribute, float& Value) const;
};
