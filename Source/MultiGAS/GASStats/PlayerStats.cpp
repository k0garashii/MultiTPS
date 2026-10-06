#include "PlayerStats.h"

void UPlayerStats::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampValue(Attribute, NewValue);
}

void UPlayerStats::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampValue(Attribute, NewValue);
}

void UPlayerStats::ClampValue(const FGameplayAttribute& Attribute, float& Value) const
{
	if (Attribute == GetHealthAttribute())
		Value = FMath::Clamp(Value, 0.f, this->GetMaxHealth());
}
