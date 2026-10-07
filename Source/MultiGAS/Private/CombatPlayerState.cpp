#include "CombatPlayerState.h"
#include "Net/UnrealNetwork.h"

void ACombatPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACombatPlayerState,Kills);
	DOREPLIFETIME(ACombatPlayerState,Deaths);
	DOREPLIFETIME(ACombatPlayerState,TeamId);
	DOREPLIFETIME(ACombatPlayerState,bHasATeam);
}

int32 ACombatPlayerState::GetKills() const
{
	return Kills;
}

int32 ACombatPlayerState::GetDeaths() const
{
	return Deaths;
}

bool ACombatPlayerState::HasATeam()
{
	return bHasATeam;
}

int8 ACombatPlayerState::GetTeamId() const
{
	return TeamId;
}

void ACombatPlayerState::SetKills(int32 NewKills)
{
	Kills = NewKills;
}

void ACombatPlayerState::SetDeaths(int32 NewDeaths)
{
	Deaths = NewDeaths;
}

void ACombatPlayerState::SetTeamId(int8 NewTeamId)
{
	bHasATeam = true;
	TeamId = NewTeamId;
}

void ACombatPlayerState::AddKill()
{
	SetKills(GetKills() + 1);
}

void ACombatPlayerState::AddDeath()
{
	SetDeaths(GetDeaths() + 1);
}

