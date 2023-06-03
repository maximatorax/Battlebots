// Fill out your copyright notice in the Description page of Project Settings.


#include "BBBotAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UBBBotAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBBBotAttributeSet, Health, OldHealth);
}

void UBBBotAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBBBotAttributeSet, Health, COND_None, REPNOTIFY_Always);
}
