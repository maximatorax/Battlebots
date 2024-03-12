// Fill out your copyright notice in the Description page of Project Settings.



#include "Player/BBPlayerController.h"
#include "Player/BBPlayerState.h"
#include "AbilitySystemComponent.h"
#include "UI/BBDamageTextWidgetComponent.h"
#include "UI/BBHUDWidget.h"

void ABBPlayerController::CreateHUD()
{
	// Only create once
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	ABBPlayerState* PS = GetPlayerState<ABBPlayerState>();
	if (!PS)
	{
		return;
	}

	UIHUDWidget = CreateWidget<UBBHUDWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();

	// Set attributes
	UIHUDWidget->SetCurrentHealth(PS->GetHealth());
	UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	UIHUDWidget->SetHealthPercentage(PS->GetHealth() / FMath::Max<float>(PS->GetMaxHealth(), 1.f));
}

UBBHUDWidget* ABBPlayerController::GetHUD()
{
	return UIHUDWidget;
}

void ABBPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ABot* TargetCharacter)
{
	if (TargetCharacter && DamageNumberClass)
	{
		UBBDamageTextWidgetComponent* DamageText = NewObject<UBBDamageTextWidgetComponent>(TargetCharacter, DamageNumberClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

bool ABBPlayerController::ShowDamageNumber_Validate(float DamageAmount, ABot* TargetCharacter)
{
	return true;
}

void ABBPlayerController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{
	if (UIHUDWidget)
	{
		UIHUDWidget->SetRespawnCountdown(RespawnTimeRemaining);
	}
}

bool ABBPlayerController::SetRespawnCountdown_Validate(float RespawnTimeRemaining)
{
	return true;
}

void ABBPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABBPlayerState* PS = GetPlayerState<ABBPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void ABBPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Player is possessed.
	CreateHUD();
}
