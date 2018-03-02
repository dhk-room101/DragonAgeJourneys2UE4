// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DragonAgeJourneysCharacter.h"
#include <TimerManager.h>
#include "Engine/StreamableManager.h"
#include "Animation/SkeletalMeshActor.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimClassInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "GameCharacter.h"
#include "Party.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Map.h"
#include "Battlefield.h"
#include "Hex.h"
#include "Effect.h"
#include "Options.h"
#include "Gender.h"
#include "DragonAgeJourneysAnimInstance.h"
#include "DragonAgeJourneysMeshDataAsset.h"
#include "DragonAgeJourneysLibrary.h"
#include "DragonAgeJourneysPlayerController.h"
#include "Slot.h"
#include "Equipment.h"
#include "Animation.h"

FName ADragonAgeJourneysCharacter::MeshHEDComponentName(TEXT("MeshHEDComponent"));
FName ADragonAgeJourneysCharacter::MeshHARComponentName(TEXT("MeshHARComponent"));
FName ADragonAgeJourneysCharacter::MeshEYEComponentName(TEXT("MeshEYEComponent"));
FName ADragonAgeJourneysCharacter::MeshARMComponentName(TEXT("MeshARMComponent"));

FName ADragonAgeJourneysCharacter::MeshRGTComponentName(TEXT("MeshRGTComponent"));
FName ADragonAgeJourneysCharacter::MeshLFTComponentName(TEXT("MeshLFTComponent"));

ADragonAgeJourneysCharacter::ADragonAgeJourneysCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.DoNotCreateDefaultSubobject(TEXT("CharacterMesh0")))
{
	this->SetActorRelativeScale3D(FVector(0.3, 0.3, 0.3));//DHK

	MeshComponentSkeletalHED = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ADragonAgeJourneysCharacter::MeshHEDComponentName);
	if (MeshComponentSkeletalHED != nullptr)
		InitSkeletalMeshComponent(MeshComponentSkeletalHED, false);

	MeshComponentSkeletalHAR = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ADragonAgeJourneysCharacter::MeshHARComponentName);
	if (MeshComponentSkeletalHAR != nullptr)
		InitSkeletalMeshComponent(MeshComponentSkeletalHAR, true);

	MeshComponentSkeletalEYE = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ADragonAgeJourneysCharacter::MeshEYEComponentName);
	if (MeshComponentSkeletalEYE != nullptr)
		InitSkeletalMeshComponent(MeshComponentSkeletalEYE, true);

	MeshComponentSkeletalARM = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ADragonAgeJourneysCharacter::MeshARMComponentName);
	if (MeshComponentSkeletalARM != nullptr)
		InitSkeletalMeshComponent(MeshComponentSkeletalARM, true);

	MeshComponentStaticRGT = CreateOptionalDefaultSubobject<UStaticMeshComponent>(ADragonAgeJourneysCharacter::MeshRGTComponentName);
	MeshComponentStaticLFT = CreateOptionalDefaultSubobject<UStaticMeshComponent>(ADragonAgeJourneysCharacter::MeshLFTComponentName);

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-45.f, -90.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Data/Art/Materials/MSC/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	//TEMP DHK Disable decal cursor
	CursorToWorld->SetVisibility(false, true);
	CursorToWorld = nullptr;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADragonAgeJourneysCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}*/

	/*if (bIsActive)
		TickAvatar();*/
}

//skeletal
void ADragonAgeJourneysCharacter::InitSkeletalMeshComponent(TWeakObjectPtr<class USkeletalMeshComponent> SSkeletalMeshPointer, bool AttachToParent)
{
	SSkeletalMeshPointer->AlwaysLoadOnClient = true;
	SSkeletalMeshPointer->AlwaysLoadOnServer = true;
	SSkeletalMeshPointer->bOwnerNoSee = false;
	SSkeletalMeshPointer->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
	SSkeletalMeshPointer->bCastDynamicShadow = true;
	SSkeletalMeshPointer->bAffectDynamicIndirectLighting = true;
	SSkeletalMeshPointer->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	// force tick after movement component updates
	if (GetCharacterMovement())
	{
		SSkeletalMeshPointer->PrimaryComponentTick.AddPrerequisite(this, GetCharacterMovement()->PrimaryComponentTick);
	}

	//SMeshPointer->AttachParent = GetCapsuleComponent();
	SSkeletalMeshPointer->SetupAttachment(GetCapsuleComponent());
	static FName CollisionProfileName(TEXT("CharacterMesh"));
	SSkeletalMeshPointer->SetCollisionObjectType(ECC_Pawn);
	SSkeletalMeshPointer->SetCollisionProfileName(CollisionProfileName);
	SSkeletalMeshPointer->bGenerateOverlapEvents = false;

	// Mesh acts as the head, as well as the parent for both animation and attachment.
	if (AttachToParent)
	{
		//SMeshPointer->AttachParent = MeshHEDComponent;
		SSkeletalMeshPointer->SetupAttachment(MeshComponentSkeletalHED);
		SSkeletalMeshPointer->SetMasterPoseComponent(MeshComponentSkeletalHED);
		SSkeletalMeshPointer->bUseBoundsFromMasterPoseComponent = true;
	}
}

//TODO randomize meshes
void ADragonAgeJourneysCharacter::InitDefaultSkeletalMeshes()
{
	FString sPath;
	FName temp;

	if (GetCharRef()->IsMonster())
	{
		FString sPath = "/Game/Data/Art/Meshes/Monsters/";

		//head
		LoadedSkeletalMeshesDatabase->SkeletalMeshList.Add(FSkeletalMeshInfo());
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_HEAD].MeshID = MESH_HEAD;
		temp = FName(*("cn_hed_" + GetLetterRace(GetCharRef()->GetRace()) + "_0"));
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_HEAD].MeshResource = FStringAssetReference(sPath + "Head/" + temp.ToString() + "." + temp.ToString());

		//eyes
		LoadedSkeletalMeshesDatabase->SkeletalMeshList.Add(FSkeletalMeshInfo());
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_EYES].MeshID = MESH_EYES;
		temp = FName(*("cn_eye_" + GetLetterRace(GetCharRef()->GetRace()) + "_0"));
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_EYES].MeshResource = FStringAssetReference(sPath + "Eyes/" + temp.ToString() + "." + temp.ToString());

		//arm
		LoadedSkeletalMeshesDatabase->SkeletalMeshList.Add(FSkeletalMeshInfo());
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_ARM].MeshID = MESH_ARM;
		temp = FName(*("cn_bdy_" + GetLetterRace(GetCharRef()->GetRace()) + "_0"));
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_ARM].MeshResource = FStringAssetReference(sPath + "Body/" + temp.ToString() + "." + temp.ToString());

		// Registers the loading requests
		ChangeMeshHED();
		ChangeMeshEYE();
		ChangeMeshARM();
	}
	else
	{
		FString sPath = "/Game/Data/Art/Meshes/UTC/";

		//head
		LoadedSkeletalMeshesDatabase->SkeletalMeshList.Add(FSkeletalMeshInfo());
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_HEAD].MeshID = MESH_HEAD;
		temp = FName(*(GetLetterRace(GetCharRef()->GetRace()) + GetLetterGender(GetCharRef()->gender) + "_uhm_basa_0"));
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_HEAD].MeshResource = FStringAssetReference(sPath + "Head/" + temp.ToString() + "." + temp.ToString());

		//hair 
		LoadedSkeletalMeshesDatabase->SkeletalMeshList.Add(FSkeletalMeshInfo());
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_HAIR].MeshID = MESH_HAIR;
		if (GetCharRef()->gender == GENDER_FEMALE)
		{
			temp = FName(*(GetLetterRace(GetCharRef()->GetRace()) + GetLetterGender(GetCharRef()->gender) + "_har_hd1a_0"));
		}
		else temp = FName(*(GetLetterRace(GetCharRef()->GetRace()) + GetLetterGender(GetCharRef()->gender) + "_har_hb3a_0"));
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_HAIR].MeshResource = FStringAssetReference(sPath + "Hair/" + temp.ToString() + "." + temp.ToString());

		//eyes
		LoadedSkeletalMeshesDatabase->SkeletalMeshList.Add(FSkeletalMeshInfo());
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_EYES].MeshID = MESH_EYES;
		temp = FName(*(GetLetterRace(GetCharRef()->GetRace()) + GetLetterGender(GetCharRef()->gender) + "_uem_basa_0"));
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_EYES].MeshResource = FStringAssetReference(sPath + "Eyes/" + temp.ToString() + "." + temp.ToString());

		//arm
		LoadedSkeletalMeshesDatabase->SkeletalMeshList.Add(FSkeletalMeshInfo());
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_ARM].MeshID = MESH_ARM;
		FString sNude = GetLetterRace(GetCharRef()->GetRace()) + GetLetterGender(GetCharRef()->gender) + "_arm_nuda_0";
		LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_ARM].MeshResource = FStringAssetReference(sPath + "Body/" + sNude + "." + sNude);

		// Registers the loading requests
		ChangeMeshHED();
		ChangeMeshHAR();
		ChangeMeshEYE();
		if (GetParty()->GetHeroCharacter() != GetCharRef())
			ChangeMeshARM();
	}
}

bool ADragonAgeJourneysCharacter::ChangeMeshHED()
{
	bool IsValid;

	if (LoadedSkeletalMeshesDatabase != NULL && LoadedSkeletalMeshesDatabase->SkeletalMeshList.Num() >= MESH_HEAD)
	{
		TArray<FStringAssetReference> ObjToLoad;
		FStreamableManager& BaseLoader = UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->AssetLoader;
		AssetToLoadHED = LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_HEAD].MeshResource.ToSoftObjectPath();
		ObjToLoad.AddUnique(AssetToLoadHED);
		BaseLoader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &ADragonAgeJourneysCharacter::DoAsyncMeshChangeHED));
		return true;
	}

	return false;
}

bool ADragonAgeJourneysCharacter::ChangeMeshHAR()
{
	bool IsValid;

	// Prevents out of index access
	if (LoadedSkeletalMeshesDatabase != NULL && LoadedSkeletalMeshesDatabase->SkeletalMeshList.Num() >= MESH_HAIR)
	{
		TArray<FStringAssetReference> ObjToLoad;
		FStreamableManager& BaseLoader = UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->AssetLoader;
		AssetToLoadHAR = LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_HAIR].MeshResource.ToSoftObjectPath();
		ObjToLoad.AddUnique(AssetToLoadHAR);
		BaseLoader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &ADragonAgeJourneysCharacter::DoAsyncMeshChangeHAR));
		return true;
	}

	return false;
}

bool ADragonAgeJourneysCharacter::ChangeMeshEYE()
{
	bool IsValid;

	// Prevents out of index access
	if (LoadedSkeletalMeshesDatabase != NULL && LoadedSkeletalMeshesDatabase->SkeletalMeshList.Num() >= MESH_EYES)
	{
		TArray<FStringAssetReference> ObjToLoad;
		FStreamableManager& BaseLoader = UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->AssetLoader;
		AssetToLoadEYE = LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_EYES].MeshResource.ToSoftObjectPath();
		ObjToLoad.AddUnique(AssetToLoadEYE);
		BaseLoader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &ADragonAgeJourneysCharacter::DoAsyncMeshChangeEYE));
		return true;
	}

	return false;
}

bool ADragonAgeJourneysCharacter::ChangeMeshARM()
{
	bool IsValid;

	// Prevents out of index access
	if (LoadedSkeletalMeshesDatabase != NULL && LoadedSkeletalMeshesDatabase->SkeletalMeshList.Num() >= MESH_ARM)
	{
		TArray<FStringAssetReference> ObjToLoad;
		FStreamableManager& BaseLoader = UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->AssetLoader;
		AssetToLoadARM = LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_ARM].MeshResource.ToSoftObjectPath();
		ObjToLoad.AddUnique(AssetToLoadARM);
		BaseLoader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &ADragonAgeJourneysCharacter::DoAsyncMeshChangeARM));
		return true;
	}

	return false;
}

// DELEGATE - Do the Async HED Change
void ADragonAgeJourneysCharacter::DoAsyncMeshChangeHED()
{
	check(AssetToLoadHED.ResolveObject() != nullptr)
	{
		// Creates a pointer to store the loaded object
		UObject* NewMeshHED = AssetToLoadHED.ResolveObject();
		// Casts and assigns
		MeshComponentSkeletalHED->SetSkeletalMesh(Cast<USkeletalMesh>(NewMeshHED));

		if (!AnimInstance)
		{
			AnimInstance = Cast<UDragonAgeJourneysAnimInstance>(MeshComponentSkeletalHED->GetAnimInstance());
			//for avatars spawned on the battlefield, make them enter combat stance by default
			if (GetCurrentPlayerController()->currentGameMode == EUI::COMBAT)
				SetCombatStance();/*AnimInstance->SetCombat(true);*/
		}

		/*IAnimClassInterface* iface = IAnimClassInterface::GetFromClass(AnimInstance->GetClass());
		MeshComponentSkeletalHED->SetAnimInstanceClass(refAnimInstance->GetClass());*/

		MeshComponentSkeletalHED->SetRelativeLocationAndRotation(FVector(0, 0, -96), FRotator(0, 90, 0));

		MeshComponentSkeletalHAR->SetMasterPoseComponent(MeshComponentSkeletalHED);
		MeshComponentSkeletalEYE->SetMasterPoseComponent(MeshComponentSkeletalHED);
		MeshComponentSkeletalARM->SetMasterPoseComponent(MeshComponentSkeletalHED);
	}
}

void ADragonAgeJourneysCharacter::DoAsyncMeshChangeHAR()
{
	check(AssetToLoadHAR.ResolveObject() != nullptr)
	{
		UObject* NewMeshHAR = AssetToLoadHAR.ResolveObject();
		MeshComponentSkeletalHAR->SetSkeletalMesh(Cast<USkeletalMesh>(NewMeshHAR));
	}
}

void ADragonAgeJourneysCharacter::DoAsyncMeshChangeEYE()
{
	check(AssetToLoadEYE.ResolveObject() != nullptr)
	{
		UObject* NewMeshEYE = AssetToLoadEYE.ResolveObject();
		MeshComponentSkeletalEYE->SetSkeletalMesh(Cast<USkeletalMesh>(NewMeshEYE));
	}
}

void ADragonAgeJourneysCharacter::DoAsyncMeshChangeARM()
{
	check(AssetToLoadARM.ResolveObject() != nullptr)
	{
		bool bReplace = false;
		if (MeshComponentSkeletalARM->SkeletalMesh)
		{
			bReplace = true;
		}
		else bReplace = false;

		UObject* NewMeshARM = AssetToLoadARM.ResolveObject();
		MeshComponentSkeletalARM->SetSkeletalMesh(Cast<USkeletalMesh>(NewMeshARM));

		//equip weapon static mesh
		if (GetParty()->GetHeroCharacter() != GetCharRef())
		{
			if (!bReplace)
			{
				if (GetCharRef()->gear[SLOT_WEAPON])
				{
					GetCharRef()->EquipDefaultWeapon(GetCharRef()->gear[SLOT_WEAPON]);
				}
			}
		}
	}
}

void ADragonAgeJourneysCharacter::InitDefaultStaticMeshes()
{
	//right
	LoadedStaticMeshesDatabase->StaticMeshList.Add(FStaticMeshInfo());
	LoadedStaticMeshesDatabase->StaticMeshList[MESH_RIGHT].MeshID = MESH_RIGHT;

	//left
	LoadedStaticMeshesDatabase->StaticMeshList.Add(FStaticMeshInfo());
	LoadedStaticMeshesDatabase->StaticMeshList[MESH_LEFT].MeshID = MESH_LEFT;

	// Registers the loading requests
	// 	ChangeMeshRGT();
	// 	ChangeMeshLFT();
}

void ADragonAgeJourneysCharacter::EquipWeapon(bool bMain)
{
	if (bMain)
	{
		if (GetCharRef()->gear[SLOT_WEAPON]
			&& GetCharRef()->gear[SLOT_WEAPON]->IsRanged())
			MeshComponentStaticRGT->AttachToComponent(MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnWeaponLFTOn);
		else 
			MeshComponentStaticRGT->AttachToComponent(MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnWeaponRGTOn);
	}
	else
	{
		if (GetCharRef()->gear[SLOT_OFFHAND] &&
			GetCharRef()->gear[SLOT_OFFHAND]->IsShield())
			MeshComponentStaticLFT->AttachToComponent(MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnShieldOn);
		else if (GetCharRef()->gear[SLOT_OFFHAND] &&
			GetCharRef()->gear[SLOT_OFFHAND]->IsWeapon())
			MeshComponentStaticLFT->AttachToComponent(MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnWeaponLFTOn);

	}
}

bool ADragonAgeJourneysCharacter::ChangeMeshRGT()
{
	bool IsValid;

	if (LoadedStaticMeshesDatabase != NULL && LoadedStaticMeshesDatabase->StaticMeshList.Num() >= MESH_RIGHT)
	{
		TArray<FStringAssetReference> ObjToLoad;
		FStreamableManager& BaseLoader = UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->AssetLoader;
		AssetToLoadRGT = LoadedStaticMeshesDatabase->StaticMeshList[MESH_RIGHT].MeshResource.ToSoftObjectPath();
		ObjToLoad.AddUnique(AssetToLoadRGT);
		BaseLoader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &ADragonAgeJourneysCharacter::DoAsyncMeshChangeRGT));
		return true;
	}

	return false;
}

bool ADragonAgeJourneysCharacter::ChangeMeshLFT()
{
	bool IsValid;

	if (LoadedStaticMeshesDatabase != NULL && LoadedStaticMeshesDatabase->StaticMeshList.Num() >= MESH_LEFT)
	{
		TArray<FStringAssetReference> ObjToLoad;
		FStreamableManager& BaseLoader = UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->AssetLoader;
		AssetToLoadLFT = LoadedStaticMeshesDatabase->StaticMeshList[MESH_LEFT].MeshResource.ToSoftObjectPath();
		ObjToLoad.AddUnique(AssetToLoadLFT);
		BaseLoader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &ADragonAgeJourneysCharacter::DoAsyncMeshChangeLFT));
		return true;
	}

	return false;
}

void ADragonAgeJourneysCharacter::DoAsyncMeshChangeRGT()
{
	check(AssetToLoadRGT.ResolveObject() != nullptr)
	{
		UObject* NewMeshRGT = AssetToLoadRGT.ResolveObject();
		MeshComponentStaticRGT->SetStaticMesh(Cast<UStaticMesh>(NewMeshRGT));
		if (GetCurrentPlayerController()->currentGameMode == EUI::COMBAT)
			EquipWeapon(true);
	}
}

void ADragonAgeJourneysCharacter::DoAsyncMeshChangeLFT()
{
	check(AssetToLoadLFT.ResolveObject() != nullptr)
	{
		UObject* NewMeshLFT = AssetToLoadLFT.ResolveObject();
		MeshComponentStaticLFT->SetStaticMesh(Cast<UStaticMesh>(NewMeshLFT));
		//mesh on or off if combat or explore
		if (GetCurrentPlayerController()->currentGameMode == EUI::COMBAT)
			EquipWeapon(false);
	}
}

void ADragonAgeJourneysCharacter::PlayCustomAnimation(FString sAnimation)
{
	AnimSequence = GetAnimationResource(sAnimation);
	if (GetCurrentPlayerController()->currentGameMode == EUI::CHARGEN)
	{

	}
	else
	{
		AnimInstance->PlaySlotAnimationAsDynamicMontage(AnimSequence, FAnimSlotGroup::DefaultSlotName, 0.1f, 0.1f);
	}

	float endTimer = AnimSequence->GetPlayLength();
	GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &ADragonAgeJourneysCharacter::ResetCustomMontage, endTimer, false);

	AnimInstance->SetCustom(true);
}

void ADragonAgeJourneysCharacter::InitializeMeshDatabases()
{
	//skeletal
	if (!LoadedSkeletalMeshesDatabase)
	{
		LoadedSkeletalMeshesDatabase = NewObject<UDragonAgeJourneysMeshDataAsset>();
		for (int32 i = 0; i < MESHES_SKELETAL; i++)
		{
			LoadedSkeletalMeshesDatabase->SkeletalMeshList.Add(FSkeletalMeshInfo());
		}
	}

	//static weapons
	if (!LoadedStaticMeshesDatabase)
	{
		LoadedStaticMeshesDatabase = NewObject<UDragonAgeJourneysMeshDataAsset>();
		for (int32 i = 0; i < MESHES_STATIC; i++)
		{
			LoadedStaticMeshesDatabase->StaticMeshList.Add(FStaticMeshInfo());
		}
	}
}

void ADragonAgeJourneysCharacter::InitializeAvatar(UGameCharacter* gameCharacter, bool bActive /*= false*/)
{
	UE_LOG(LogTemp, Warning, TEXT("InitializeAvatar %s"), *GetName());

	_charRef = gameCharacter;

	InitializeMeshDatabases();
	InitDefaultSkeletalMeshes();

	//TODO soundset
	//if(gameCharacter)
		//walkSounds = Sounds.getWalkSoundsForRace(gameCharacter->GetRace());

	if (bActive)
	{
		gameCharacter->avatar = this;
		bIsActive = bActive;
	}
}

void ADragonAgeJourneysCharacter::SetOutline(int32 nStencil, bool bEnable)
{
	GetMesh()->SetRenderCustomDepth(bEnable);
	GetMesh()->CustomDepthStencilValue = nStencil;
}

void ADragonAgeJourneysCharacter::AnimateAvatar(int32 nAnimation)
{
	currentAnimation = nAnimation;
	//TODO AnimateAvatar switch
	if (currentAnimation == ANIMATION_ATTACK)
	{	//mh_c_bw_fire
		
		if (GetCharRef()->gear[SLOT_WEAPON]
			&& GetCharRef()->gear[SLOT_WEAPON]->IsStaff())
			PlayCustomAnimation("mh_c_stf_spl_a_cst");
		else if (GetCharRef()->gear[SLOT_WEAPON]
			&& GetCharRef()->gear[SLOT_WEAPON]->IsRanged())
			PlayCustomAnimation("mh_c_bw_fire");
		/*else if (GetCharRef()->gear[SLOT_WEAPON]
			&& GetCharRef()->gear[SLOT_WEAPON]->IsTwoHander())
			PlayCustomAnimation("//TODO");*/
		else if (GetCharRef()->gear[SLOT_WEAPON]
			&& GetCharRef()->gear[SLOT_WEAPON]->IsMelee()
			&& !GetCharRef()->gear[SLOT_WEAPON]->IsTwoHander())
		{
			if (GetCharRef()->gear[SLOT_OFFHAND]
				&& GetCharRef()->gear[SLOT_OFFHAND]->IsMelee()) //DUAL DAGGER
				PlayCustomAnimation("mh_c_dw_atk_v1");
			else
				PlayCustomAnimation("mh_c_sw_atk_v1");//SWORD
		}
	}
	else if (currentAnimation == ANIMATION_DEATH)
	{
		//PlayCustomAnimation("mh_c_sw_die_v2");
		AnimInstance->SetDead(true);
	}
}

void ADragonAgeJourneysCharacter::ClearLargeHexes()
{
	for (int32 i = 0; i < largeHexes.Num(); i++)
	{
		AHex* loc1 = largeHexes[i];
		loc1->largeAvatar = nullptr;
	}
	largeHexes.Empty();
}

void ADragonAgeJourneysCharacter::AddLargeHex(AHex* hex)
{
	if (hex == nullptr)
	{
		return;
	}

	largeHexes.Add(hex);
	hex->largeAvatar = this;
}

void ADragonAgeJourneysCharacter::MoveTowards(float fEndX, float fEndY, int32 nBaseSpeed /*= -1*/)
{
	FVector vLoc = FVector(GetActorLocation().X, GetActorLocation().Y, GetMain()->LevelMap->MapZ);
	FVector vDest = FVector(fEndX, fEndY, GetMain()->LevelMap->MapZ);

	int32 nSpeed = GetCharRef()->GetSpeed() * BATTLESPEED;
	if (nBaseSpeed != -1)
	{
		nSpeed = nBaseSpeed;
	}

	SetActorLocation(FMath::VInterpConstantTo(vLoc, vDest, GetMain()->Battlefield->DeltaSeconds, nSpeed));

	/*float fRelX = fEndX - GetActorLocation().X;
	float fRelY = fEndY - GetActorLocation().Y;
	FVector fVec = FVector(fRelX, fRelY, GetMain()->LevelMap->MapZ);*/

	//fVec.normalize(1);

// 	float fSpeedX = fVec.X * nSpeed;
// 	float fSpeedY = fVec.Y * nSpeed;

	/*if (FMath::Abs(fRelX) < FMath::Abs(fSpeedX))
	{
		fSpeedX = fRelX;
	}
	if (FMath::Abs(fRelY) < FMath::Abs(fSpeedY))
	{
		fSpeedY = fRelY;
	}*/

	/*x = x + loc5;
	y = y + loc6;*/

	// 	FVector vDest = FVector(GetActorLocation().X + fSpeedX, GetActorLocation().Y + fSpeedY, GetActorLocation().Z);
	// 	movedY = movedY + FMath::Abs(fSpeedY);

		//FVector vDest1 = FVector(mapHex->GetActorLocation().X, mapHex->GetActorLocation().Y, MapZ);
		//FVector vResult = FMath::VInterpConstantTo(vEnd, vEnd, GetMain()->Battlefield->DeltaSeconds, 1 /*//TODO hardcoded?*/);

		//TODO rotation
		/*if (loc5 < 0)
		{
			setFacingLeft(true);
		}
		else if (loc5 > 0)
		{
			setFacingLeft(false);
		}*/
}

void ADragonAgeJourneysCharacter::AddFloatingText(FString sText, int32 nColor /*= 16777215*/)
{
	//TODO AddFloatingText
	UE_LOG(LogTemp, Warning, TEXT("AddFloatingText %s %s"), *GetName(), *sText);
	GetCurrentPlayerController()->DisplayFloaty(this, sText, 0, nColor, 1);
}

void ADragonAgeJourneysCharacter::AddFloatingDamage(int32 nAmount, bool bCritical /*= false*/)
{
	int32 nColor = 0;
	if (bCritical)
	{
		//TODO VFX shake
		//packages.battle.Battlefield.current.fx.shake();

		AddFloatingText("Critical!", 16711680);
	}

	if (FLOATING_DAMAGE)
	{
		if (bCritical)
		{
			nColor = 16711680;
		}
		else
		{
			nColor = 16777215;
		}

		AddFloatingText(FString::FromInt(nAmount), nColor);
	}

}

void ADragonAgeJourneysCharacter::AddFloatingHealing(int32 nAmount)
{
	if (FLOATING_DAMAGE)
	{
		AddFloatingText(FString::FromInt(nAmount), 3407667 /*33FF33*/);
	}
}

void ADragonAgeJourneysCharacter::EffectAdded(UEffect* effect)
{
	if (!effect->suppressFloatingNotification)
	{
		AddFloatingText("+" + GetEffectName(effect->effectId), 16777215);
	}
	if (effect->vfx == -1)
	{
		return;
	}
	/*switch (effect->vfx)
	{
		//TODO Add VFX
	}*/
}

void ADragonAgeJourneysCharacter::EffectRemoved(UEffect* effect)
{
	if (effect->vfx == -1)
	{
		return;
	}
	/*switch (effect->vfx)
	{
		//TODO Remove VFX
	}*/
}

void ADragonAgeJourneysCharacter::FaceAttacker(ADragonAgeJourneysCharacter* attacker)
{
	//TODO rotation toward attacker
}

void ADragonAgeJourneysCharacter::FaceHex(AHex* hex)
{
	//TODO rotation toward hex
}

void ADragonAgeJourneysCharacter::SetFacingLeft(bool bLeft)
{
	//TODO setFacingLeft to rotation
	//and check for setFacingLeft or SetFacingLeft code to update
}

void ADragonAgeJourneysCharacter::SetCombatStance()
{
	if (GetCharRef()->gear[SLOT_WEAPON]
		&& GetCharRef()->gear[SLOT_WEAPON]->IsStaff())
		AnimInstance->SetCombat(true, EWeaponStyle::STAFF);
	else if (GetCharRef()->gear[SLOT_WEAPON]
		&& GetCharRef()->gear[SLOT_WEAPON]->IsRanged())
		AnimInstance->SetCombat(true, EWeaponStyle::BOW);
	else if (GetCharRef()->gear[SLOT_WEAPON]
		&& GetCharRef()->gear[SLOT_WEAPON]->IsTwoHander())
		AnimInstance->SetCombat(true, EWeaponStyle::TWOHANDS);
	else if (GetCharRef()->gear[SLOT_WEAPON]
		&& GetCharRef()->gear[SLOT_WEAPON]->IsMelee())
	{
		if (GetCharRef()->gear[SLOT_OFFHAND]
			&& GetCharRef()->gear[SLOT_OFFHAND]->IsMelee())
			AnimInstance->SetCombat(true, EWeaponStyle::DUAL);
		else 
			AnimInstance->SetCombat(true, EWeaponStyle::SINGLE);
	}
}

bool ADragonAgeJourneysCharacter::GetFacingLeft()
{
	//TODO relative rotation, facing the other way
	return _facingLeft;
}

bool ADragonAgeJourneysCharacter::IsAnimationAtFinalFrame()
{
	//TODO IsAnimationAtFinalFrame
	return false;
}

UGameCharacter* ADragonAgeJourneysCharacter::GetCharRef()
{
	return _charRef;
}

void ADragonAgeJourneysCharacter::TickAvatar()
{
	//TODO TickAvatar?
}

void ADragonAgeJourneysCharacter::ResetCustomMontage()
{
	if (GetCurrentPlayerController()->currentGameMode == EUI::CHARGEN)
	{
		float _z = GetActorRotation().Yaw;
		SetActorRelativeRotation(FRotator(0, _z - 90, 0));
	}

	AnimInstance->SetCustom(false);
}