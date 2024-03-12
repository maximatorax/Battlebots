// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BBAbilitySystemComponent.h"

void UBBAbilitySystemComponent::ReceiveDamage(UBBAbilitySystemComponent* SourceASC, float UnmitigatedDamage,
	float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
