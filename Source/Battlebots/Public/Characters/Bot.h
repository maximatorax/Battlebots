﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Attributes/BBBotAttributeSet.h"
#include "GameplayTagContainer.h"
#include "Battlebots/Battlebots.h"
#include "Bot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ABot*, Character);

UCLASS()
class BATTLEBOTS_API ABot : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABot();

	UPROPERTY(BlueprintAssignable, Category="Battlebot|Bot")
	FCharacterDiedDelegate OnCharacterDied;

	//~ Begin IAbilitySystemInterface
	/** Returns our Ability System Component. */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

	UFUNCTION(BlueprintCallable, Category="Battlebot|Bot")
	virtual bool IsAlive() const;

	// Switch on AbilityID to return individual ability levels.
	UFUNCTION(BlueprintCallable, Category = "Battlebot|BBBot")
	virtual int32 GetAbilityLevel(EBBAbilityInputID AbilityID) const;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	/**
	* Getters for attributes from BBBotAttributeSet
	**/

	UFUNCTION(BlueprintCallable, Category = "Battlebot|Bot|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Battlebot|Bot|Attributes")
	float GetMaxHealth() const;

	//Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category="Battlebot|Bot|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Battlebot|Bot|Attributes")
	float GetMoveSpeedBaseValue() const;

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category="Battlebot|Bot")
	virtual void FinishDying();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TWeakObjectPtr<class UBBAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UBBBotAttributeSet> BotAttributeSet;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battlebot|Bot")
	FText CharacterName;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battlebot|Abilities")
	TArray<TSubclassOf<class UBBGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battlebot|Abilities")
	TSubclassOf<class UBBGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battlebot|Abilities")
	TArray<TSubclassOf<UBBGameplayEffect>> StartupEffects;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();


	/**
	* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
	* These change the Attribute's Base Value.
	*/
	virtual void SetHealth(float Health);
};
