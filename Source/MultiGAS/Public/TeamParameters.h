

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TeamParameters.generated.h"

// Defines team Type
UENUM(BlueprintType)
enum class ETeamType : uint8 
{
	TT_Blue UMETA(DisplayName = "Blue Team"),
	TT_Red UMETA(DisplayName = "Red Team")
};


UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable, BlueprintType)
class MULTIGAS_API UTeamParameters : public UObject
{
	GENERATED_BODY()
public :
	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> TeamPawn;;
	UPROPERTY(EditAnywhere)
	bool bUseDefaultGameModePawn = false;
};

