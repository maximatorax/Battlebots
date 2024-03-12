// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Characters/Bot.h"
#include "BBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void CreateHUD();
	
	UPROPERTY(EditAnywhere, Category = "Battlebot|UI")
	TSubclassOf<class UBBDamageTextWidgetComponent> DamageNumberClass;

	class UBBHUDWidget* GetHUD();

	UFUNCTION(Client, Reliable, WithValidation)
	void ShowDamageNumber(float DamageAmount, ABot* TargetCharacter);
	void ShowDamageNumber_Implementation(float DamageAmount, ABot* TargetCharacter);
	bool ShowDamageNumber_Validate(float DamageAmount, ABot* TargetCharacter);
	
	// Simple way to RPC to the client the countdown until they respawn from the GameMode. Will be latency amount of out sync with the Server.
	UFUNCTION(Client, Reliable, WithValidation)
	void SetRespawnCountdown(float RespawnTimeRemaining);
	void SetRespawnCountdown_Implementation(float RespawnTimeRemaining);
	bool SetRespawnCountdown_Validate(float RespawnTimeRemaining);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlebot|UI")
	TSubclassOf<class UBBHUDWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "Battlebot|UI")
	class UBBHUDWidget* UIHUDWidget;
	
	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};
