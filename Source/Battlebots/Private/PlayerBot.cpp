// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBot.h"
#include "BBPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BBAbilitySystemComponent.h"
#include "BBPlayerController.h"


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
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;


	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
}

// Called when the game starts or when spawned
void APlayerBot::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
/*void APlayerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

// Called to bind functionality to input
void APlayerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
	}
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
	}
}
