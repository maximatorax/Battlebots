// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bot.h"
#include "PlayerBot.generated.h"

UCLASS()
class BATTLEBOTS_API APlayerBot : public ABot
{
private:
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	// Client only
	virtual void OnRep_PlayerState() override;

public:
	// Sets default values for this character's properties
	APlayerBot();

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	class USpringArmComponent* GetSpringArmComp();

	class UCameraComponent* GetCameraComp();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetStartingCameraArmLength();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	FVector GetStartingCameraLocation();

	USkeletalMeshComponent* GetSkeletalMeshComp() const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
