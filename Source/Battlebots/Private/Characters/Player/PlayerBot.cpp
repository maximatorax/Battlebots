// Fill out your copyright notice in the Description page of Project Settings.



#include "Characters/Player/PlayerBot.h"
#include "Player/BBPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystem/BBAbilitySystemComponent.h"
#include "Player/BBPlayerController.h"
#include "Battlebots/BattlebotsGameModeBase.h"
#include "Components/CapsuleComponent.h"


// Sets default values
APlayerBot::APlayerBot()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create our components
	RootComponent = GetRootComponent();
	SkeletalMeshComp = GetMesh();
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	//Attach our components
	SkeletalMeshComp->SetupAttachment(RootComponent);
	SpringArmComp->SetupAttachment(SkeletalMeshComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//Assign SpringArm class variables.
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 70.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;

	//Assign Camera class variables.
	CameraComp->FieldOfView = 80.0f;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;

	// Makes sure that the animations play on the Server so that we can use bone and socket transforms
	// to do things like spawning projectiles and other FX.
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

// Called when the game starts or when spawned
void APlayerBot::BeginPlay()
{
	Super::BeginPlay();

	StartingCameraArmLength = SpringArmComp->TargetArmLength;
	StartingCameraLocation = SpringArmComp->GetRelativeLocation();
}

void APlayerBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called to bind functionality to input
void APlayerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* APlayerBot::GetAbilitySystemComponent() const
{
	return GetPlayerState<ABBPlayerState>()->GetAbilitySystemComponent();
}

void APlayerBot::FinishDying()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ABattlebotsGameModeBase* GM = Cast<ABattlebotsGameModeBase>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->PlayerDied(GetController());
		}
	}

	Super::FinishDying();
}

//Server only
void APlayerBot::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ABBPlayerState* PS = GetPlayerState<ABBPlayerState>();

	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UBBAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set the AttributeSetBase for convenience attribute functions
		BotAttributeSet = PS->GetBotAttributeSet();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();


		// Respawn specific things that won't affect first possession.
		
		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());

		// End respawn specific things


		AddStartupEffects();

		AddCharacterAbilities();

		ABBPlayerController* PC = Cast<ABBPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}
	}
}

USpringArmComponent* APlayerBot::GetSpringArmComp()
{
	return SpringArmComp;
}

UCameraComponent* APlayerBot::GetCameraComp()
{
	return CameraComp;
}

float APlayerBot::GetStartingCameraArmLength()
{
	return StartingCameraArmLength;
}

FVector APlayerBot::GetStartingCameraLocation()
{
	return StartingCameraLocation;
}

USkeletalMeshComponent* APlayerBot::GetSkeletalMeshComp() const
{
	return SkeletalMeshComp;
}

void APlayerBot::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ABBPlayerState* PS = GetPlayerState<ABBPlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UBBAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Set the AttributeSetBase for convenience attribute functions
		BotAttributeSet = PS->GetBotAttributeSet();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();
		
		ABBPlayerController* PC = Cast<ABBPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
	}
}
