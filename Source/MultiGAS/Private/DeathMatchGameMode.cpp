#include "DeathMatchGameMode.h"
#include "PlayerTeam.h"
#include "TeamParameters.h"
#include "CombatPlayerState.h"

void ADeathMatchGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	for (const TPair<ETeamType,int32> TeamTypeInfo : StarterTeams)
	{
		for (int32 i = 0; i < TeamTypeInfo.Value; i++)
		{
			CreateTeam(TeamTypeInfo.Key,{});
		}
	}
}

FString ADeathMatchGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	const FString ErrorMessage =
		Super::InitNewPlayer(
			NewPlayerController,
			UniqueId,
			Options,
			Portal);

	if (!ErrorMessage.IsEmpty())
	{
		return ErrorMessage;
	}

	if (bJoinSmallestTeam)
	{
		UPlayerTeam* SmallestTeam = GetSmallestTeam();

		if (SmallestTeam)
		{
			JoinTeam(SmallestTeam, NewPlayerController);
		}
	}

	return TEXT("");
}



UClass* ADeathMatchGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	APlayerController* PlayerController = Cast<APlayerController>(InController);
	if (!PlayerController)
	{
		return Super::GetDefaultPawnClassForController_Implementation(InController);
	}
	UPlayerTeam* PlayerTeam = GetTeamByPlayer(PlayerController);
	if (PlayerTeam == nullptr)
	{
		return Super::GetDefaultPawnClassForController_Implementation(InController);
	}
	UTeamParameters* TeamParameter = TeamParameters[PlayerTeam->GetTeamType()];
	if (!TeamParameter || TeamParameter->bUseDefaultGameModePawn)
	{
		return Super::GetDefaultPawnClassForController_Implementation(InController);
	}
	return TeamParameter->TeamPawn;
}

UPlayerTeam* ADeathMatchGameMode::CreateTeam(ETeamType TeamType, TArray<APlayerController*> PlayerControllers)
{
	if (!TeamParameters.Contains(TeamType))
	{
		return nullptr;
	}
	TObjectPtr<UPlayerTeam> NewPlayerTeam = NewObject<UPlayerTeam>(this);
	
	NewPlayerTeam->Init(PlayerControllers,TeamType);
	
	FTeams& TeamsForId = TeamsById.FindOrAdd(TeamType);
	TeamsForId.PlayerTeams.Add(NewPlayerTeam);
	OnNewTeamCreated(NewPlayerTeam,TeamType);
	
	for (APlayerController* PlayerController : PlayerControllers)
	{
		OnPlayerJoinTeam(PlayerController,NewPlayerTeam);
	}
	
	return NewPlayerTeam;
}

bool ADeathMatchGameMode::JoinTeam(UPlayerTeam* Team, APlayerController* PlayerController)
{
	bool HasJoinedTeam = Team->AddMemberToTeam(PlayerController);
	if (HasJoinedTeam)
	{
		OnPlayerJoinTeam(PlayerController,Team);
		return true;
	}
	return false;
}
UPlayerTeam* ADeathMatchGameMode::JoinTeam(ETeamType TeamType, APlayerController* PlayerController)
{
	FTeams* TeamsForId = TeamsById.Find(TeamType);
	if (TeamsForId == nullptr)
	{
		return nullptr;
	}
	for (UPlayerTeam* Team : TeamsForId->PlayerTeams)
	{
		bool HasJoinedTeam = JoinTeam(Team,PlayerController);
		if (HasJoinedTeam)
		{
			return Team;
		}
	}
	return nullptr;
}



UPlayerTeam* ADeathMatchGameMode::GetTeamById(int32 TeamId)
{
	if (TeamId >= CreatedTeams.Num())
	{
		return CreatedTeams[TeamId];
	}
	return nullptr;
}

bool ADeathMatchGameMode::GetTeamId(UPlayerTeam* Team, int32& TeamId)
{
	for (int i = 0; i < CreatedTeams.Num(); i++)
	{
		if (CreatedTeams[i] == Team)
		{
			TeamId = i;
			return true;
		}
	}
	return false;
}

UPlayerTeam* ADeathMatchGameMode::GetTeamByPlayer(APlayerController* Player)
{
	ACombatPlayerState* CombatPlayerState = Player->GetPlayerState<ACombatPlayerState>();
	if (CombatPlayerState == nullptr || !CombatPlayerState->HasATeam())
	{
		return nullptr;
	}
	int8 TeamId = CombatPlayerState->GetTeamId();
	check(TeamId < CreatedTeams.Num());
	return CreatedTeams[TeamId];
}

UPlayerTeam* ADeathMatchGameMode::GetSmallestTeam()
{
	UPlayerTeam* SmallestTeam = nullptr;
	for (UPlayerTeam* Team : CreatedTeams)
	{
		if (SmallestTeam == nullptr || Team->GetTeamSize() < SmallestTeam->GetTeamSize())
		{
			SmallestTeam = Team;
		}
	}
	return SmallestTeam;
}


void ADeathMatchGameMode::OnPlayerJoinTeam_Implementation(APlayerController* Player, UPlayerTeam* Team)
{
	ACombatPlayerState* CombatPlayerState = Player->GetPlayerState<ACombatPlayerState>();
	if (CombatPlayerState == nullptr)
	{
		return;
	}
	int32 TeamId;
	if (GetTeamId(Team, TeamId))
	{
		CombatPlayerState->SetTeamId(TeamId);
	}
}

void ADeathMatchGameMode::OnNewTeamCreated_Implementation(UPlayerTeam* NewTeam,ETeamType TeamType)
{
	CreatedTeams.Add(NewTeam);
	OnTeamCreated.Broadcast(NewTeam,TeamType);
}
