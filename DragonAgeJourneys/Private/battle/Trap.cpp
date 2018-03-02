#include "Trap.h"
#include "DragonAgeJourneys.h"
#include "DragonMain.h"

ATrap::ATrap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

int32 ATrap::GetDamage()
{
	return level * 10;
}
