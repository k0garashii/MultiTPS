#include "PlayerTeam.h"

void UPlayerTeam::Init(ETeamType InTeamType)
{
	TeamType = InTeamType;
}
void UPlayerTeam::Init(const TArray<APlayerController*> InTeamMembers,ETeamType InTeamType)
{
	TeamMembers = InTeamMembers;
	Init(InTeamType);
}

void UPlayerTeam::Init(const TArray<APlayerController*> InTeamMembers,ETeamType InTeamType, uint32 InTeamMaxSize)
{
	Init(InTeamMembers,InTeamType);
	SetTeamMaxSize(InTeamMaxSize);
}



bool UPlayerTeam::SetTeamMaxSize(uint32 NewTeamMaxSize)
{
	if (NewTeamMaxSize <= TeamMaxSize)
	{
		return false;
	}
	TeamMaxSize = NewTeamMaxSize;
	bTeamCanBeFull = true;
	return true;
}

uint32 UPlayerTeam::GetTeamSize() const
{
	return TeamMembers.Num();
}

ETeamType UPlayerTeam::GetTeamType() const
{
	return TeamType;
}

const TArray<APlayerController*>& UPlayerTeam::GetTeamMembers() const
{
	return TeamMembers;
}

bool UPlayerTeam::AddMemberToTeam(APlayerController* NewPlayerController)
{
	if (bTeamCanBeFull && GetTeamSize() >= TeamMaxSize)
	{
		return false;
	}
	TeamMembers.Add(NewPlayerController);
	return true;
}

void UPlayerTeam::RemoveMemberFromTeam(APlayerController* NewPlayerController)
{
	TeamMembers.Remove(NewPlayerController);
}

void UPlayerTeam::Disband()
{
	TeamMembers.Empty();
}
