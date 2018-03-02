#include "Animation.h"

bool IsLoopAnimation(int32 nAnim)
{
	switch (nAnim)
	{
	case ANIMATION_IDLE:
	case ANIMATION_WALK:
	{
		return true;
	}
	case ANIMATION_ATTACK:
	case ANIMATION_INJURY:
	case ANIMATION_DEATH:
	default:
	{
		break;
	}
	}
	
	return false;
}

