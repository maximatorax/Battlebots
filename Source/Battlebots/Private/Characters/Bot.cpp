// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Bot.h"

#include "AbilitySystem/BBAbilitySystemComponent.h"
#include "AbilitySystem/BBGameplayAbility.h"
#include "AbilitySystem/BBGameplayEffect.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABot::ABot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);

	bAlwaysRelevant = true;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
}

UAbilitySystemComponent* ABot::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool ABot::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 ABot::GetAbilityLevel(EBBAbilityInputID AbilityID) const
{
	return 1;
}

void ABot::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->
		bCharacterAbilitiesGiven)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

float ABot::GetHealth() const
{
	if (BotAttributeSet.IsValid())
	{
		return BotAttributeSet->GetHealth();
	}

	return 0.0f;
}

float ABot::GetMaxHealth() const
{
	if (BotAttributeSet.IsValid())
	{
		return BotAttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float ABot::GetMoveSpeed() const
{
	if(BotAttributeSet.IsValid())
	{
		return BotAttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float ABot::GetMoveSpeedBaseValue() const
{
	if (BotAttributeSet.IsValid())
	{
		return BotAttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(BotAttributeSet.Get())->GetBaseValue();
	}

	return 0.0f;
}

void ABot::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	FinishDying();
}

void ABot::FinishDying()
{
	Destroy();
}

// Called when the game starts or when spawned
void ABot::BeginPlay()
{
	Super::BeginPlay();
}

void ABot::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->
		bCharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UBBGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void ABot::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."),
		       *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
			*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void ABot::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->
		bStartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->
			MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}

void ABot::SetHealth(float Health)
{
	if (BotAttributeSet.IsValid())
	{
		BotAttributeSet->SetHealth(Health);
	}
}
