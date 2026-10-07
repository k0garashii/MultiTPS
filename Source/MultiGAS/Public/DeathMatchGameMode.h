#pragma once

#include "CoreMinimal.h"
#include "PlayerTeam.h"
#include "TeamParameters.h"
#include "GameFramework/GameMode.h"
#include "DeathMatchGameMode.generated.h"





DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamCreated,UPlayerTeam*,NewTeam,ETeamType,TeamID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerJoinedTeam,APlayerController*,Player,UPlayerTeam*,Team);
USTRUCT()
struct FTeams
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<TObjectPtr<UPlayerTeam>> PlayerTeams;
};

UCLASS()
class MULTIGAS_API ADeathMatchGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	UFUNCTION(BlueprintCallable)
	UPlayerTeam* CreateTeam(ETeamType TeamType,TArray<APlayerController*> PlayerControllers);
	UFUNCTION(BlueprintCallable)
	// Try to find a team with team id and joins it
	UPlayerTeam* JoinTeam(ETeamType TeamType,APlayerController* PlayerControllers);
	// Try to find a team with team id and joins it
	bool JoinTeam(UPlayerTeam* Team,APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable)
	UPlayerTeam* GetTeamById(int32 TeamId);
	UFUNCTION(BlueprintCallable)
	bool GetTeamId(UPlayerTeam* Team,int32& TeamId);
	UFUNCTION(BlueprintCallable)
	UPlayerTeam* GetTeamByPlayer(APlayerController* Player);
protected:
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;
	UFUNCTION(BlueprintNativeEvent)
	void OnNewTeamCreated(UPlayerTeam* NewTeam,ETeamType TeamType);
	UFUNCTION(BlueprintNativeEvent)
	void OnPlayerJoinTeam(APlayerController* Player,UPlayerTeam* Team);
	
	
private :
	UPlayerTeam* GetSmallestTeam();
public :
	UPROPERTY(BlueprintAssignable)
	FOnTeamCreated OnTeamCreated;
	UPROPERTY(EditDefaultsOnly)
	bool bJoinSmallestTeam = false;
protected:
	UPROPERTY(Instanced,EditDefaultsOnly)
	TMap<ETeamType,TObjectPtr<UTeamParameters>> TeamParameters;
	UPROPERTY(EditDefaultsOnly)
	uint32 NumberOfTeamsToStartGame;
	UPROPERTY(EditDefaultsOnly)
	// Will create X amounts of TeamType at beginning
	TMap<ETeamType,int32> StarterTeams;
private:
	UPROPERTY()
	TMap<ETeamType,FTeams> TeamsById;
	UPROPERTY()
	// Teams created in order, index is TeamId
	TArray<TObjectPtr<UPlayerTeam>> CreatedTeams;
	
	
};
