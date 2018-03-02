// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "STypes.h"
#include "Animation/AnimInstance.h"
#include "DragonAgeJourneysAnimInstance.generated.h"

/**
*
*/
UCLASS()
class DRAGONAGEJOURNEYS_API UDragonAgeJourneysAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "NativeUpdateAnimation")
		void NativeUpdateAnimation(float DeltaSeconds) override;

	void NativeInitializeAnimation() override;

	void SetMovementSpeed(float fSpeed);
	float GetMovementSpeed();

	void SetCustom(bool bCustom);
	void SetCombat(bool bCombat, EWeaponStyle weaponStyle);
	void SetDead(bool bDead);

	bool IsCustom();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float fMovementSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsCustom;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsCharGen;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsCombat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsSingleWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsDualWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsBow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsStaff;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsTwoHands;
};
