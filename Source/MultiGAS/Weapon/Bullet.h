#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "Bullet.generated.h"

UCLASS()
class MULTIGAS_API UBullet : public UDataAsset
{
	GENERATED_BODY()
public:
	int GetDamages() const { return Damages;}
	TArray<TSubclassOf<UGameplayEffect>> GetEffects() const { return OnHitEffects; }
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Damages = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OnHitEffects;
};
