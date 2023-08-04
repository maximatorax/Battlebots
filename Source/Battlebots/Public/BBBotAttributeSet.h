// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BBBotAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(UBBBotAttributeSet, Health) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UBBBotAttributeSet, Health) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(Health) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(Health) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(Health)

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API UBBBotAttributeSet : public UAttributeSet
{
private:
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBBBotAttributeSet, Health)
};
