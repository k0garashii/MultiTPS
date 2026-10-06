#include "Weapon/Mag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Weapon/Weapon.h"

void UMag::DecreaseBullet(AWeapon* Weapon) const
{
	if (UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Weapon))
	{
		FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DecreaseBulletEffect, 1.0f, ContextHandle);
		
		if (SpecHandle.IsValid())
		{
			SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), SourceASC);
		}
	}
}
