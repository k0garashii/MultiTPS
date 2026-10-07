// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CombatPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIGAS_API ACombatPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintGetter)
	int32 GetKills() const;
	UFUNCTION(BlueprintGetter)
	int32 GetDeaths() const;
	UFUNCTION(BlueprintCallable)
	bool HasATeam();
	
	int8 GetTeamId() const;
	
	UFUNCTION(BlueprintCallable)
	void SetKills(int32 NewKills);
	UFUNCTION(BlueprintCallable)
	void SetDeaths(int32 NewDeaths);
	
	void SetTeamId(int8 NewTeamId);
	
	UFUNCTION(BlueprintCallable)
	void AddKill();
	UFUNCTION(BlueprintCallable)
	void AddDeath();

protected:
	UPROPERTY(Replicated, BlueprintGetter = GetKills)
	int32 Kills;
	
	UPROPERTY(Replicated, BlueprintGetter = GetDeaths)
	int32 Deaths;
	
	UPROPERTY(Replicated)
	int8 TeamId;
private :
	UPROPERTY(Replicated)
	bool bHasATeam = false;
};
