// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SSCharacter.h"

#include "Inventory/LyraInventoryItemDefinition.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/LyraAnimInstance.h"
#include "Animation/SSAnimInstance.h"
#include "Camera/LyraCameraComponent.h"
#include "Character/LyraCharacterMovementComponent.h"
#include "Character/LyraHealthComponent.h"
#include "Weapons/LyraWeaponSpawner.h"
#include "Weapons/SSWeaponBase.h"




ASSCharacter::ASSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULyraCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetMesh()->CastShadow = true;
	GetMesh()->bCastHiddenShadow = true;
	
	Mesh1p= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1p"));
	Mesh1p->SetupAttachment(CameraComponent);
	Mesh1p->SetOnlyOwnerSee(true);
	Mesh1p->CastShadow = false;
	Mesh1p->SetOwnerNoSee(true);	

	FWeaponExperience NewWeaponExp;
	NewWeaponExp.EXPThresholds.Add(0.0f);
	NewWeaponExp.EXPThresholds.Add(200.0f);
	NewWeaponExp.EXPThresholds.Add(300.0f);
	// NewWeaponExp.EXPThresholds.Add(400.0f);
	WeaponExp.Add(EWeaponType::Pistol, NewWeaponExp);

	// static ConstructorHelpers::FClassFinder<ULyraInventoryItemDefinition> WeaponItemClass(TEXT("/Script/Engine.Blueprint'/Game/Weapons/Pistol/ID_Pistol_SSLv2.ID_Pistol_SSLv2'"));
	// if (WeaponItemClass.Succeeded())
	// {
	// 	WeaponItemDefinition = WeaponItemClass.Class;
	// }
	
}

float ASSCharacter::GetSpeed() const
{
	return HealthComponent ? HealthComponent->GetMoveSpeed() : 0.0f;
}

void ASSCharacter::SetCurrentWeapon(ASSWeaponBase* NewWeapon)
{
	CurrentWeapon= NewWeapon;;
}

void ASSCharacter::OnPlayerEquippedNewWeapon(float WeaponOffset)
{
	if(!IsValid(GetMesh())) return;

	
	if (USSAnimInstance* AnimInstance = Cast<USSAnimInstance>(Mesh1p->GetAnimInstance()))
	{
		AnimInstance->OnNewWeaponEquipped(WeaponOffset);
	}

}

void ASSCharacter::SetIsFirstPerson()
{
	if(Mesh1p->bOwnerNoSee==true)
	{
		Mesh1p->SetOwnerNoSee(false);
		IsFirstPerson=true;
	}
	else
	{
		Mesh1p->SetOwnerNoSee(true);
		IsFirstPerson=false;
	}
	
}

void ASSCharacter::CheckLevelUp(EWeaponType WeaponType)
{
	if(WeaponExp[WeaponType].WeaponLevel==3) return;
	// if(!WeaponExp[WeaponType].EXPThresholds[WeaponExp[WeaponType].WeaponLevel]) return;

	if(WeaponExp[WeaponType].CurrentEXP>=WeaponExp[WeaponType].EXPThresholds[WeaponExp[WeaponType].WeaponLevel]
	&& WeaponExp[WeaponType].WeaponLevel<WeaponExp[WeaponType].EXPThresholds.Num())
	{
		WeaponExp[WeaponType].CurrentEXP-=WeaponExp[WeaponType].EXPThresholds[WeaponExp[WeaponType].WeaponLevel];
		WeaponExp[WeaponType].WeaponLevel++;
		OnLevelUp(WeaponType, WeaponExp[WeaponType].WeaponLevel);
	}

	return;
}

void ASSCharacter::OnLevelUp(EWeaponType WeaponType, int32 Level)
{
	for (const auto& Row : WeaponItemTable->GetRowMap())
	{
		FWeaponItem* WeaponData = (FWeaponItem*)Row.Value;
		if (WeaponData->WeaponType ==  WeaponType && WeaponData->WeaponLevel == Level)
		{
			WeaponItemDefinition = WeaponData->WeaponItemClass;
			WeaponLevelUp(WeaponItemDefinition, this, WeaponType);
			break;
		}
	}

}


void ASSCharacter::AddEXP(EWeaponType WeaponType, float EXP)
{
	// Add EXP
	if(WeaponType== EWeaponType::Healing) return;
	if(WeaponExp.Contains(WeaponType))
	{
		
		WeaponExp[WeaponType].CurrentEXP+=EXP;
	}
	else
	{
		FWeaponExperience NewWeaponExp;
		NewWeaponExp.EXPThresholds.Add(0.0f);
		NewWeaponExp.EXPThresholds.Add(200.0f);
		NewWeaponExp.EXPThresholds.Add(300.0f);
		// NewWeaponExp.EXPThresholds.Add(400.0f);
		WeaponExp.Add(WeaponType, NewWeaponExp);


	}
	CheckLevelUp(WeaponType);
}

float ASSCharacter::GetWeaponExp(EWeaponType WeaponType) const
{
	return WeaponExp.Contains(WeaponType) ? WeaponExp[WeaponType].CurrentEXP : 0.0f;
}
