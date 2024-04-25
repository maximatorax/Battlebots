// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BBSessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBBOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBBOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBBOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBBOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBBOnDestroySessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FBBOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FBBOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

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
	void UpdateSession();
	UFUNCTION(BlueprintCallable, Category="Online|Session")
	void StartSession();
	void EndSession();
	void DestroySession();
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);

	FBBOnCreateSessionComplete OnCreateSessionCompleteEvent;
	FBBOnUpdateSessionComplete OnUpdateSessionCompleteEvent;
	FBBOnStartSessionComplete OnStartSessionCompleteEvent;
	FBBOnEndSessionComplete OnEndSessionCompleteEvent;
	FBBOnDestroySessionComplete OnDestroySessionCompleteEvent;
	FBBOnFindSessionsComplete OnFindSessionsCompleteEvent;
	FBBOnJoinSessionComplete OnJoinGameSessionCompleteEvent;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnEndSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession();

private:
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;
	
	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
