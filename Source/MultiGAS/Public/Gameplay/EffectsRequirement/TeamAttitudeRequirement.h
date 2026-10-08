

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCustomApplicationRequirement.h"
#include "GenericTeamAgentInterface.h"
#include "TeamAttitudeRequirement.generated.h"


UCLASS()
class MULTIGAS_API UTeamAttitudeRequirement : public UGameplayEffectCustomApplicationRequirement
{
	GENERATED_BODY()
public :
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Requirement")
	TEnumAsByte<ETeamAttitude::Type> RequiredAttitude;
	virtual bool CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect, const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const override;
};
