#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TeamParameters.h"
#include "PlayerTeam.generated.h"

class APlayerController;


UCLASS()
class MULTIGAS_API UPlayerTeam : public UObject
{
	GENERATED_BODY()
public :
	void Init(ETeamType InTeamType);
	void Init(const TArray<APlayerController*> InTeamMembers,ETeamType InTeamType);
	void Init(const TArray<APlayerController*> InTeamMembers,ETeamType InTeamType,uint32 InTeamMaxSize);
	
	bool SetTeamMaxSize(uint32 NewTeamMaxSize);
	uint32 GetTeamSize() const;
	ETeamType GetTeamType() const;
	const TArray<APlayerController*>& GetTeamMembers() const;
	bool AddMemberToTeam(APlayerController* NewPlayerController);
	void RemoveMemberFromTeam(APlayerController* NewPlayerController);
	
	void Disband();
protected :
	TArray<APlayerController*> TeamMembers;
private :
	ETeamType TeamType;
	bool bTeamCanBeFull = false; // false means team can have unlimited members, true has a max size
	uint32 TeamMaxSize;
};
