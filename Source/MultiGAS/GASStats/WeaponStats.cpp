#include "GASStats/WeaponStats.h"

void UWeaponStats::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampValue(Attribute, NewValue);
}

void UWeaponStats::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampValue(Attribute, NewValue);
}

void UWeaponStats::ClampValue(const FGameplayAttribute& Attribute, float& Value) const
{
	if (Attribute == GetAmmoAttribute())
		Value = FMath::Clamp(Value, 0.f, this->GetMaxAmmo());
}
