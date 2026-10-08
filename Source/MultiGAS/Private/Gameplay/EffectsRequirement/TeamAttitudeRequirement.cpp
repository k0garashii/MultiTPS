#include "Gameplay/EffectsRequirement/TeamAttitudeRequirement.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

bool UTeamAttitudeRequirement::CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect,
                                                                     const FGameplayEffectSpec& Spec, UAbilitySystemComponent* ASC) const
{
	if (!ASC) return false;

	AActor* TargetActor = ASC->GetOwnerActor();
	const FGameplayEffectContextHandle& Context = Spec.GetEffectContext();
	AActor* SourceActor = Context.GetInstigator();
	
	if (!TargetActor || !SourceActor)
	{
		return false;
	}
	
	IGenericTeamAgentInterface* SourceTeamAgent = Cast<IGenericTeamAgentInterface>(SourceActor);
	
	if (!SourceTeamAgent)
	{
		return false;
	}
	return SourceTeamAgent->GetTeamAttitudeTowards(*TargetActor) == RequiredAttitude;
}
