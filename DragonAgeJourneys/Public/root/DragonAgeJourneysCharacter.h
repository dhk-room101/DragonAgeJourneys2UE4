// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DragonAgeJourneysCharacter.generated.h"

class UGameCharacter;
class AHex;
class UEffect;
class UDragonAgeJourneysAnimInstance;
class UAnimSequence;
class UAnimMontage;
class UDragonAgeJourneysMeshDataAsset;

UCLASS(Blueprintable)
class ADragonAgeJourneysCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADragonAgeJourneysCharacter(const FObjectInitializer& ObjectInitializer);

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	//DHK
	UPROPERTY()
		UDragonAgeJourneysMeshDataAsset* LoadedSkeletalMeshesDatabase = nullptr;

	UPROPERTY()
		UDragonAgeJourneysMeshDataAsset* LoadedStaticMeshesDatabase = nullptr;

	UPROPERTY()
		UAnimSequence* AnimSequence = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Anim)
		USkeleton* Skeleton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Anim)
		USkeletalMeshComponent* MeshComponentSkeletalHED = nullptr;
	static FName MeshHEDComponentName;

	UPROPERTY()
		USkeletalMeshComponent* MeshComponentSkeletalHAR = nullptr;
	static FName MeshHARComponentName;

	UPROPERTY()
		USkeletalMeshComponent* MeshComponentSkeletalEYE = nullptr;
	static FName MeshEYEComponentName;

	UPROPERTY()
		USkeletalMeshComponent* MeshComponentSkeletalARM = nullptr;
	static FName MeshARMComponentName;

	// Method to setup the new character
	void InitDefaultSkeletalMeshes();
	void InitDefaultStaticMeshes();
	void EquipWeapon(bool bMain);

	// Direct ID mesh changing "request" methods, usable with inventory loading, swapping systems... etc
	bool ChangeMeshHED();
	bool ChangeMeshHAR();
	bool ChangeMeshEYE();
	bool ChangeMeshARM();

	//right hand static mesh weapon
	UPROPERTY()
		UStaticMeshComponent* MeshComponentStaticRGT = nullptr;
	static FName MeshRGTComponentName;

	//left hand static mesh weapon
	UPROPERTY()
		UStaticMeshComponent* MeshComponentStaticLFT = nullptr;
	static FName MeshLFTComponentName;

	bool ChangeMeshRGT();
	bool ChangeMeshLFT();

	UPROPERTY()
		UDragonAgeJourneysAnimInstance* AnimInstance = nullptr;

	void PlayCustomAnimation(FString sAnimation);

	UPROPERTY()
		AHex* hex = nullptr;
	UPROPERTY()
		AHex* deathHex = nullptr;
	UPROPERTY()
		TArray<AHex*> largeHexes;
	int32 battlefieldX = 0;
	int32 battlefieldY = 0;
	int32 movedY = 0;
	int32 currentAnimation = 1;
	FString currentLabel;

	void InitializeAvatar(UGameCharacter* gameCharacter, bool bActive = false);
	void SetOutline(int32 nStencil, bool bEnable = true);
	void AnimateAvatar(int32 nAnimation);
	void ClearLargeHexes();
	void AddLargeHex(AHex* hex);
	void MoveTowards(float nX, float nY, int32 nBaseSpeed = -1);
	void AddFloatingText(FString sText, int32 nColor = 0xFFFFFF);
	void AddFloatingDamage(int32 nAmount, bool bCritical = false);
	void AddFloatingHealing(int32 nAmount);
	void EffectAdded(UEffect* effect);
	void EffectRemoved(UEffect* effect);
	void FaceAttacker(ADragonAgeJourneysCharacter* attacker);
	void FaceHex(AHex* hex);
	void SetFacingLeft(bool bLeft);
	void SetCombatStance();
	bool GetFacingLeft();
	bool IsAnimationAtFinalFrame();
	UGameCharacter* GetCharRef();

private:
	//skeletal
	FStringAssetReference AssetToLoadHED;
	FStringAssetReference AssetToLoadHAR;
	FStringAssetReference AssetToLoadEYE;
	FStringAssetReference AssetToLoadARM;

	//static
	FStringAssetReference AssetToLoadRGT;
	FStringAssetReference AssetToLoadLFT;

	// Method to setup/initialize skeletal mesh components
	void InitSkeletalMeshComponent(TWeakObjectPtr<class USkeletalMeshComponent> SSkeletalMeshPointer, bool AttachToParent);

	// Delegates to be "shoot" at end of loading processes
	void DoAsyncMeshChangeHED();
	void DoAsyncMeshChangeHAR();
	void DoAsyncMeshChangeEYE();
	void DoAsyncMeshChangeARM();

	void DoAsyncMeshChangeRGT();
	void DoAsyncMeshChangeLFT();

	void InitializeMeshDatabases();
	
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	void TickAvatar();

	UPROPERTY()
		UGameCharacter* _charRef = nullptr;
	bool bIsActive = false;
	bool _facingLeft = false;

	UPROPERTY()
		FTimerHandle MontageTimerHandle;
	UFUNCTION()
		void ResetCustomMontage();
};

