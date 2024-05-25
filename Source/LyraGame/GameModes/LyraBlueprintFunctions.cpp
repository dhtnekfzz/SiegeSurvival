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
#include "Player/LyraPlayerStart.h" // ������� �߰� (05.25)
#include "EngineUtils.h" // for TActorIterator (05.25)


// �� �յ� ���� ���� �߰� ���� �ڵ� (05.25)
void ULyraBlueprintFunctions::SpawnEnemy(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, int32 NumBots)
{
    // LyraPlayerStart �ν��Ͻ� �迭 ��������
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GameStateComponent->GetWorld(), ALyraPlayerStart::StaticClass(), PlayerStarts);

    // Melee �±װ� �ִ� �ν��Ͻ��� ���͸�
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

    // �ε����� ����� ��ȯ�ϸ鼭 ���� ����
    int32 PlayerStartIndex = 0;

    for (int32 i = 0; i < NumBots; ++i)
    {
        // ���� �ε����� �ش��ϴ� Melee �±׸� ���� PlayerStart�� ����
        AActor* SelectedStart = MeleeTaggedStarts[PlayerStartIndex];
        FVector SpawnLocation = SelectedStart->GetActorLocation();
        FRotator SpawnRotation = SelectedStart->GetActorRotation();

        // �� ����
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

        // ���� MeleeTaggedStarts �ε����� �̵� (��ȯ)
        PlayerStartIndex = (PlayerStartIndex + 1) % MeleeTaggedStarts.Num();
    }
}







// �� �յ� ���� ���� �߰� �� ���� ����(05.25)
// �Ʒ� int32 NumBots �߰�
//void ULyraBlueprintFunctions::SpawnEnemy(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, int32 NumBots)
//{
//	FActorSpawnParameters SpawnInfo;
//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	SpawnInfo.OverrideLevel = GameStateComponent->GetComponentLevel();
//	SpawnInfo.ObjectFlags |= RF_Transient;
//
//	// for�� �߰�
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