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
#include "Player/LyraPlayerStart.h" // 헤더파일 추가 (05.25)
#include "EngineUtils.h" // for TActorIterator (05.25)


// 봇 균등 생성 로직 추가 버전 코드 (05.25)
void ULyraBlueprintFunctions::SpawnEnemy(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, int32 NumBots)
{
    // LyraPlayerStart 인스턴스 배열 가져오기
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GameStateComponent->GetWorld(), ALyraPlayerStart::StaticClass(), PlayerStarts);

    // Melee 태그가 있는 인스턴스만 필터링
    TArray<AActor*> MeleeTaggedStarts;
    for (AActor* Start : PlayerStarts)
    {
        ALyraPlayerStart* LyraStart = Cast<ALyraPlayerStart>(Start);
        if (LyraStart && LyraStart->PlayerStartTag == FName("Melee"))
        {
            MeleeTaggedStarts.Add(Start);
        }
    }

    if (MeleeTaggedStarts.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Melee-tagged PlayerStarts found in the level."));
        return;
    }

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.OverrideLevel = GameStateComponent->GetComponentLevel();
    SpawnInfo.ObjectFlags |= RF_Transient;

    // 인덱스를 사용해 순환하면서 봇을 생성
    int32 PlayerStartIndex = 0;

    for (int32 i = 0; i < NumBots; ++i)
    {
        // 현재 인덱스에 해당하는 Melee 태그를 가진 PlayerStart를 선택
        AActor* SelectedStart = MeleeTaggedStarts[PlayerStartIndex];
        FVector SpawnLocation = SelectedStart->GetActorLocation();
        FRotator SpawnRotation = SelectedStart->GetActorRotation();

        // 봇 생성
        AAIController* NewController = GameStateComponent->GetWorld()->SpawnActor<AAIController>(BotControllerClass,
            SpawnLocation, SpawnRotation, SpawnInfo);

        if (NewController != nullptr)
        {
            check(GameMode);

            GameMode->GenericPlayerInitialization(NewController);
            GameMode->RestartPlayerAtPlayerStart(NewController, SelectedStart);

            if (NewController->GetPawn() != nullptr)
            {
                if (ULyraPawnExtensionComponent* PawnExtComponent = NewController->GetPawn()->FindComponentByClass<ULyraPawnExtensionComponent>())
                {
                    PawnExtComponent->CheckDefaultInitialization();
                }
            }
        }

        // 다음 MeleeTaggedStarts 인덱스로 이동 (순환)
        PlayerStartIndex = (PlayerStartIndex + 1) % MeleeTaggedStarts.Num();
    }
}







// 봇 균등 생성 로직 추가 전 기존 로직(05.25)
// 아래 int32 NumBots 추가
//void ULyraBlueprintFunctions::SpawnEnemy(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, int32 NumBots)
//{
//	FActorSpawnParameters SpawnInfo;
//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	SpawnInfo.OverrideLevel = GameStateComponent->GetComponentLevel();
//	SpawnInfo.ObjectFlags |= RF_Transient;
//
//	// for문 추가
//	for (int32 i = 0; i < NumBots; ++i)
//	{
//		AAIController* NewController = GameStateComponent->GetWorld()->SpawnActor<AAIController>(BotControllerClass,
//			FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
//
//		if (NewController != nullptr)
//		{
//			check(GameMode);
//
//			GameMode->GenericPlayerInitialization(NewController);
//			GameMode->RestartPlayer(NewController);
//
//			if (NewController->GetPawn() != nullptr)
//			{
//				if (ULyraPawnExtensionComponent* PawnExtComponent = NewController->GetPawn()->FindComponentByClass<ULyraPawnExtensionComponent>())
//				{
//					PawnExtComponent->CheckDefaultInitialization();
//				}
//			}
//		}
//	}
//}