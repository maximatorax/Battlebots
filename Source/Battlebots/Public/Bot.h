// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BBBotAttributeSet.h"
#include "GameplayTagContainer.h"
#include "Battlebots/Battlebots.h"
#include "Bot.generated.h"

UCLASS()
class BATTLEBOTS_API ABot : public ACharacter, public IAbilitySystemInterface
{
private:
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;*/

	TWeakObjectPtr<class UBBAbilitySystemComponent> AbilitySystemComponent;

	TWeakObjectPtr<class UBBBotAttributeSet> BotAttributeSet;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UBBGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Abilities")
	FGameplayTagContainer BotGameplayTags;

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

public:
	// Sets default values for this pawn's properties
	ABot();

	//~ Begin IAbilitySystemInterface
	/** Returns our Ability System Component. */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDCharacter|Attributes")
	float GetMaxHealth() const;
};
