// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SSCharacter.h"

#include "Animation/LyraAnimInstance.h"
#include "Animation/SSAnimInstance.h"
#include "Camera/LyraCameraComponent.h"
#include "Character/LyraCharacterMovementComponent.h"
#include "Character/LyraHealthComponent.h"
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

void ASSCharacter::CheckLevelUp()
{

	
}

void ASSCharacter::OnLevelUp()
{
	
}

void ASSCharacter::AddEXP(EWeaponType WeaponType, float EXP)
{
	// Add EXP
	if(WeaponExp.Contains(WeaponType))
	{
		WeaponExp[WeaponType].CurrentEXP+=EXP;
	}
	else
	{
		FWeaponExperience NewWeaponExp;
		WeaponExp.Add(WeaponType, NewWeaponExp);
	}
	CheckLevelUp();
	UE_LOG( LogTemp, Log, TEXT("%d: %f"), WeaponType, WeaponExp[WeaponType].CurrentEXP);
}
