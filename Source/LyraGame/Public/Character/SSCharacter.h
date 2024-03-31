// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LyraCharacter.h"
#include "SSCharacter.generated.h"

enum class EWeaponType : uint8;
class ULyraCameraComponent;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeaponExperience
{
	GENERATED_BODY()

public:
	// 현재 경험치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Level")
	float CurrentEXP = 0.0f;

	// 현재 레벨
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Level")
	int32 WeaponLevel = 1;

	// 레벨업에 필요한 경험치 임계값
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Level")
	TArray<float> EXPThresholds;
};

UCLASS()
class LYRAGAME_API ASSCharacter : public ALyraCharacter
{
	GENERATED_BODY()
	
public:
	
	ASSCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	float GetSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() const { return Mesh1p; }

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	FORCEINLINE ULyraCameraComponent* GetCameraComponent() const {  return CameraComponent; }
	
	UFUNCTION(BlueprintPure, Category = "SS|Character")
	FORCEINLINE class ASSWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }
	
	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	void SetCurrentWeapon(ASSWeaponBase* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	void OnPlayerEquippedNewWeapon(float WeaponOffset);

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	void SetIsFirstPerson();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
	void CheckLevelUp();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
	void OnLevelUp();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
	void AddEXP(EWeaponType WeaponType, float EXP);
	
public:
	//Weapon
	UPROPERTY()
	class ASSWeaponBase* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "SS|Character")
	bool IsFirstPerson = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Level")
	TMap<EWeaponType, FWeaponExperience> WeaponExp;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SS|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh1p;
	
};
