#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapon/Bullet.h"
#include "Mag.generated.h"

class AWeapon;

UCLASS()
class MULTIGAS_API UMag : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UBullet* GetBullet() const { return Bullet; }
	void DecreaseBullet(AWeapon* Weapon) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	UBullet* Bullet = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	int NumBullets = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	float ReloadTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effect")
	TSubclassOf<UGameplayEffect> DecreaseBulletEffect;
};
