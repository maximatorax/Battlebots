// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/PlayerState.h"
#include "BBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABBPlayerState();

	//~ Begin IAbilitySystemInterface
	/** Returns our Ability System Component. */
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface
	
	class UBBBotAttributeSet* GetBotAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "BBPlayerState")
	bool IsAlive() const;

	/**
	* Getters for attributes from GDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMaxHealth() const;


protected:
	/** Ability System Component. Required to use Gameplay Attributes and Gameplay Abilities. */
	UPROPERTY()
	class UBBAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UBBBotAttributeSet* BotAttributeSet;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);

	virtual void BeginPlay() override;
};
