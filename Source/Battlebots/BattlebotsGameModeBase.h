// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattlebotsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABattlebotsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABattlebotsGameModeBase();

	void PlayerDied(AController* Controller);

protected:
	float RespawnDelay;
	
	TSubclassOf<class APlayerBot> PlayerBot;

	virtual void BeginPlay() override;

	void RespawnPlayer(AController* Controller);
	
};
