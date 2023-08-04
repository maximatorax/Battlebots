// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot.h"
#include "BBAbilitySystemComponent.h"


// Sets default values
ABot::ABot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bAlwaysRelevant = true;

	BotAttributeSet = CreateDefaultSubobject<UBBBotAttributeSet>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void ABot::BeginPlay()
{
	Super::BeginPlay();
	
	BotAttributeSet->InitHealth(100.0f);
}

// Called every frame
// void ABot::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }

UAbilitySystemComponent* ABot::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}
