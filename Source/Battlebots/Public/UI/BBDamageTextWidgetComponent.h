// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "BBDamageTextWidgetComponent.generated.h"


UCLASS(ClassGroup=(CustomWidget), meta=(BlueprintSpawnableComponent))
class BATTLEBOTS_API UBBDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBBDamageTextWidgetComponent();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};
