// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Characters/Bot.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerBot.generated.h"

UCLASS()
class BATTLEBOTS_API APlayerBot : public ABot
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBot();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	USpringArmComponent* GetSpringArmComp();

	UCameraComponent* GetCameraComp();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetStartingCameraArmLength();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	FVector GetStartingCameraLocation();

	USkeletalMeshComponent* GetSkeletalMeshComp() const;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void FinishDying() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	float StartingCameraArmLength;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	FVector StartingCameraLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Camera")
	class UCameraComponent* CameraComp;

	FGameplayTag DeadTag;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// Client only
	virtual void OnRep_PlayerState() override;
};
