// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Player/LyraPlayerSpawningManagerComponent.h"

#include "TDM_PlayerSpawningManagmentComponent.generated.h"

class AActor;
class AController;
class ALyraPlayerStart;
class UObject;

/**
 * 
 */
UCLASS()
class UTDM_PlayerSpawningManagmentComponent : public ULyraPlayerSpawningManagerComponent
{
	GENERATED_BODY()

public:

	UTDM_PlayerSpawningManagmentComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = OnChoosePlayerStart))
	AActor* OnChoosePlayerStart(AController* Player, const TArray<ALyraPlayerStart*>& PlayerStarts) override;
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) override;

protected:

};
