// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LyraCharacter.h"
#include "SSCharacter.generated.h"

class ULyraCameraComponent;
/**
 * 
 */
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

	//Weapon
	UFUNCTION(BlueprintPure, Category = "SS|Character")
	FORCEINLINE class ASSWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }
	
	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	void SetCurrentWeapon(ASSWeaponBase* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	void OnPlayerEquippedNewWeapon(float WeaponOffset);

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	void SetIsFirstPerson();

	
public:
	//Weapon
	UPROPERTY()
	class ASSWeaponBase* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "SS|Character")
	bool IsFirstPerson = false;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SS|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh1p;
	
};
