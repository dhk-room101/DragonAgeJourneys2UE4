#include "Effect.h"
#include "GameCharacter.h"
#include "Skills.h"
#include "Scripting.h"

UEffect::UEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UEffect::InitializeEffect(int32 nEffectId, UGameCharacter* charCaster, UGameCharacter* charTarget, int32 nLevel /*= -1*/)
{
	caster = charCaster;
	target = charTarget;

	if (nLevel == -1)
	{
		level = charCaster->level;
	}
	else
	{
		level = nLevel;
	}

	effectId = nEffectId;

	SetEffectProperties();

	//DHK
	/*const TCHAR* arChar = *GetEffectName(effectId);
	this->Rename(arChar, this->GetOuter(), REN_None);*/
}

void UEffect::ApplyTo(UGameCharacter* gameCharacter)
{
	if (type == EFFECT_TYPE_PASSIVE)
	{
		return;
	}
	
	/*if (type != EFFECT_TYPE_PERIODIC)
	{

	}*/

	if (type == EFFECT_TYPE_INSTANT)
	{
		if (healthDelta != 0)
		{
			if (healthDelta > 0)
			{
				gameCharacter->IncreaseHealth(healthDelta);
			}
			else
			{
				gameCharacter->TakeDamage(-healthDelta);
			}
		}
		
		if (powerDelta != 0)
		{
			if (powerDelta > 0)
			{
				gameCharacter->IncreasePower(powerDelta);
			}
			else
			{
				gameCharacter->ReducePower(-powerDelta);
			}
		}
		
		if (removesNegativeEffects)
		{
			gameCharacter->ClearNegativeEffects();
		}
	}
}

UEffect::~UEffect()
{
}

void UEffect::SetEffectProperties()
{
	switch (effectId)
	{
	case EFFECT_COUNTER_ATTACK:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		duration = EFFECT_DURATION_SPECIAL;
		beneficial = true;
		iconFrame = 2;
		suppressFloatingNotification = true;
		break;
	}
	case EFFECT_FLAMING_WEAPON:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_FIRE;
		weaponDamageModifier = 1 + FMath::RoundToInt(caster->GetSpellpower() / 2);
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 6;
		vfx = VFX_FLAMING_WEAPON;
		beneficial = true;
		break;
	}
	case EFFECT_FEIGN_DEATH:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 23;
		beneficial = true;
		break;
	}
	case EFFECT_DIRTY_FIGHTING:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		stun = true;
		duration = EFFECT_DURATION_NEXT_END;
		beneficial = false;
		iconFrame = 22;
		break;
	}
	case EFFECT_BELOW_THE_BELT:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		duration = 3;
		movementModifier = -1;
		defenseModifier = -6;
		beneficial = false;
		iconFrame = 17;
		break;
	}
	case EFFECT_STEALTH:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 41;
		beneficial = true;
		if (caster->GetStealthStrength() == 4)
		{
			movementModifier = 0;
		}
		else if (caster->GetStealthStrength() > 2)
		{
			movementModifier = -1;
		}
		else
		{
			movementModifier = -2;
		}
		hidden = true;
		vfx = VFX_STEALTH;
		break;
	}
	case EFFECT_TAUNT:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_MENTAL;
		mustTargetCaster = true;
		duration = 1;
		iconFrame = 42;
		break;
	}
	case EFFECT_THREATEN:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_MENTAL;
		enemyHateModifier = 100;
		beneficial = true;
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 43;
		break;
	}
	case EFFECT_DISENGAGE:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_MENTAL;
		enemyHateModifier = -100;
		beneficial = true;
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 47;
		break;
	}
	case EFFECT_PRECISE_STRIKING:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_MENTAL;
		attackModifier = 10;
		criticalChanceModifier = 5;
		if (caster->HasSkill(SKILL_PERFECT_STRIKE, true))
		{
			attackModifier = 25;
			criticalChanceModifier = 8;
			criticalDamageMultiplier = 1.25;
		}
		attackPowerCostMultiplier = 1.15;
		attackPowerCostModifier = 5;
		beneficial = true;
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 48;
		break;
	}
	case EFFECT_WAR_CRY:
	{
		type = EFFECT_TYPE_PASSIVE;
		duration = 3;
		source = EFFECT_MENTAL;
		attackModifier = -6;
		iconFrame = 53;
		break;
	}
	case EFFECT_RALLY:
	{
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_SPECIAL;
		source = EFFECT_MENTAL;
		beneficial = true;
		defenseModifier = 6;
		iconFrame = 112;
		break;
	}
	case EFFECT_INDOMITABLE:
	{
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_SPECIAL;
		beneficial = true;
		resistStun = 100;
		resistSlow = 100;
		damageMultiplier = 1.05;
		iconFrame = 29;
		break;
	}
	case EFFECT_POWERFUL_SWINGS:
	{
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_SPECIAL;
		beneficial = true;
		damageMultiplier = 1.15;
		attackModifier = -20;
		defenseModifier = -20;
		iconFrame = 115;
		break;
	}
	case EFFECT_MIGHTY_BLOW:
	{
		type = EFFECT_TYPE_PASSIVE;
		duration = 3;
		movementModifier = -1;
		beneficial = false;
		iconFrame = 35;
		break;
	}
	case EFFECT_SHIELD_DEFENSE:
	{
		if (caster->HasSkill(SKILL_SHIELD_BALANCE, true) == false)
		{
			attackModifier = -10;
		}
		beneficial = true;
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_SPECIAL;
		defenseMissileModifier = 10;
		defenseModifier = 10;
		iconFrame = 85;
		break;
	}
	case EFFECT_SHIELD_COVER:
	{
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_SPECIAL;
		source = EFFECT_PHYSICAL;
		beneficial = true;
		defenseModifier = 5;
		iconFrame = 90;
		break;
	}
	case EFFECT_OVERRUN:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		duration = EFFECT_DURATION_NEXT_END;
		movementModifier = 3;
		beneficial = true;
		iconFrame = 10;
		break;
	}
	case EFFECT_MOMENTUM:
	{
		duration = EFFECT_DURATION_NEXT_START;
		source = EFFECT_PHYSICAL;
		initiativeModifier = 10;
		iconFrame = 77;
		beneficial = true;
		break;
	}
	case EFFECT_DEATH_BLOW:
	{
		type = EFFECT_TYPE_INSTANT;
		source = EFFECT_PHYSICAL;
		powerDelta = 50;
		beneficial = true;
		break;
	}
	case EFFECT_DUAL_DOUBLE_STRIKE:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		duration = EFFECT_DURATION_SPECIAL;
		giveOffhandFullDamage = true;
		cancelCriticals = true;
		cancelBackstabs = true;
		iconFrame = 24;
		beneficial = true;
		break;
	}
	case EFFECT_DUAL_CRIPPLE:
	{
		defenseModifier = -8;
		attackModifier = -8;
		movementModifier = -1;
		source = EFFECT_PHYSICAL;
		type = EFFECT_TYPE_PASSIVE;
		duration = 3;
		iconFrame = 25;
		beneficial = false;
		break;
	}
	case EFFECT_AIM:
	{
		attackModifier = 8;
		weaponDamageModifier = 5;
		armorPenetrationModifier = 1;
		criticalChanceModifier = 2;
		attackPowerCostMultiplier = 1.3;
		attackPowerCostModifier = 10;
		iconFrame = 28;
		source = EFFECT_MENTAL;
		beneficial = true;
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_SPECIAL;
		break;
	}
	case EFFECT_DEFENSIVE_FIRE:
	{
		attackModifier = -10;
		defenseModifier = 10;
		iconFrame = 56;
		source = EFFECT_PHYSICAL;
		beneficial = true;
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_SPECIAL;
		break;
	}
	case EFFECT_PINNING_SHOT:
	{
		beneficial = false;
		source = EFFECT_PHYSICAL;
		movementModifier = -2;
		iconFrame = 57;
		type = EFFECT_TYPE_PASSIVE;
		duration = 3;
		break;
	}
	case EFFECT_SUPPRESSING_FIRE:
	{
		type = EFFECT_TYPE_PASSIVE;
		beneficial = false;
		source = EFFECT_MENTAL;
		attackModifier = -10;
		preventsRanged = true;
		iconFrame = 59;
		duration = 3;
		break;
	}
	case EFFECT_CRIPPLING_SHOT:
	{
		type = EFFECT_TYPE_PASSIVE;
		beneficial = false;
		source = EFFECT_PHYSICAL;
		attackModifier = -25;
		defenseModifier = -25;
		duration = EFFECT_DURATION_NEXT_END;
		iconFrame = 58;
		break;
	}
	case EFFECT_DISTRACTION:
	{
		disorient = true;
		beneficial = false;
		source = EFFECT_MENTAL;
		type = EFFECT_TYPE_PASSIVE;
		iconFrame = 63;
		duration = 3;
		break;
	}
	case EFFECT_SONG_VALOR:
	{
		beneficial = true;
		powerRegen = 3 + FMath::RoundToInt(caster->GetCunning() / 3);
		iconFrame = 65;
		duration = EFFECT_DURATION_SPECIAL;
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_MENTAL;
		break;
	}
	case EFFECT_SONG_COURAGE:
	{
		beneficial = true;
		attackModifier = 5 + FMath::RoundToInt(caster->GetCunning() / 4);
		weaponDamageModifier = 5 + FMath::RoundToInt(caster->GetCunning() / 5);
		criticalChanceModifier = 1 + FMath::RoundToInt(caster->GetCunning() / 10);
		iconFrame = 62;
		duration = EFFECT_DURATION_SPECIAL;
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_MENTAL;
		break;
	}
	case EFFECT_SONG_CAPTIVATE:
	{
		beneficial = false;
		stun = true;
		iconFrame = 64;
		duration = EFFECT_DURATION_NEXT_END;
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_MENTAL;
		break;
	}
	case EFFECT_BERSERK:
	{
		damageMultiplier = 1.25;
		powerRegen = -20;
		if (caster->HasSkill(SKILL_RESILIENCE))
		{
			powerRegen = powerRegen + 5;
			healthRegen = 10;
		}
		if (caster->HasSkill(SKILL_CONSTRAINT))
		{
			powerRegen = 0;
		}
		source = EFFECT_MENTAL;
		beneficial = true;
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 7;
		break;
	}
	case EFFECT_BLOOD_MAGIC:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_BLOOD;
		beneficial = true;
		skillsCostHealthInsteadOfPower = true;
		incomingHealingMultiplier = 0.2;
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 68;
		break;
	}
	case EFFECT_BLOOD_WOUND:
	{
		source = EFFECT_BLOOD;
		stun = true;
		duration = EFFECT_DURATION_NEXT_END;
		iconFrame = 69;
		break;
	}
	case EFFECT_DEATH_MAGIC:
	{
		source = EFFECT_CURSE;
		type = EFFECT_TYPE_PASSIVE;
		beneficial = true;
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 106;
		break;
	}
	case EFFECT_MARK_OF_DEATH:
	{
		source = EFFECT_CURSE;
		beneficial = false;
		incomingDamageMultiplier = 1.2;
		duration = EFFECT_DURATION_BATTLE;
		iconFrame = 71;
		break;
	}
	case EFFECT_MIND_BLAST:
	{
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_NEXT_END;
		stun = true;
		iconFrame = 108;
		beneficial = false;
		break;
	}
	case EFFECT_FORCE_FIELD:
	{
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_NEXT_END;
		source = EFFECT_MAGIC;
		stun = true;
		incomingDamageMultiplier = 0;
		vfx = VFX_FORCE_FIELD;
		iconFrame = 124;
		beneficial = true;
		break;
	}
	case EFFECT_CORROSIVE_SPIT:
	{
		type = EFFECT_TYPE_PASSIVE;
		beneficial = false;
		duration = 10;
		iconFrame = 138;
		armorModifier = -10;
		break;
	}
	case EFFECT_GENLOCK_SHIELD_ALLY:
	{
		type = EFFECT_TYPE_PASSIVE;
		beneficial = true;
		defenseModifier = 10;
		duration = EFFECT_DURATION_SPECIAL;
		iconFrame = 85;
		break;
	}
	case EFFECT_GENLOCK_SHIELD_SELF:
	{
		type = EFFECT_TYPE_PASSIVE;
		beneficial = true;
		defenseModifier = 12;
		defenseMissileModifier = 50;
		armorModifier = 6;
		duration = EFFECT_DURATION_NEXT_START;
		iconFrame = 85;
		break;
	}
	case EFFECT_GENLOCK_ACID_VIAL:
	{
		source = EFFECT_POISON;
		type = EFFECT_TYPE_PASSIVE;
		beneficial = false;
		duration = EFFECT_DURATION_NEXT_END;
		iconFrame = 61;
		movementModifier = -1;
		attackModifier = -10;
		break;
	}
	case EFFECT_HURLOCK_DEEP_WOUNDS:
	{
		source = EFFECT_PHYSICAL;
		type = EFFECT_TYPE_PERIODIC;
		beneficial = false;
		healthDelta = -5;
		duration = 3;
		iconFrame = 135;
		break;
	}
	case EFFECT_HURLOCK_BERSERK:
	{
		source = EFFECT_MENTAL;
		beneficial = true;
		duration = EFFECT_DURATION_BATTLE;
		iconFrame = 7;
		vfx = VFX_BERSERK;
		suppressFloatingNotification = true;
		movementModifier = 1;
		break;
	}
	case EFFECT_SPIDER_WEB:
	{
		type = EFFECT_TYPE_PASSIVE;
		beneficial = false;
		source = EFFECT_PHYSICAL;
		movementModifier = -999;
		duration = EFFECT_DURATION_NEXT_END;
		iconFrame = 122;
		vfx = VFX_WEB;
		break;
	}
	case EFFECT_SPIDER_POISON:
	{
		type = EFFECT_TYPE_PERIODIC;
		beneficial = false;
		source = EFFECT_POISON;
		healthDelta = -10;
		duration = 4;
		iconFrame = 134;
		break;
	}
	case EFFECT_HEADSHOT:
	{
		type = EFFECT_TYPE_PASSIVE;
		beneficial = false;
		source = EFFECT_MENTAL;
		attackPowerCostMultiplier = 1.25;
		duration = 3;
		iconFrame = 131;
		break;
	}
	case EFFECT_SHRIEK_SCREAM:
	{
		type = EFFECT_TYPE_PASSIVE;
		beneficial = false;
		source = EFFECT_MENTAL;
		duration = EFFECT_DURATION_NEXT_END;
		attackModifier = -15;
		defenseModifier = -15;
		movementModifier = -1;
		iconFrame = 123;
		break;
	}
	case EFFECT_SHOCKWAVE:
	{
		type = EFFECT_TYPE_PASSIVE;
		duration = EFFECT_DURATION_NEXT_END;
		source = EFFECT_PHYSICAL;
		beneficial = false;
		stun = true;
		iconFrame = 132;
		break;
	}
	case EFFECT_WEAKNESS:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_CURSE;
		duration = 3;
		beneficial = false;
		attackModifier = -10;
		defenseModifier = -10;
		movementModifier = -1;
		iconFrame = 137;
		break;
	}
	case EFFECT_REMOVE_NEGATIVE_EFFECTS:
	{
		type = EFFECT_TYPE_INSTANT;
		removesNegativeEffects = true;
		beneficial = true;
		break;
	}
	case EFFECT_NO_POWER_REGEN:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		duration = EFFECT_DURATION_NEXT_START;
		cancelPowerRegen = true;
		iconFrame = 61;
		break;
	}
	case EFFECT_TIRED_3:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		duration = 3;
		cancelPowerRegen = true;
		iconFrame = 61;
		break;
	}
	case EFFECT_DIZZY:
	{
		type = EFFECT_TYPE_PASSIVE;
		source = EFFECT_PHYSICAL;
		beneficial = false;
		duration = 5;
		randomFacing = true;
		iconFrame = 136;
		break;
	}
	default:
	{
		break;
	}
	}
}
