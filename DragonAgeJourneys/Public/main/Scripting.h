#pragma once

#include "CoreMinimal.h"
#include "EmbeddedAssets.h"
#include "DragonMain.h"

class UParty;
class UGameCharacter;
class UUserWidgetQuickBar;
class ADragonAgeJourneysPlayerController;
class UAnimSequence;

template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
{
	if (Path == NAME_None) return NULL;

	return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}

static FORCEINLINE UAnimSequence* LoadAnimationSequenceFromPath(const FName& Path)
{
	if (Path == NAME_None) return NULL;

	return LoadObjFromPath<UAnimSequence>(Path);
}

FName GetFileFromClassName(FName fName);
void ShowBark(FString sBark, UGameCharacter* uGameChar = nullptr);
void TriggerConversation(FString sConversation, FString locId /*, Function fCallback*/);
bool TriggerDialog(FString sDialog, bool bVisited);
bool TriggerTutorial(int32 nTutorial);
UUserWidgetQuickBar* GetQuickBar();
UWorld* GetStage();
UDragonMain* GetMain();
UParty* GetParty();
UEmbeddedAssets* GetAssets();
ADragonAgeJourneysPlayerController* GetCurrentPlayerController();
FVector GetBattleTraceImpact();
FString GetEffectName(int32 nEffectId);
FString GetLetterRace(int32 nRace);
FString GetLetterGender(int32 nGender);
UAnimSequence* GetAnimationResource(FString AnimationResource);
