#include "Weapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MultiGASCharacter.h"
#include "Camera/CameraComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	Stats = CreateDefaultSubobject<UWeaponStats>("WeaponStats");
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	InitMaxAmmo();
	InitAmmo();
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Stats->GetAmmoAttribute()).AddUObject(this, &AWeapon::OnAmmoChanged);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::OnAmmoChanged(const FOnAttributeChangeData& Data)
{
	
}

void AWeapon::Shoot(AMultiGASCharacter* Character) const
{
	FHitResult HitResult;
	float TraceDistance = 1000000.f;
	FVector Start = Character->GetFollowCamera()->GetComponentLocation();
	FVector End = Start + Character->GetFollowCamera()->GetForwardVector() * TraceDistance;
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		if (AMultiGASCharacter* Enemy = Cast<AMultiGASCharacter>(HitResult.GetActor()))
		{
			ApplyHitEffectsToTarget(Character, Enemy, Mag->GetBullet()->GetDamages());
			ApplyGameplayCue(Character, HitResult);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Shoot"));
}

void AWeapon::ApplyHitEffectsToTarget(AActor* Source, AActor* Target, int Damages) const
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Source);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	
	if (SourceASC && TargetASC)
	{
		FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
		for (const TSubclassOf<UGameplayEffect>& effect : Mag->GetBullet()->GetEffects())
		{
			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(effect, 1.f, ContextHandle);
			
			if (SpecHandle.IsValid())
				SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}
	}
}

void AWeapon::ApplyGameplayCue(AActor* Source, const FHitResult& HitResult) const
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Source);

	if (SourceASC && ImpactCueTag.IsValid())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = HitResult.ImpactPoint;
		if (HitResult.ImpactPoint.IsNearlyZero())
			CueParams.Location = GetActorLocation();
		CueParams.Normal = HitResult.ImpactNormal;
		CueParams.SourceObject = this;
		
		SourceASC->ExecuteGameplayCue(ImpactCueTag, CueParams);
	}
}
