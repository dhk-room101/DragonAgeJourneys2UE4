#pragma once

const int32 ANIMATION_IDLE = 1;
const int32 ANIMATION_WALK = 2;
const int32 ANIMATION_ATTACK = 3;
const int32 ANIMATION_SPECIAL1 = 4;
const int32 ANIMATION_SPECIAL2 = 5;
const int32 ANIMATION_SPELL = 5;
const int32 ANIMATION_SPELL2 = 4;
const int32 ANIMATION_INJURY = 6;
const int32 ANIMATION_DEATH = 7;
const int32 ANIMATION_INSTANT_DEATH = 10;

bool IsLoopAnimation(int32 nAnim);