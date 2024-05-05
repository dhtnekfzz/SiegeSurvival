// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LyraBlueprintFunctions.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraBlueprintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Lyra Blueprint Functions")
	static void SpawnEnemy(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, int32 NumBots);
};
// int32 NumBots Ãß°¡
