// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraBlueprintFunctions.h"

#include "GameModes/LyraGameMode.h"
#include "Components/GameStateComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "GameModes/LyraExperienceDefinition.h"
#include "Character/LyraPawnExtensionComponent.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "Character/LyraPawnExtensionComponent.h"

// 아래 int32 NumBots 추가
void ULyraBlueprintFunctions::SpawnEnemy(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, int32 NumBots)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = GameStateComponent->GetComponentLevel();
	SpawnInfo.ObjectFlags |= RF_Transient;

	// for문 추가
	for (int32 i = 0; i < NumBots; ++i)
	{
		AAIController* NewController = GameStateComponent->GetWorld()->SpawnActor<AAIController>(BotControllerClass,
			FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		if (NewController != nullptr)
		{
			check(GameMode);

			GameMode->GenericPlayerInitialization(NewController);
			GameMode->RestartPlayer(NewController);

			if (NewController->GetPawn() != nullptr)
			{
				if (ULyraPawnExtensionComponent* PawnExtComponent = NewController->GetPawn()->FindComponentByClass<ULyraPawnExtensionComponent>())
				{
					PawnExtComponent->CheckDefaultInitialization();
				}
			}
		}
	}
}