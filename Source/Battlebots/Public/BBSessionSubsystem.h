// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BBSessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBBOnCreateSessionComplete, bool, Succesful);

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API UBBSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UBBSessionSubsystem();
	
	void CreateSession(int32 NumPublicConnections, bool IsLANMatch);

	FBBOnCreateSessionComplete OnCreateSessionCompleteEvent;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
};
