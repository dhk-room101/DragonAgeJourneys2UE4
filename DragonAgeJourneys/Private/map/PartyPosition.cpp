#include "PartyPosition.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Map.h"
#include "DragonAgeJourneysCharacter.h"
#include "Animation.h"

UPartyPosition::UPartyPosition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UPartyPosition::EndMove()
{
	moving = false;
	//TODO UpdateAvatars(false);
}

void UPartyPosition::FaceTowards(float fPosX)
{
	//TODO FaceTowards
}

void UPartyPosition::BeginMove(float fFacing /*= 0*/)
{
	//TODO PartyPosition BeginMove
	moving = true;

	for (int32 i = 0; i < avatars.Num(); i++)
	{
		ADragonAgeJourneysCharacter* avatar = avatars[i];
		if (avatar->currentAnimation != ANIMATION_WALK)
		{
			avatar->AnimateAvatar(ANIMATION_WALK);
		}
		/*if (arg1 < this.x)
		{
			avatar->SetFacingLeft(true);
			continue;
		}
		if (arg1 > this.x)
		{
			avatar->SetFacingLeft(false);
		}*/
	}
}

void UPartyPosition::UpdatePartyPosition(FVector fVector)
{
	X = fVector.X;
	Y = fVector.Y;

	for (int32 i = 0; i < avatars.Num(); i++)
	{
		ADragonAgeJourneysCharacter* avatar = avatars[i];
		avatar->SetActorLocation(FVector(X, Y, fVector.Z));
	}
}

UPartyPosition::~UPartyPosition()
{
}
