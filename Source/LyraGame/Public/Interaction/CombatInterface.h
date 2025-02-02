// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

enum class EWeaponType : uint8;
struct FGameplayTag;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LYRAGAME_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetLevel(EWeaponType WeaponType) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP(EWeaponType WeaponType) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddXP(int32 InXP,EWeaponType WeaponType);

	UFUNCTION(BlueprintNativeEvent)
	void AddWeaponLevel(int32 InLevel,EWeaponType WeaponType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);
};
