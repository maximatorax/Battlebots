// Fill out your copyright notice in the Description page of Project Settings.


#include "BBPlayerState.h"
#include "AbilitySystemComponent.h"
#include "BBAbilitySystemComponent.h"
#include "BBBotAttributeSet.h"
#include "PlayerBot.h"
#include "Util/ColorConstants.h"

ABBPlayerState::ABBPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UBBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another BBPlayerState (PlayerBot) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	BotAttributeSet = CreateDefaultSubobject<UBBBotAttributeSet>(TEXT("PlayerAttributeSet"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ABBPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UBBBotAttributeSet* ABBPlayerState::GetBotAttributeSet() const
{
	return BotAttributeSet;
}

bool ABBPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ABBPlayerState::GetHealth() const
{
	return BotAttributeSet->GetHealth();
}

float ABBPlayerState::GetMaxHealth() const
{
	return BotAttributeSet->GetMaxHealth();
}

void ABBPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"), Data.NewValue));
}

void ABBPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;
}

void ABBPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if(AbilitySystemComponent)
	{
		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BotAttributeSet->GetHealthAttribute()).AddUObject(this, &ABBPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BotAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ABBPlayerState::MaxHealthChanged);
	}
}
