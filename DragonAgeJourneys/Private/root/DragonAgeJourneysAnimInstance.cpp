// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonAgeJourneysAnimInstance.h"
#include "DragonAgeJourneys.h"
#include "DragonAgeJourneysCharacter.h"
#include "DragonAgeJourneysPlayerController.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "GameFramework/NavMovementComponent.h"
#include "Scripting.h"
#include <TimerManager.h>

void UDragonAgeJourneysAnimInstance::NativeInitializeAnimation()
{

}

void UDragonAgeJourneysAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	/*ADragonAgeJourneysCharacter* _avatar = Cast<ADragonAgeJourneysCharacter>(TryGetPawnOwner());

	if (_avatar)
	{

	}*/
}

void UDragonAgeJourneysAnimInstance::SetMovementSpeed(float fSpeed)
{
	fMovementSpeed = fSpeed;
}

float UDragonAgeJourneysAnimInstance::GetMovementSpeed()
{
	return fMovementSpeed;
}

void UDragonAgeJourneysAnimInstance::SetCustom(bool bCustom)
{
	if (GetCurrentPlayerController()->currentGameMode == EUI::CHARGEN)
		bIsCharGen = bCustom;
	else bIsCustom = bCustom;
}

void UDragonAgeJourneysAnimInstance::SetCombat(bool bCombat, EWeaponStyle weaponStyle)
{
	bIsCombat = bCombat;

	switch (static_cast<uint32>(weaponStyle))
	{
	case static_cast<uint32>(EWeaponStyle::SINGLE) :
	{
		bIsSingleWeapon = true;
		break;
	}
	case static_cast<uint32>(EWeaponStyle::DUAL) :
	{
		bIsDualWeapon = true;
		break;
	}
	case static_cast<uint32>(EWeaponStyle::BOW) :
	{
		bIsBow = true;
		break;
	}
	case static_cast<uint32>(EWeaponStyle::STAFF) :
	{
		bIsStaff = true;
		break;
	}
	case static_cast<uint32>(EWeaponStyle::TWOHANDS) :
	{
		bIsTwoHands = true;
		break;
	}
	default:
		break;
	}
}

void UDragonAgeJourneysAnimInstance::SetDead(bool bDead)
{
	bIsDead = bDead;
}

bool UDragonAgeJourneysAnimInstance::IsCustom()
{
	return bIsCustom;
}
