// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BBBotAttributeSet.h"
#include "Bot.generated.h"

UCLASS()
class BATTLEBOTS_API ABot : public ACharacter, public IAbilitySystemInterface
{
private:
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TWeakObjectPtr<class UBBAbilitySystemComponent> AbilitySystemComponent;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Abilities")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Abilities")
	UBBBotAttributeSet* BotAttributeSet;

public:
	// Sets default values for this pawn's properties
	ABot();

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	//~ Begin IAbilitySystemInterface
	/** Returns our Ability System Component. */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface
};
