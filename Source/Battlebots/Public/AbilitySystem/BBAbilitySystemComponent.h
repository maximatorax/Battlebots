// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BBAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UBBAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API UBBAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;
	
	FReceivedDamageDelegate ReceivedDamage;

	// Called from GDDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UBBAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
