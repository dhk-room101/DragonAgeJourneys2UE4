#include "Scripting.h"
#include "DragonAgeJourneysLibrary.h"
#include "DragonAgeJourneysSingleton.h"
#include "Effect.h"
#include "DragonAgeJourneysPlayerController.h"
#include "Engine/World.h"
#include "Race.h"
#include "Gender.h"

FName GetFileFromClassName(FName fName)
{
	for (int32 i = 0; i < GetAssets()->embed_list->asset_list.Num(); i++)
	{
		FEmbedListMember embed = GetAssets()->embed_list->asset_list[i];
		if (embed.classname == fName)
			return embed.file;
	}

#ifdef DEBUG
	UE_LOG(LogTemp, Error, TEXT("Asset %s not found!"), *fName.ToString());
#endif // DEBUG

	return "notFound";
}

void ShowBark(FString sBark, UGameCharacter * uGameChar)
{
	//TODO ShowBark
}

void TriggerConversation(FString sConversation, FString locId /*, Function fCallback*/)
{
	//TODO TriggerConversation
}

bool TriggerDialog(FString sDialog, bool bVisited)
{
	//TODO TriggerDialog
	return false;
}

bool TriggerTutorial(int32 nTutorial)
{
	//TODO TriggerTutorial
	return false;
}

UUserWidgetQuickBar* GetQuickBar()
{
	return GetCurrentPlayerController()->UIQuickBar;
}

UWorld* GetStage()
{
	bool IsValid;
	return UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysCurrentWorld;
}

UDragonMain * GetMain()
{
	bool IsValid;
	if (UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysMain == nullptr)
		UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysMain = NewObject<UDragonMain>();
	return UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysMain;
}

UParty * GetParty()
{
	bool IsValid;
	if (UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysParty == nullptr)
		UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysParty = NewObject<UParty>();
	return UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysParty;
}

UEmbeddedAssets* GetAssets()
{
	bool IsValid;
	if (UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->ASSETS == nullptr)
		UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->ASSETS = NewObject<UEmbeddedAssets>();
	return UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->ASSETS;
}

ADragonAgeJourneysPlayerController* GetCurrentPlayerController()
{
	return (Cast<ADragonAgeJourneysPlayerController>(GetStage()->GetFirstPlayerController()));
}

FVector GetBattleTraceImpact()
{
	return GetCurrentPlayerController()->BattleTraceImpact;
}

FString GetEffectName(int32 nEffectId)
{
	switch (nEffectId)
	{
	case EFFECT_FLAMING_WEAPON:
	{
		return "Flaming Weapon";
	}
	case EFFECT_FEIGN_DEATH:
	{
		return "Feign Death";
	}
	case EFFECT_STEALTH:
	{
		return "Stealth";
	}
	case EFFECT_DIRTY_FIGHTING:
	{
		return "Dirty Fighting";
	}
	case EFFECT_BELOW_THE_BELT:
	{
		return "Below the Belt";
	}
	case EFFECT_DUAL_DOUBLE_STRIKE:
	{
		return "Double Strike";
	}
	case EFFECT_DUAL_CRIPPLE:
	{
		return "Crippled";
	}
	case EFFECT_AIM:
	{
		return "Aim";
	}
	case EFFECT_DEFENSIVE_FIRE:
	{
		return "Defensive Fire";
	}
	case EFFECT_CRIPPLING_SHOT:
	{
		return "Crippling Shot";
	}
	case EFFECT_PINNING_SHOT:
	{
		return "Pinning Shot";
	}
	case EFFECT_SUPPRESSING_FIRE:
	{
		return "Suppressing Fire";
	}
	case EFFECT_DISTRACTION:
	{
		return "Distraction";
	}
	case EFFECT_SONG_VALOR:
	{
		return "Song of Valor";
	}
	case EFFECT_SONG_COURAGE:
	{
		return "Song of Courage";
	}
	case EFFECT_SONG_CAPTIVATE:
	{
		return "Captivating Song";
	}
	case EFFECT_CLERIC_HOLY_SHIELD:
	{
		return "Shield of Faith";
	}
	case EFFECT_CLERIC_ANOINT:
	{
		return "Anoint";
	}
	case EFFECT_CLERIC_FLASH:
	{
		return "Holy Light";
	}
	case EFFECT_CLERIC_PURIFY:
	{
		return "Purified";
	}
	case EFFECT_MAGE_ARMOR:
	{
		return "Arcane Armor";
	}
	case EFFECT_MAGE_INVISIBILITY:
	{
		return "Invisible";
	}
	case EFFECT_MAGE_PRISON:
	{
		return "Cosmic Prison";
	}
	case EFFECT_POISON_DRAKE:
	{
		return "Drake Poison";
	}
	case EFFECT_POISON_DRAKE_AE:
	{
		return "Venomous Cloud";
	}
	case EFFECT_OVERRUN:
	{
		return "Overrun";
	}
	case EFFECT_WAR_CRY:
	{
		return "War Cry";
	}
	case EFFECT_RALLY:
	{
		return "Rally";
	}
	case EFFECT_INDOMITABLE:
	{
		return "Indomitable";
	}
	case EFFECT_POWERFUL_SWINGS:
	{
		return "Powerful Swings";
	}
	case EFFECT_MIGHTY_BLOW:
	{
		return "Mighty Blow";
	}
	case EFFECT_SHIELD_DEFENSE:
	{
		return "Shield Defense";
	}
	case EFFECT_SHIELD_COVER:
	{
		return "Shield Cover";
	}
	case EFFECT_MOMENTUM:
	{
		return "Momentum";
	}
	case EFFECT_COUNTER_ATTACK:
	{
		return "Counter Attack";
	}
	case EFFECT_TAUNT:
	{
		return "Taunt";
	}
	case EFFECT_THREATEN:
	{
		return "Threaten";
	}
	case EFFECT_DISENGAGE:
	{
		return "Disengage";
	}
	case EFFECT_PRECISE_STRIKING:
	{
		return "Precise Striking";
	}
	case EFFECT_BERSERK:
	{
		return "Berserk";
	}
	case EFFECT_DEATH_MAGIC:
	{
		return "Death Magic";
	}
	case EFFECT_BLOOD_MAGIC:
	{
		return "Blood Magic";
	}
	case EFFECT_BLOOD_WOUND:
	{
		return "Blood Wound";
	}
	case EFFECT_BLOOD_CONTROL:
	{
		return "Blood Control";
	}
	case EFFECT_MARK_OF_DEATH:
	{
		return "Mark of Death";
	}
	case EFFECT_MIND_BLAST:
	{
		return "Mind Blast";
	}
	case EFFECT_FORCE_FIELD:
	{
		return "Force Field";
	}
	case EFFECT_SPIDER_POISON:
	{
		return "Spider Venom";
	}
	case EFFECT_SPIDER_WEB:
	{
		return "Spider Web";
	}
	case EFFECT_SHRIEK_SCREAM:
	{
		return "Chilling Shriek";
	}
	case EFFECT_HEADSHOT:
	{
		return "Headshot";
	}
	case EFFECT_SHOCKWAVE:
	{
		return "Shockwave";
	}
	case EFFECT_WEAKNESS:
	{
		return "Weakness";
	}
	case EFFECT_GENLOCK_SHIELD_ALLY:
	{
		return "Shield Ally";
	}
	case EFFECT_GENLOCK_SHIELD_SELF:
	{
		return "Shield Self";
	}
	case EFFECT_CORROSIVE_SPIT:
	{
		return "Corrosive Spit";
	}
	case EFFECT_GENLOCK_ACID_VIAL:
	{
		return "Choking Cloud";
	}
	case EFFECT_HURLOCK_DEEP_WOUNDS:
	{
		return "Deep Wounds";
	}
	case EFFECT_HURLOCK_BERSERK:
	{
		return "Berserk";
	}
	case EFFECT_NO_POWER_REGEN:
	{
		return "Exhausted";
	}
	case EFFECT_TIRED_3:
	{
		return "Exhausted";
	}
	case EFFECT_DIZZY:
	{
		return "Dizzy";
	}
	default:
	{
		break;
	}
	}

	return "Undefined Effect";
}

FString GetLetterRace(int32 nRace)
{
	switch (nRace)
	{
	case RACE_HUMAN:
		return "h";
	case RACE_ELF:
		return "e";
	case RACE_DWARF:
		return "d";
	case RACE_GENLOCK:
		return "glka";
	default:
		break;
	}
	return "h";
}

FString GetLetterGender(int32 nGender)
{
	switch (nGender)
	{
	case GENDER_MALE:
		return "m";
	case GENDER_FEMALE:
		return "f";
	default:
		break;
	}
	return "m";
}

UAnimSequence* GetAnimationResource(FString AnimationResource)
{
	FString sPath;

	if(GetCurrentPlayerController()->currentGameMode != EUI::COMBAT)
		sPath = "/Game/Data/Art/Anims/";
	else sPath = "/Game/Data/Art/Anims/Combat/";

	sPath += AnimationResource;
	sPath += ".";
	sPath += AnimationResource;

	UAnimSequence* animSequence = LoadAnimationSequenceFromPath(FName(*sPath));
	return animSequence;
}
