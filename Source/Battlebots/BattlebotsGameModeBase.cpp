// Copyright Epic Games, Inc. All Rights Reserved.


#include "BattlebotsGameModeBase.h"

#include "BBPlayerController.h"
#include "PlayerBot.h"
#include "GameFramework/SpectatorPawn.h"

ABattlebotsGameModeBase::ABattlebotsGameModeBase()
{
	RespawnDelay = 5.0f;
	
	PlayerBot = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/Player/BP_BotPlayer.BP_BotPlayer_C"));

	if(!PlayerBot)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find PlayerBot. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}
}

void ABattlebotsGameModeBase::PlayerDied(AController* Controller)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParameters);

	Controller->UnPossess();
	Controller->Possess(SpectatorPawn);

	FTimerHandle RespawnTimerHandle;

	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ABattlebotsGameModeBase::RespawnPlayer,
	                                                               Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);

	if (ABBPlayerController* PC = Cast<ABBPlayerController>(Controller))
	{
		PC->SetRespawnCountdown(RespawnDelay);
	}
}

void ABattlebotsGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABattlebotsGameModeBase::RespawnPlayer(AController* Controller)
{
	if (Controller->IsPlayerController())
	{
		// Respawn player hero
		AActor* PlayerStart = FindPlayerStart(Controller);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APlayerBot* Player = GetWorld()->SpawnActor<APlayerBot>(PlayerBot, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), SpawnParameters);

		APawn* OldSpectatorPawn = Controller->GetPawn();
		Controller->UnPossess();
		OldSpectatorPawn->Destroy();
		Controller->Possess(Player);
	}
}
