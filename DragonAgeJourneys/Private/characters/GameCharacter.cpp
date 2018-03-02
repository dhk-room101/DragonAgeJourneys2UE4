#include "GameCharacter.h"
#include "Difficulty.h"
#include "Skill.h"
#include "Equipment.h"
#include "Classes.h"
#include "Scripting.h"
#include "Effect.h"
#include "DragonMain.h"
#include "Battlefield.h"
#include "TalentLine.h"
#include "Intelligence.h"
#include "Skills.h"
#include "Slot.h"
#include "Party.h"
#include "Tutorial.h"
#include "Race.h"
#include "Animation.h"
#include "DragonAgeJourneysCharacter.h"
#include "ItemTypes.h"
#include "Hex.h"
#include "UniqueItems.h"
#include "Gender.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "DragonAgeJourneysMeshDataAsset.h"
#include "DragonAgeJourneysPlayerController.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include <TimerManager.h>
#include "DragonAgeJourneysAnimInstance.h"

UGameCharacter::UGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UGameCharacter::InitializeGameCharacter(int32 nClassId, bool bIsAI)
{
	health = GetHealthMax();
	power = GetPowerMax();
	effects.Empty();
	gear.SetNum(GEAR_SLOTS);
	classId = nClassId;
	_speed = BaseSpeedFor(nClassId);
	description = GetDescriptionForClass(nClassId);
	convDescription = GetConversationDescription(nClassId);
	SetBaseStats(this);
	if (bIsAI)
	{
		ai = NewObject<UIntelligence>();
		ai->InitializeIntelligence(this);
		charName = GetNameForClass(nClassId);
	}
	talents.Empty();
	talents = GetTalentsForClass(this);
	InitSkills();
	health = GetHealthMax();
	power = GetPowerMax();

	//DHK
	const TCHAR* arChar = *charName;
	this->Rename(arChar, this->GetOuter(), REN_None);
}

void UGameCharacter::DefaultEquipment()
{
	UEquipment* _item = nullptr;
	gear.Empty();
	gear.SetNum(GEAR_SLOTS);

	if (gear[SLOT_ARMOR] == nullptr && defaultArmor > -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("adding default armor(%i) for %s, default hue modifier is %i"), defaultArmor, *charName, defaultArmorHue);

		_item = GetUniqueItem(defaultArmor);
		_item->isDefault = true;
		_item->playerOwner = this;

		if (defaultArmorHue != 0)
		{
			_item->hue = defaultArmorHue;
		}

		gear[SLOT_ARMOR] = _item;
		EquipDefaultArmor(_item);
	}

	if (gear[SLOT_AMMO] == nullptr)
	{
		_item = GetUniqueItem(UNIQUEITEM_AMMO_DEFAULT);
		_item->isDefault = true;
		_item->playerOwner = this;
		gear[SLOT_AMMO] = _item;
	}

	if (!avatar->MeshComponentSkeletalARM->SkeletalMesh)
		avatar->GetWorldTimerManager().SetTimer(tTimerDefaultWeapon, this, &UGameCharacter::DefaultEquipmentWeapon, 1, false);
	else DefaultEquipmentWeapon();
}

void UGameCharacter::DefaultEquipmentWeapon()
{
	UEquipment* _item = nullptr;
	if (gear[SLOT_WEAPON] == nullptr && defaultWeapon > -1)
	{
		_item = GetUniqueItem(defaultWeapon);
		_item->isDefault = true;
		_item->playerOwner = this;
		gear[SLOT_WEAPON] = _item;
		if (_item->IsTwoHanded())
		{
			Unequip(gear[SLOT_OFFHAND]);
		}
		EquipDefaultWeapon(_item);
		UE_LOG(LogTemp, Warning, TEXT("adding default weapon(%i) for %s"), _item->uniqueId, *charName);
	}
}

void UGameCharacter::SetDefaultEquipmentTemplate()
{
	if (classId == CLASS_MAGE)
	{
		defaultWeapon = UNIQUEITEM_STARTER_STAFF;
		defaultArmor = UNIQUEITEM_STARTER_ROBES;
	}
	else if (classId == CLASS_ROGUE)
	{
		if (GetRace() != RACE_ELF)
		{
			defaultWeapon = UNIQUEITEM_STARTER_DAGGER;
		}
		else defaultWeapon = UNIQUEITEM_STARTER_BOW;
		defaultArmor = UNIQUEITEM_STARTER_LEATHER;
	}
	else if (classId == CLASS_WARRIOR)
	{
		defaultWeapon = UNIQUEITEM_STARTER_LONGSWORD;
		defaultArmor = UNIQUEITEM_STARTER_ARMOR;
	}
}

void UGameCharacter::ForceEquip(UEquipment* item, int32 nSlot)
{
	item->playerOwner = this;
	gear[nSlot] = item;
}

void UGameCharacter::EquipDefaultArmor(UEquipment* itemArmor)
{
	FString sGender = (gender == GENDER_FEMALE) ? "f" : "m";
	FString sRace;
	if (_race == RACE_DWARF) sRace = "d";
	else if (_race == RACE_ELF) sRace = "e";
	else if (_race == RACE_HUMAN) sRace = "h";

	FString sClass;

	UDataTable* meshTable = GetCurrentPlayerController()->tables[ETable::MESH];
	if (meshTable)
	{
		FMeshTable* RowMesh = meshTable->FindRow<FMeshTable>(FName(*(FString::FromInt(defaultArmor))), TEXT(""));
		if (RowMesh)
		{
			sClass = "_" + RowMesh->MeshString + "_0";
		}
	}

	FString sPath;
	if (classId == CLASS_MAGE) sPath = "/Game/Data/Art/Meshes/UTI/Robes/";
	else sPath = "/Game/Data/Art/Meshes/UTI/Armor/Chest/Light/";

	FString sArmor = sRace + sGender + sClass;

	avatar->LoadedSkeletalMeshesDatabase->SkeletalMeshList[MESH_ARM].MeshResource = FStringAssetReference(sPath + sArmor + "." + sArmor);
	avatar->ChangeMeshARM();
}

void UGameCharacter::EquipDefaultWeapon(UEquipment* itemWeapon, bool bActive)
{
	//TODO if bActive, equip in hand, otherwise passive on back
	FString sWeapon;

	UDataTable* meshTable = GetCurrentPlayerController()->tables[ETable::MESH];
	if (meshTable)
	{
		FMeshTable* RowMesh = meshTable->FindRow<FMeshTable>(FName(*(FString::FromInt(itemWeapon->uniqueId))), TEXT(""));
		if (RowMesh)
		{
			sWeapon = /*"w_" + */RowMesh->MeshString + "_0";
		}
	}

	avatar->MeshComponentStaticRGT->SetStaticMesh(nullptr);
	avatar->MeshComponentStaticLFT->SetStaticMesh(nullptr);

	FString sPath;
	if (itemWeapon->uniqueId == UNIQUEITEM_STARTER_LONGSWORD || itemWeapon->typeId == ITEMTYPE_SWORD_LONG)
	{
		sPath = "/Game/Data/Art/Meshes/UTI/Weapon/Melee/Longsword/";
		avatar->LoadedStaticMeshesDatabase->StaticMeshList[MESH_RIGHT].MeshResource = FStringAssetReference(sPath + sWeapon + "." + sWeapon);
		avatar->ChangeMeshRGT();
		if (!IsMonster())
		{
			avatar->MeshComponentStaticRGT->AttachToComponent(avatar->MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnLongswordOff);
		
			ForceEquip(GetUniqueItem(UNIQUEITEM_STARTER_BUCKLER), SLOT_OFFHAND);

			FMeshTable* RowMesh = meshTable->FindRow<FMeshTable>(FName(*(FString::FromInt(UNIQUEITEM_STARTER_BUCKLER))), TEXT(""));
			if (RowMesh)
			{
				sWeapon = RowMesh->MeshString + "_0";
			}
			sPath = "/Game/Data/Art/Meshes/UTI/Weapon/Shield/Small/";
			avatar->LoadedStaticMeshesDatabase->StaticMeshList[MESH_LEFT].MeshResource = FStringAssetReference(sPath + sWeapon + "." + sWeapon);
			avatar->ChangeMeshLFT();
			avatar->MeshComponentStaticLFT->AttachToComponent(avatar->MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnShieldOff);
		}
		else
			avatar->MeshComponentStaticRGT->AttachToComponent(avatar->MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnWeaponRGTOn);
	}
	else if (itemWeapon->uniqueId == UNIQUEITEM_STARTER_STAFF)
	{
		sPath = "/Game/Data/Art/Meshes/UTI/Weapon/Ranged/Staff/";
		avatar->LoadedStaticMeshesDatabase->StaticMeshList[MESH_RIGHT].MeshResource = FStringAssetReference(sPath + sWeapon + "." + sWeapon);
		avatar->ChangeMeshRGT();
		avatar->MeshComponentStaticRGT->AttachToComponent(avatar->MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnStaffOff);
	}
	else if (itemWeapon->uniqueId == UNIQUEITEM_STARTER_DAGGER)
	{
		sPath = "/Game/Data/Art/Meshes/UTI/Weapon/Melee/Dagger/";
		avatar->LoadedStaticMeshesDatabase->StaticMeshList[MESH_RIGHT].MeshResource = FStringAssetReference(sPath + sWeapon + "." + sWeapon);
		avatar->ChangeMeshRGT();
		avatar->MeshComponentStaticRGT->AttachToComponent(avatar->MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnDaggerRGTOff);

		ForceEquip(GetUniqueItem(UNIQUEITEM_STARTER_DAGGER), SLOT_OFFHAND);
		avatar->LoadedStaticMeshesDatabase->StaticMeshList[MESH_LEFT].MeshResource = FStringAssetReference(sPath + sWeapon + "." + sWeapon);
		avatar->ChangeMeshLFT();
		avatar->MeshComponentStaticLFT->AttachToComponent(avatar->MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnDaggerLFTOff);
	}
	else if (itemWeapon->uniqueId == UNIQUEITEM_STARTER_BOW)
	{
		sPath = "/Game/Data/Art/Meshes/UTI/Weapon/Ranged/Longbow/";
		avatar->LoadedStaticMeshesDatabase->StaticMeshList[MESH_RIGHT].MeshResource = FStringAssetReference(sPath + sWeapon + "." + sWeapon);
		avatar->ChangeMeshRGT();
		avatar->MeshComponentStaticRGT->AttachToComponent(avatar->MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnLongbowOff);

		FMeshTable* RowMesh = meshTable->FindRow<FMeshTable>(FName(*(FString::FromInt(UNIQUEITEM_STARTER_QUIVER))), TEXT(""));
		if (RowMesh)
		{
			sWeapon = RowMesh->MeshString + "_0";
		}
		sPath = "/Game/Data/Art/Meshes/UTI/Weapon/Ranged/Quiver/";
		avatar->LoadedStaticMeshesDatabase->StaticMeshList[MESH_LEFT].MeshResource = FStringAssetReference(sPath + sWeapon + "." + sWeapon);
		avatar->ChangeMeshLFT();
		avatar->MeshComponentStaticLFT->AttachToComponent(avatar->MeshComponentSkeletalARM, FAttachmentTransformRules::SnapToTargetIncludingScale, fnQuiver);
	}
}

void UGameCharacter::TakeDamage(int32 nAmount, UGameCharacter* charAttacker /*= nullptr*/ /*, AHex* arg3 = nullptr*/)
{
	if (charAttacker && charAttacker->avatar)
		GetMain()->Battlefield->SetAvatarRotation(avatar, charAttacker->avatar->hex);

	int32 nChance = 0;
	int32 nRoll = 0;
	auto loc6 = nullptr;
	int32 item = ReduceHealth(nAmount);
	if (item > 0)
	{
		for (int32 i = 0; i < effects.Num(); i++)
		{
			UEffect* loc2 = effects[i];
			if (loc2->disorient)
			{
				RemoveEffectWithId(loc2->effectId);
			}
		}

		bool bCritical = false;
		if (charAttacker != nullptr)
		{
			if (ai != nullptr)
			{
				ai->TookDamageFrom(item, charAttacker);
			}
			if (charAttacker->scoredCritical)
			{
				bCritical = true;
			}
		}

		if (avatar != nullptr)
		{
			avatar->AddFloatingDamage(item, bCritical);

			if (health > 0)
			{
				avatar->AnimateAvatar(ANIMATION_INJURY);
				if (GetRace() == RACE_HURLOCK && classId != CLASS_HURLOCK_EMISSARY && !IsBerserk())
				{
					nChance = int(item / GetHealthMax() * 100);
					nRoll = FMath::RandRange(0, 99);
					UE_LOG(LogTemp, Warning, TEXT("doing berserk check: chance = %i, roll = %i"), nChance, nRoll);
					if (nRoll < nChance)
					{
						UEffect* effect = NewObject<UEffect>();
						effect->InitializeEffect(EFFECT_HURLOCK_BERSERK, this, this);
						AddEffect(effect);
						avatar->AddFloatingText("Berserk!", 16711680);
					}
				}
			}
			else
			{
				//TODO telemetry
				/*if (party)
				{
					loc6 = packages.battle.Battlefield.current == null ? "unknown" : packages.battle.Battlefield.current.enemyGroup.id;
					loc7 = new packages.metrics.MCharacterDeath(this, loc6);
					packages.metrics.Metrics.PTR.submit_telemetry(loc7);
				}*/

				avatar->AnimateAvatar(ANIMATION_DEATH);

				if (avatar->hex != nullptr)
				{
					avatar->deathHex = avatar->hex;
					avatar->hex->DetachAvatar();
					avatar->ClearLargeHexes();
				}

				UntoggleSkills();
				ClearStatusEffects();
			}
		}
		//TODO portrait dead
		/*if (portrait != nullptr)
		{
			portrait.updateHealth();
			portrait.showAlertDamage();
			if (health <= 0)
			{
				portrait.dead();
			}
		}*/
	}
}

void UGameCharacter::SetMonsterLevel(int32 nLevel)
{
	if (!IsMonster())
	{
		return;
	}

	level = nLevel;

	health = FMath::Min(GetHealthMax(), health);
}

void UGameCharacter::ClearStatusEffects()
{
	bool bResult = false;
	int32 nIndex = 0;

	while (nIndex < effects.Num())
	{
		UEffect* effect = effects[nIndex];

		bResult = RemoveEffectWithIndex(nIndex);
		if (!bResult)
		{
			++nIndex;
		}
	}

	effects.Empty();
}

void UGameCharacter::ClearNegativeEffects()
{
	int32 nIndex = effects.Num() - 1;
	while (nIndex >= 0)
	{
		UEffect* effect = effects[nIndex];
		if (effect->beneficial == false)
		{
			RemoveEffectWithId(effect->effectId);
		}
		--nIndex;
	}
}

void UGameCharacter::UntoggleSkills()
{
	for (int i = 0; i < GetSkills().Num(); i++)
	{
		USkill* skill = GetSkills()[i];

		if (GetMain()->Battlefield)
		{
			if (skill->IsUnlocked() && skill->type == SKILL_TYPE_TOGGLED && skill->toggledOn)
			{
				GetMain()->Battlefield->ToggleOffSkill(skill, false, false);
			}
		}

		if (skill->type == SKILL_TYPE_TOGGLED)
		{
			skill->toggledOn = false;
		}
	}
}

void UGameCharacter::RefreshSkills()
{
	_skills = GetUnlockedTalents();
}

void UGameCharacter::BoundHealth()
{
	if (health <= 0)
	{
		health = 0;
		ClearStatusEffects();
	}
	if (health > GetHealthMax())
	{
		health = GetHealthMax();
	}
}

void UGameCharacter::BoundPower()
{
	if (power < 0)
	{
		power = 0;
	}
	if (power > GetPowerMax())
	{
		power = GetPowerMax();
	}
	return;
}

//one tap
void UGameCharacter::MouseOver(FVector vLocation, bool bTooltip /* = true*/)
{
	//TODO MouseOver
}

int32 UGameCharacter::GetBonusDamageToDarkspawn()
{
	int32 nBonus = 0;

	if (this == GetParty()->GetCompanionByClass(CLASS_CHAMPION, false))
	{
		nBonus = FMath::FloorToInt(3 + level * 0.5);
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];

		if (item != nullptr)
		{
			nBonus = nBonus + item->bonusDamageToDarkspawn;
		}
	}

	return nBonus;
}

int32 UGameCharacter::GetCriticalRate()
{
	int32 nRate = 1;

	if (IsMonster())
	{
		if (level <= 2)
		{
			return -1;
		}

		if (GetParty()->GetDifficulty() == DIFFICULTY_EASY)
		{
			return -1;
		}

		return FMath::FloorToInt(1 + level / 5);
	}

	if (IsMage())
	{
		return -1;

	}

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];

		nRate = nRate + effect->criticalChanceModifier;

		if (effect->cancelCriticals)
		{
			return -1;
		}
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item)
		{
			nRate = nRate + item->criticalChance;
		}
	}

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* skill = passiveSkills[i];
		if (skill->IsUnlocked())
		{
			nRate = nRate + skill->passiveCriticalChanceIncrease;
		}
	}

	return nRate;
}

int32 UGameCharacter::GetCriticalDamageMultiplier()
{
	int32 nMultiplier = 1;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* loc2 = effects[i];

		nMultiplier = nMultiplier + loc2->criticalDamageMultiplier;
	}

	return nMultiplier;
}

int32 UGameCharacter::GetIncomingDamageMultiplier()
{
	int32 nMultiplier = 1;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* loc2 = effects[i];

		nMultiplier = nMultiplier + loc2->incomingDamageMultiplier;
	}

	return nMultiplier;
}

int32 UGameCharacter::GetIncomingHealingMultiplier()
{
	int32 nMultiplier = 1;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* loc2 = effects[i];

		nMultiplier = nMultiplier + loc2->incomingHealingMultiplier;
	}

	return nMultiplier;
}

int32 UGameCharacter::GetInitiative() const
{
	int32 nInitiative = base_initiative;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect && effect->initiativeModifier != 0)
		{
			nInitiative = nInitiative + effect->initiativeModifier;
		}
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* gearItem = gear[i];
		if (gearItem && gearItem->mod_quickness)
		{
			nInitiative = nInitiative + gearItem->mod_quickness;
		}
	}

	return nInitiative;
}

int32 UGameCharacter::GetWillpower()
{
	int32 nWillpower = _willpower;

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* gearItem = gear[i];
		if (gearItem)
		{
			nWillpower = nWillpower + gearItem->mod_willpower;
		}
	}

	return nWillpower;
}

int32 UGameCharacter::GetMagic()
{
	int32 nMagic = _magic;

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* gearItem = gear[i];
		if (gearItem)
		{
			nMagic = nMagic + gearItem->mod_magic;
		}
	}

	return nMagic;
}

int32 UGameCharacter::GetCunning()
{
	int32 nCunning = _cunning;

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* gearItem = gear[i];
		if (gearItem)
		{
			nCunning = nCunning + gearItem->mod_cunning;
		}
	}

	return nCunning;
}

int32 UGameCharacter::GetStrength()
{
	int32 nStrength = _strength;

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* gearItem = gear[i];
		if (gearItem)
		{
			nStrength = nStrength + gearItem->mod_strength;
		}
	}

	return nStrength;
}

int32 UGameCharacter::GetConstitution()
{
	int32 nConstitution = _constitution;

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* gearItem = gear[i];
		if (gearItem)
		{
			nConstitution = nConstitution + gearItem->mod_constitution;
		}
	}

	return nConstitution;
}

int32 UGameCharacter::GetDexterity()
{
	int32 nDexterity = _dexterity;

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* gearItem = gear[i];
		if (gearItem)
		{
			nDexterity = nDexterity + gearItem->mod_dexterity;
		}
	}

	return nDexterity;
}

int32 UGameCharacter::GetBaseWillpower()
{
	return _willpower;
}

int32 UGameCharacter::GetBaseMagic()
{
	return _magic;
}

int32 UGameCharacter::GetBaseCunning()
{
	return _cunning;
}

int32 UGameCharacter::GetBaseStrength()
{
	return _strength;
}

int32 UGameCharacter::GetBaseConstitution()
{
	return _constitution;
}

int32 UGameCharacter::GetBaseDexterity()
{
	return _dexterity;
}

void UGameCharacter::SetBaseWillpower(int32 nValue)
{
	_willpower = nValue;
}

void UGameCharacter::SetBaseMagic(int32 nValue)
{
	_magic = nValue;
}

void UGameCharacter::SetBaseCunning(int32 nValue)
{
	_cunning = nValue;
}

void UGameCharacter::SetBaseStrength(int32 nValue)
{
	_strength = nValue;
}

void UGameCharacter::SetBaseConstitution(int32 nValue)
{
	_constitution = nValue;
}

void UGameCharacter::SetBaseDexterity(int32 nValue)
{
	_dexterity = nValue;
}

int32 UGameCharacter::GetSpeed()
{
	int32 nSpeed = _speed;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect && effect->movementModifier != 0)
		{
			nSpeed = nSpeed + effect->movementModifier;
		}
	}

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* loc3 = passiveSkills[i];
		if (loc3->IsUnlocked())
		{
			nSpeed = nSpeed + loc3->passiveMovementIncrease;
		}
	}

	return FMath::Max(0, nSpeed);
}

int32 UGameCharacter::GetAttack()
{
	int32 nAttack = FMath::RoundToInt(BaseAttackFor(classId) + GetDexterity() * 0.5 + GetStrength() * 0.5);

	if (IsMonster())
	{
		nAttack = nAttack + (level - 1) * 2;
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item != nullptr)
		{
			nAttack = nAttack + item->attack;
		}
	}

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		nAttack = nAttack + effect->attackModifier;
	}

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* skill = passiveSkills[i];
		if (skill->IsUnlocked())
		{
			nAttack = nAttack + skill->passiveAttackValueBonus;
		}
	}

	return FMath::Max(0, nAttack);
}

int32 UGameCharacter::GetDefense()
{
	int32 nDefense = BaseDefenseFor(classId) + GetDexterity();

	if (IsMonster())
	{
		nDefense = nDefense + (level - 1) * 2;
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item != nullptr)
		{
			nDefense = nDefense + item->defense;
		}
	}

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		nDefense = nDefense + effect->defenseModifier;
	}

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* skill = passiveSkills[i];
		if (skill->IsUnlocked())
		{
			nDefense = nDefense + skill->passiveDefenseValueBonus;
		}
	}

	return FMath::Max(0, nDefense);
}

int32 UGameCharacter::GetDefenseMissile()
{
	int32 nDefenseMissile = GetDefense();

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item != nullptr)
		{
			nDefenseMissile = nDefenseMissile + item->defenseMissile;
		}
	}

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		nDefenseMissile = nDefenseMissile + effect->defenseMissileModifier;
	}

	return FMath::Max(0, nDefenseMissile);
}

int32 UGameCharacter::GetArmor()
{
	int32 nArmor = BaseArmorFor(classId);

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item)
		{
			nArmor = nArmor + item->armor;
		}
	}

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect)
		{
			nArmor = nArmor + effect->armorModifier;
		}
	}

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* skill = passiveSkills[i];
		if (skill->IsUnlocked())
		{
			nArmor = nArmor + skill->passiveArmorValueBonus;
		}
	}

	return FMath::Max(nArmor, 0);
}

int32 UGameCharacter::GetPenetration()
{
	int32 nPenetration = BaseArmorFor(classId);

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item)
		{
			nPenetration = nPenetration + item->piercing;
		}
	}

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect)
		{
			nPenetration = nPenetration + effect->armorPenetrationModifier;
		}
	}

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* skill = passiveSkills[i];
		if (skill->IsUnlocked())
		{
			nPenetration = nPenetration + skill->armorPenetrationBonus;
		}
	}

	return FMath::Max(nPenetration, 0);//DHK TODO 0 originally
}

int32 UGameCharacter::GetDecisiveStrikePercentage()
{
	int32 nStrike = BaseArmorFor(classId);

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item)
		{
			nStrike = nStrike + item->decisiveStrikePercentage;
		}
	}

	return nStrike;
}

int32 UGameCharacter::GetHealthMax()
{
	USkill* skill = nullptr;
	int32 baseHealth = 75;
	if (IsMonster())
	{
		baseHealth = (MONSTER_BASE_HEALTH + (level - 1) * 5) * monsterHealthMultiplier;
		baseHealth = ModifyMonsterHealth(baseHealth);
	}
	else
	{
		baseHealth = baseHealth + GetConstitution() * 5;
		for (int32 i = 0; i < passiveSkills.Num(); i++)
		{
			skill = passiveSkills[i];
			if (skill->passiveHealthMultiplier != 1 && skill->IsUnlocked())
			{
				baseHealth = baseHealth * skill->passiveHealthMultiplier;
			}
		}
	}

	baseHealth = baseHealth < 0 ? 0 : baseHealth;

	return FMath::CeilToInt(baseHealth);
}

int32 UGameCharacter::GetPowerMax()
{
	int32 basePower = 75;
	basePower = basePower + GetWillpower() * 5;

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* equipment = gear[i];

		if (equipment == nullptr)
		{
			continue;
		}

		if (equipment->stamina > 0 && !UsesMana(classId))
		{
			basePower = basePower + equipment->stamina;
			continue;
		}

		if (equipment->mana > 0 && UsesMana(classId))
		{
			basePower = basePower + equipment->mana;
			continue;
		}
	}

	basePower = basePower < 0 ? 0 : basePower;

	return basePower;
}

int32 UGameCharacter::GetSpellpower()
{
	int32 spellpower = level + GetMagic();

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* skill = passiveSkills[i];
		if (skill->IsUnlocked())
		{
			spellpower = spellpower + skill->passiveSpellpowerBonus;
		}
	}

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect->effectId == EFFECT_DEATH_MAGIC
			&& GetMain()->Battlefield)
		{
			spellpower = spellpower + 2 * GetMain()->Battlefield->CountCorpses();

		}
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* gearItem = gear[i];
		if (gearItem)
		{
			spellpower = spellpower + gearItem->spellpower;
		}
	}

	return spellpower;
}

int32 UGameCharacter::GetStealthStrength()
{
	int32 nBaseStealthStrength = 50;
	int32 nUnlocked = 0;

	for (int32 i = 0; i < talents.Num(); i++)
	{
		UTalentLine* talentLine = talents[i];
		if (talentLine->treeId == TALENT_ROGUE_STEALTH)
		{
			nUnlocked = talentLine->unlocked;
			break;
		}
	}

	nBaseStealthStrength = nBaseStealthStrength + 15 * (nUnlocked - 1);

	return nBaseStealthStrength;
}

int32 UGameCharacter::GetPerceptionRoll()
{
	return FMath::RandRange(0, 100);
}

int32 UGameCharacter::GetPhysicalResistance()
{
	if (IsMonster())
	{
		return (level - 1) * 3;
	}

	int32 nPhysicalResistance = (GetStrength() + GetConstitution() + GetDexterity()) * 0.3;

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* skill = passiveSkills[i];
		if (skill->IsUnlocked())
		{
			nPhysicalResistance = nPhysicalResistance + skill->passivePhysicalResistance;
		}
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item != nullptr)
		{
			nPhysicalResistance = nPhysicalResistance + item->resistPhysical;
		}
	}

	return nPhysicalResistance;
}

int32 UGameCharacter::GetMentalResistance()
{
	if (IsMonster())
	{
		return (level - 1) * 2;
	}

	int32 nMentalResistance = (GetMagic() + GetCunning() + GetWillpower()) * 0.3;

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* skill = passiveSkills[i];
		if (skill->IsUnlocked())
		{
			nMentalResistance = nMentalResistance + skill->passiveMentalResistance;
		}
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item != nullptr)
		{
			nMentalResistance = nMentalResistance + item->resistMental;
		}
	}

	return nMentalResistance;
}

int32 UGameCharacter::GetStunResistance()
{
	int32 nStunResistance = 0;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		nStunResistance = nStunResistance + effect->resistStun;
	}

	return nStunResistance;
}

int32 UGameCharacter::GetSlowResistance()
{
	int32 nSlowResistance = 0;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		nSlowResistance = nSlowResistance + effect->resistSlow;
	}

	return nSlowResistance;
}

int32 UGameCharacter::GetHealthRegeneration()
{
	int32 nHealthRegeneration = 0;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect->healthRegen > 0)
		{
			nHealthRegeneration = nHealthRegeneration + effect->healthRegen;
		}

		if (effect->effectId == EFFECT_DEATH_MAGIC && GetMain()->Battlefield)
		{
			nHealthRegeneration = nHealthRegeneration + FMath::FloorToInt(5 * GetMain()->Battlefield->CountCorpses() * GetHealthMax() / 100);
		}
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item && item->healthRegen)
		{
			nHealthRegeneration = nHealthRegeneration + item->healthRegen;
		}
	}

	return nHealthRegeneration;
}

int32 UGameCharacter::GetPowerRegeneration()
{
	int32 nPowerRegeneration = 10;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect->cancelPowerRegen)
		{
			return 0;
		}
		if (effect->powerRegen > 0)
		{
			nPowerRegeneration = nPowerRegeneration + effect->powerRegen;
		}
	}

	for (int32 i = 0; i < gear.Num(); i++)
	{
		UEquipment* item = gear[i];
		if (item && item->powerRegen)
		{
			nPowerRegeneration = nPowerRegeneration + item->powerRegen;
		}
	}

	return nPowerRegeneration;
}

int32 UGameCharacter::GetAttackCostModifier()
{
	int32 nAttackCostModifier = 0;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect)
		{
			nAttackCostModifier = nAttackCostModifier + effect->attackPowerCostModifier;
		}
	}

	return nAttackCostModifier;
}

int32 UGameCharacter::GetSpellpowerForSkill(USkill* skill)
{
	int32 nSpellpower = GetSpellpower();
	if (skill && skill->talentLine)
	{
		nSpellpower = nSpellpower + GetSpellpowerBonusForSchool(skill->talentLine->type);
	}

	return nSpellpower;
}

int32 UGameCharacter::GetRace()
{
	return _race;
}

void UGameCharacter::SetRace(int32 nValue)
{
	_race = nValue;
}

void UGameCharacter::InitSkills()
{
	for (int32 i = 0; i < talents.Num(); i++)
	{
		UTalentLine* talentLine = talents[i];

		int32 nCounter = 0;
		while (nCounter < talentLine->skillIds.Num())
		{
			if (talentLine->skillIds[nCounter] != SKILL_DUMMY_SKILL)
			{
				USkill* skill = NewObject<USkill>();
				skill->InitializeSkill(talentLine->skillIds[nCounter], this);

				skill->talentLine = talentLine;
				_allSkills.Add(skill);
				if (skill->type == SKILL_TYPE_PASSIVE)
				{
					passiveSkills.Add(skill);
				}
			}
			++nCounter;
		}
	}
}

bool UGameCharacter::IsMage()
{
	return GetBaseClassOf(classId) == CLASS_MAGE;
}

bool UGameCharacter::IsWarrior()
{
	return GetBaseClassOf(classId) == CLASS_WARRIOR;
}

bool UGameCharacter::IsRogue()
{
	return GetBaseClassOf(classId) == CLASS_ROGUE;
}

//TODO more option templates for monsters
bool UGameCharacter::IsMonster()
{
	if (classId >= 100)
	{
		return true;
	}
	return false;
}

bool UGameCharacter::IsHidden()
{
	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect->hidden)
		{
			return true;
		}
	}

	return false;
}

bool UGameCharacter::IsInactive()
{
	if (health <= 0)
	{
		return true;
	}

	if (retreated)
	{
		return true;
	}

	return false;
}

bool UGameCharacter::IsLargeCreature()
{
	if (creatureWidth > 1 || creatureHeight > 1)
	{
		return true;
	}

	return false;
}

bool UGameCharacter::IsBerserk()
{
	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect->effectId == EFFECT_HURLOCK_BERSERK)
		{
			return true;
		}
	}

	return false;
}

bool UGameCharacter::IsStunned()
{
	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect->stun)
		{
			return true;
		}
	}

	return false;
}

bool UGameCharacter::IsDisoriented()
{
	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect->disorient)
		{
			return true;
		}
	}

	return false;
}

bool UGameCharacter::IsUsingHealthForPower()
{
	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect->skillsCostHealthInsteadOfPower)
		{
			return true;
		}
	}

	return false;
}

bool UGameCharacter::IsImmuneToFlanking()
{
	if (HasShield())
	{
		USkill* skill = GetSkill(SKILL_SHIELD_BLOCK);
		if (skill != nullptr && skill->IsUnlocked())
		{
			return true;
		}
	}
	return false;
}

bool UGameCharacter::HasShield()
{
	UEquipment* item = gear[SLOT_OFFHAND];
	if (item == nullptr)
	{
		return false;
	}

	if (item->IsShield())
	{
		return true;
	}

	return false;
}

bool UGameCharacter::CanCounterattack()
{
	if (GetBaseClassOf(classId) == CLASS_MAGE)
	{
		return false;
	}

	if (isRunt)
	{
		return false;
	}

	return true;
}

void UGameCharacter::SetCounterAttackReady(bool bReady)
{
	if (bReady)
	{
		UEffect* effect = NewObject<UEffect>();
		effect->InitializeEffect(EFFECT_COUNTER_ATTACK, this, this);
		AddEffect(effect);
	}
	else
	{
		RemoveEffectWithId(EFFECT_COUNTER_ATTACK);
	}
}

bool UGameCharacter::GetCounterAttackReady()
{
	UE_LOG(LogTemp, Warning, TEXT("get counterAttackReady: test current effects on %s"), *charName);

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		UE_LOG(LogTemp, Warning, TEXT("effectId: %i, name: %s"), effect->effectId, *GetEffectName(effect->effectId));

		if (effect->effectId == EFFECT_COUNTER_ATTACK)
		{
			UE_LOG(LogTemp, Warning, TEXT("counter attack effect found"));
			return true;
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("no counter attack effect found"));

	return false;
}

bool UGameCharacter::AddEffect(UEffect* effect)
{
	UE_LOG(LogTemp, Warning, TEXT("%s AddEffect(%s) - duration %i"), *charName, *GetEffectName(effect->effectId), effect->duration);
	int32 nColor = 13547636;//CEB874
	int32 nRoll = 0;

	if (!effect->beneficial)
	{
		if (effect->stun)
		{
			nRoll = FMath::RandRange(1, 100);
			if (nRoll <= GetStunResistance())
			{
				avatar->AddFloatingText("Resist Stun", nColor);
				return false;
			}
		}

		if (effect->movementModifier < 0)
		{
			nRoll = FMath::RandRange(1, 100);
			if (nRoll <= GetSlowResistance())
			{
				avatar->AddFloatingText("Resist Slow", nColor);
				return false;
			}
		}

		if (effect->source == EFFECT_PHYSICAL)
		{
			nRoll = FMath::RandRange(1, 100);
			if (nRoll <= GetPhysicalResistance())
			{
				avatar->AddFloatingText("Resist Physical", nColor);
				return false;
			}
		}

		if (effect->source == EFFECT_MENTAL)
		{
			nRoll = FMath::RandRange(1, 100);
			if (nRoll <= GetMentalResistance())
			{
				avatar->AddFloatingText("Resist Mental", nColor);
				return false;
			}
		}
	}

	if (effect->type == EFFECT_TYPE_INSTANT)
	{
		effect->ApplyTo(this);
		return true;
	}

	int32 loc3 = 0;
	while (loc3 < effects.Num())
	{
		if (effects[loc3]->effectId/*name*/ == effect->effectId/*name*/)
		{
			if (effects[loc3]->level == effect->level)
			{
				effects[loc3] = effect;
				return true;
			}
			if (effects[loc3]->level < effect->level)
			{
				effects[loc3] = effect;
				return true;
			}
			return false;
		}
		++loc3;
	}

	effects.Add(effect);

	//TODO effect de/buff anim
	//auto loc4 = nullptr;
	/*if (avatar != nullptr && GetMain()->Battlefield)
	{
		switch (effect->effectId)
		{
		case EFFECT_COUNTER_ATTACK:
		case EFFECT_FLAMING_WEAPON:
		{
			break;
		}
		case EFFECT_RALLY:
		{
			if (HasEffectWithId(EFFECT_RALLY) == false)
			{
				GetMain()->Battlefield->AddAnimationToAvatar(new CasterAnim(new generic_buff()), avatar, nullptr);
			}
			break;
		}
		default:
		{
			if (effect->beneficial)
			{
				loc4 = new CasterAnim(new generic_buff());
			}
			else
			{
				loc4 = new CasterAnim(new generic_debuff());
			}
			GetMain()->Battlefield->AddAnimationToAvatar(loc4, avatar, nullptr);
			break;
		}
		}
	}*/

	if (avatar)
	{
		avatar->EffectAdded(effect);
	}

	return true;
}

bool UGameCharacter::RemoveEffectWithId(int32 nEffectId)
{
	int32 nLastEffect = effects.Num() - 1;

	while (nLastEffect >= 0)
	{
		UEffect* loc2 = effects[nLastEffect];

		if (loc2->effectId == nEffectId)
		{
			return RemoveEffectWithIndex(nLastEffect);
		}

		--nLastEffect;
	}

	return false;
}

bool UGameCharacter::RemoveEffectWithIndex(int32 nIndex)
{
	if (effects[nIndex] == nullptr)
	{
		return false;
	}

	UEffect* effect = effects[nIndex];
	effect->active = false;

	UE_LOG(LogTemp, Warning, TEXT("%s RemoveEffect %s"), *charName, *GetEffectName(nIndex));
	effects.RemoveAt(nIndex, 1);

	if (avatar != nullptr)
	{
		avatar->EffectRemoved(effect);
	}

	return true;
}

float UGameCharacter::GetBattleInitiative() const
{
	return GetInitiative() + initiativeRoll;
}

int32 UGameCharacter::IncreaseHealth(int32 nAmount, bool bResurrects /*= false*/)
{
	int32 baseHealth = health;
	if (baseHealth <= 0 && !bResurrects)
	{
		return 0;
	}

	health = health + nAmount;

	BoundHealth();

	if (health == 0)
	{
		ClearStatusEffects();
	}

	if (bResurrects && baseHealth <= 0 && health > 0)
	{
		ClearStatusEffects();
	}

	return health - baseHealth;
}

int32 UGameCharacter::ReduceHealth(int32 nAmount)
{
	int32 nBaseHealth = health;
	health = health - nAmount;

	BoundHealth();

	if (immortal)
	{
		health = FMath::Max(1, health);
	}

	if (GetHealthPercent() < LOW_HEALTH_PERCENT && GetHealthPercent() != 0 && GetParty()->members.Contains(this))
	{
		TriggerTutorial(TUT_LOW_HEALTH);
	}

	return nBaseHealth - health;
}

int32 UGameCharacter::IncreasePower(int32 nAmount)
{
	old_power = power;
	power = power + nAmount;
	BoundPower();
	return power - old_power;
}

int32 UGameCharacter::ReducePower(int32 nAmount)
{
	old_power = power;
	power = power - nAmount;
	BoundPower();
	return old_power - power;

}

int32 UGameCharacter::GetHealthPercent()
{
	return (health > 0) ? FMath::RoundToInt(100 / (GetHealthMax() / health)) : 0;
}

int32 UGameCharacter::GetPowerPercent()
{
	return (power > 0) ? FMath::RoundToInt(100 / (GetPowerMax() / power)) : 0;
}

int32 UGameCharacter::GetWeaponDamage()
{
	float loc5 = 0;
	UEquipment* item = gear[SLOT_WEAPON];
	float loc2 = item->damage;

	if (IsMonster())
	{
		return loc2;
	}

	auto loc3 = 0;
	auto loc4 = 0;

	if (item->IsMelee())
	{
		loc3 = 1;
		if (item->typeId == ITEMTYPE_DAGGER_KNIFE ||
			item->typeId == ITEMTYPE_DAGGER_KRIS)
		{
			loc4 = 0.5;
		}
		else
		{
			loc4 = 0;
		}

		if (gear[SLOT_OFFHAND] != nullptr)
		{
			loc2 = loc2 + GetOffhandDamage();
		}
	}
	else if (item->IsRanged())
	{
		loc4 = 0.5;
		loc3 = 0.5;
	}
	else if (item->IsStaff())
	{
		loc5 = 0.35;
		loc2 = loc2 + GetSpellpower() * loc5;
	}

	loc2 = loc2 + GetStrength() * loc3;
	loc2 = loc2 + GetDexterity() * loc4;

	return FMath::RoundToInt(loc2);
}

int32 UGameCharacter::GetOffhandDamage()
{
	float fDamage = gear[SLOT_OFFHAND]->damage;

	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* loc2 = effects[i];
		if (loc2 && loc2->giveOffhandFullDamage)
		{
			return fDamage;
		}
	}

	return FMath::RoundToInt(fDamage * 0.5);
}

int32 UGameCharacter::GetMonsterBaseDamage()
{
	float fMultiplier = monsterDamageMultiplier + 0.1 * (level - 1);
	float fDamage = MONSTER_BASE_DAMAGE * fMultiplier;

	return FMath::RoundToInt(fDamage);
}

int32 UGameCharacter::GetDamageMultiplier()
{
	float nMultiplier = 1;

	for (int32 i = 0; i < passiveSkills.Num(); i++)
	{
		USkill* skill = passiveSkills[i];
		if (skill->IsUnlocked() && skill->passiveDamageMultiplier != 1)
		{
			nMultiplier = nMultiplier * skill->passiveDamageMultiplier;
		}
	}

	return nMultiplier;
}

int32 UGameCharacter::GetMovesLeft()
{
	return FMath::Max(0, GetSpeed() - moved);
}

int32 UGameCharacter::GetHealingBonus(bool bBool /*= true*/)
{
	//TODO implement GetHealingBonus, not in original
	return 0;
}

bool UGameCharacter::HasSkill(int32 nSkillId, bool bPassive /*= false*/)
{
	if (GetSkill(nSkillId, bPassive))
	{
		return true;
	}
	return false;
}

bool UGameCharacter::HasEffectWithId(int32 nEffectId)
{
	for (int32 i = 0; i < effects.Num(); i++)
	{
		UEffect* effect = effects[i];
		if (effect->effectId == nEffectId)
			return true;
	}

	return false;
}

bool UGameCharacter::Unequip(UEquipment* item)
{
	if (party == nullptr || item == nullptr)
	{
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Character.unequip(%s) - slot = "), *item->itemName, item->slot);

	if (item->slot == SLOT_WEAPON && gear[SLOT_OFFHAND] == item)
	{
		item->slot = SLOT_OFFHAND;
	}
	else if (item->slot == SLOT_RING1 && gear[SLOT_RING2] == item)
	{
		item->slot = SLOT_RING2;
	}
	else if (item != gear[item->slot])
	{
		UE_LOG(LogTemp, Warning, TEXT("mismatch! item is not equipped!"));

		if (gear[item->slot] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("item equipped in that slot is "), *gear[item->slot]->itemName);
		}

		return false;
	}

	if (GetParty()->AddToInventory(gear[item->slot]))
	{
		gear[item->slot] = nullptr;
		item->playerOwner = nullptr;
		DefaultEquipment();
		return true;
	}

	return false;
}

USkill* UGameCharacter::GetDefaultAttack()
{
	if (_defaultAttack == nullptr)
	{
		if (IsMonster())
		{
			_defaultAttack = GetSkills()[0];
		}
		else if (gear[SLOT_WEAPON]->IsStaff())
		{
			_defaultAttack = NewObject<USkill>();
			_defaultAttack->InitializeSkill(SKILL_MAGE_BOLT, this);
		}
		else if (gear[SLOT_WEAPON]->IsRanged())
		{
			_defaultAttack = NewObject<USkill>();
			_defaultAttack->InitializeSkill(SKILL_BOW_FIRE, this);
		}
		else
		{
			_defaultAttack = NewObject<USkill>();
			_defaultAttack->InitializeSkill(SKILL_STRIKE, this);
		}

		_defaultAttack->shortcutSlot = 1;
	}

	return _defaultAttack;
}

USkill* UGameCharacter::GetSkill(int32 nSkillId, bool bPassive /*= false*/)
{
	USkill* skill = nullptr;

	if (bPassive)
	{
		for (int32 i = 0; i < passiveSkills.Num(); i++)
		{
			skill = passiveSkills[i];
			if (skill->skillId == nSkillId && skill->IsUnlocked())
			{
				return skill;
			}
		}
	}
	else
	{
		for (int32 i = 0; i < GetSkills().Num(); i++)
		{
			skill = GetSkills()[i];
			if (skill->skillId == nSkillId && skill->IsUnlocked())
			{
				return skill;
			}
		}
	}

	return nullptr;
}

USkill* UGameCharacter::FindSkillWithId(int32 nSkillId)
{
	for (int32 i = 0; i < GetSkills().Num(); i++)
	{
		USkill* skill = GetSkills()[i];
		if (skill->skillId != nSkillId)
		{
			return skill;
		}
	}

	return nullptr;
}

TArray<USkill*> UGameCharacter::GetSkills()
{
	if (_skills.Num() == 0)
	{
		_skills = GetUnlockedTalents();
	}
	return _skills;
}

UGameCharacter::~UGameCharacter()
{
}

int32 UGameCharacter::GetSpellpowerBonusForSchool(int32 nType)
{
	UEquipment* item = nullptr;
	int32 nBonus = 0;

	if (nType == TALENT_TYPE_ENTROPY)
	{
		for (int32 i = 0; i < gear.Num(); i++)
		{
			item = gear[i];
			if (item != nullptr)
			{
				nBonus = nBonus + item->spellpower_entropy;
			}
		}
	}

	if (nType == TALENT_TYPE_CREATION)
	{
		for (int32 i = 0; i < gear.Num(); i++)
		{
			item = gear[i];
			if (item != nullptr)
			{
				nBonus = nBonus + item->spellpower_creation;
			}
		}
	}

	if (nType == TALENT_TYPE_SPIRIT)
	{
		for (int32 i = 0; i < gear.Num(); i++)
		{
			item = gear[i];
			if (item != nullptr)
			{
				nBonus = nBonus + item->spellpower_spirit;
			}
		}
	}

	if (nType == TALENT_TYPE_PRIMAL)
	{
		for (int32 i = 0; i < gear.Num(); i++)
		{
			item = gear[i];
			if (item != nullptr)
			{
				nBonus = nBonus + item->spellpower_primal;
			}
		}
	}

	return nBonus;
}

TArray<USkill*> UGameCharacter::GetUnlockedTalents()
{
	TArray<USkill*> arSkills;

	for (int32 i = 0; i < _allSkills.Num(); i++)
	{
		USkill* _skill = _allSkills[i];

		if (IsMonster())
		{
			arSkills.Add(_skill);
		}

		if (_skill->IsUnlocked())
		{
			arSkills.Add(_skill);
		}
	}

	for (int32 i = 0; i < _tempSkills.Num(); i++)
	{
		USkill* item = _tempSkills[i];
		arSkills.Add(item);
	}

	//UE_LOG(LogTemp, Warning, TEXT("SKILL || character: %s has skills %i of %i "), *charName, arSkills.Num(), _allSkills.Num());

	return arSkills;
}
