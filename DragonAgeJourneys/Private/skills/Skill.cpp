#include "Skill.h"
#include "Skills.h"
#include "Effect.h"
#include "Animation.h"
#include "ActionIndicator.h"
#include "Gender.h"
#include "TalentLine.h"
#include "GameCharacter.h"
#include "Slot.h"
#include "Equipment.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Map.h"
#include "Consumable.h"
#include "Classes.h"
#include "Difficulty.h"
#include "Group.h"
#include "Battlefield.h"
#include "Animation/AnimationAsset.h"
#include "CharacterCreation.h"

USkill::USkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void USkill::InitializeSkill(int32 nSkillId, UGameCharacter* uGameChar)
{
	skillId = nSkillId;
	char_ref = uGameChar;
	aiPriority = 5;
	SetSkillProperties();

	//DHK
	/*if (nSkillId != SKILL_MOVE && nSkillId != SKILL_STRIKE)
	{
		FString sSpace = " ";
		if (skillname.Contains(sSpace))
		{
			FString sLeft;
			FString sRight;
			skillname.Split(TEXT(" "), &sLeft, &sRight);
			skillname = sLeft + sRight;
		}
		const TCHAR* arChar = *skillname;
		this->Rename(arChar, this->GetOuter(), REN_None);
	}*/
}

//TODO GetTargetAnimation
UAnimationAsset* USkill::GetTargetAnimation()
{
	return NewObject<UAnimationAsset>();
}

bool USkill::IsUnlocked()
{
	if (consumable != nullptr)
	{
		return true;
	}
	if (talentLine && talentLine->unlocked > GetTalentIndex())
	{
		return true;
	}

	return false;
}

bool USkill::IsUsable()
{
	if (type == SKILL_MOVE) return true;//DHK

	if (type == SKILL_TYPE_TOGGLED && toggledOn)
	{
		return true;
	}

	if (currentCooldown > 0)
	{
		return false;
	}

	if (powerCost > 0)
	{
		if (char_ref->IsUsingHealthForPower())
		{
			if (char_ref->health < powerCost)
			{
				return false;
			}
		}
		else if (char_ref->power < powerCost)
		{
			return false;
		}
	}

	if (!MeetsEquipmentRequirements())
	{
		return false;
	}

	if (requiresLowHealth && char_ref->GetHealthPercent() > 20)
	{
		return false;
	}

	for (int32 i = 0; i < char_ref->effects.Num(); i++)
	{
		UEffect* effect = char_ref->effects[i];
		if (effect->preventsRanged && (rangedAttack || directAttack))
		{
			return false;
		}
	}

	if (GetMinimumEnemyDistance() > 1)
	{
		if (GetMain()->Battlefield && GetMain()->Battlefield->GetClosestEnemyDistanceTo(char_ref) < GetMinimumEnemyDistance())
		{
			return false;
		}
	}

	if (skillId == SKILL_CONSUMABLE)
	{
		if (consumable == nullptr)
		{
			return false;
		}
		if (consumable->stack <= 0)
		{
			return false;
		}
		if (!consumable->UsableBy(char_ref, true))
		{
			return false;
		}
	}

	if (GetMain()->LevelMap != nullptr)
	{
		if (!targetsFriendlies && !hitsAllFriendlies && !targetsSelf)
		{
			return false;
		}

		if (GetHealing() && !GetPowerDelta() && !resurrects)
		{
			return false;
		}
	}

	if (!(char_ref->ai == nullptr) && !AiFlagsOkay())
	{
		return false;
	}

	return true;
}

bool USkill::MeetsEquipmentRequirements()
{
	UEquipment* itemMain = char_ref->gear[SLOT_WEAPON];
	UEquipment* itemOff = char_ref->gear[SLOT_OFFHAND];
	UEquipment* itemArmor = char_ref->gear[SLOT_ARMOR];

	if (requiresMediumArmor)
	{
		if (itemArmor == nullptr)
		{
			return false;
		}
		if (itemArmor->IsHeavyArmor())
		{
			return false;
		}
	}
	if (itemMain == nullptr)
	{
		if (requiresWeapon)
		{
			return false;
		}
		if (requiresMelee)
		{
			return false;
		}
		if (requiresRanged)
		{
			return false;
		}
		if (requiresDualWield)
		{
			return false;
		}
		if (requiresTwoHander)
		{
			return false;
		}
	}
	if (itemOff == nullptr)
	{
		if (requiresDualWield)
		{
			return false;
		}
		if (requiresShield)
		{
			return false;
		}
	}
	if (requiresMelee && itemMain->IsMelee() == false)
	{
		return false;
	}
	if (requiresRanged && itemMain->IsRanged() == false)
	{
		return false;
	}
	if (requiresShield && itemOff->IsShield() == false)
	{
		return false;
	}
	if (requiresDualWield && (!itemMain->IsMelee() || !itemOff->IsMelee()))
	{
		return false;
	}
	if (requiresTwoHander && itemMain->IsTwoHander() == false)
	{
		return false;
	}
	return true;
}

bool USkill::ShowDirection()
{
	if (rangedAttack)
	{
		return false;
	}

	if (hitsAllEnemies)
	{
		return false;
	}

	if (hitsAllFriendlies)
	{
		return false;
	}

	if (directAttack)
	{
		return false;
	}

	if (consumable)
	{
		return false;
	}

	return true;
}

int32 USkill::GetTalentIndex()
{
	int32 nIndex = 0;

	while (nIndex < talentLine->skillIds.Num())
	{
		if (talentLine->skillIds[nIndex] == skillId)
		{
			return nIndex;
		}
		++nIndex;
	}

	return -1;
}

int32 USkill::GetMinimumEnemyDistance()
{
	if (skillId == SKILL_STEALTH1 && char_ref->GetStealthStrength() > 2)
	{
		return 3;//TODO hardcoded?
	}

	return _minimumEnemyDistance;
}

void USkill::SetRange(int32 nAmount)
{
	_range = nAmount;
}

int32 USkill::GetRange()
{
	if (usesWeaponRange)
	{
		UEquipment* weapon = char_ref->gear[SLOT_WEAPON];
		if (requiresRanged && weapon->IsRanged())
		{
			return weapon->range;
		}
	}

	return _range;
}

void USkill::SetMinimumRange(int32 nAmount)
{
	_minimumRange = nAmount;
}

int32 USkill::GetMinimumRange()
{
	if (requiresRanged)
	{
		if (char_ref->HasSkill(SKILL_MELEE_ARCHER))
		{
			return 1;//TODO hardcoded
		}
		return 2;
	}

	return _minimumRange;
}

void USkill::SetPowerCost(int32 nAmount)
{
	powerCost = nAmount;
}

int32 USkill::GetPowerCost()
{
	int32 nCost = 0;

	if (powerCost == 0)
	{
		return 0;
	}

	if (costsRemainingPower)
	{
		return powerCost;
	}

	nCost = FMath::RoundToInt((powerCost + char_ref->GetAttackCostModifier()) * char_ref->GetAttackCostModifier());

	return nCost;
}

void USkill::SetBaseHealing(int32 nAmount)
{
	_healing = nAmount;
}

int32 USkill::GetBaseHealing()
{
	if (consumable != nullptr)
	{
		return consumable->GetHealing(char_ref);
	}

	return _healing;
}

void USkill::SetPowerDelta(int32 nAmount)
{
	_powerDelta = nAmount;
}

int32 USkill::GetPowerDelta()
{
	if (consumable != nullptr)
	{
		return consumable->GetPower(char_ref);
	}

	return _powerDelta;
}

int32 USkill::GetHealing()
{
	int32 nHealing = GetBaseHealing();
	if (!IsSpell() || GetBaseClassOf(char_ref->classId) != CLASS_MAGE)
	{
		return GetBaseHealing();
	}

	int32 nBonus = 1 + char_ref->GetSpellpowerForSkill(this) * SPELLPOWER_BONUS_PC_HEALING;
	nHealing = nHealing * nBonus;
	nHealing = nHealing + char_ref->GetHealingBonus();

	return FMath::RoundToInt(nHealing);
}

int32 USkill::GetDamage()
{
	float loc2 = 0;
	float loc3 = 0;
	int32 nDamage = damage;

	if (char_ref->IsMonster())
	{
		if (monsterDamageScale > 0)
		{
			nDamage = char_ref->GetMonsterBaseDamage() * monsterDamageScale;
			nDamage = ModifyMonsterDamage(nDamage);
		}
		else
		{
			return 0;
		}
	}

	if (addWeaponDamage && char_ref != nullptr)
	{
		loc2 = char_ref->GetWeaponDamage();
		loc2 = loc2 * scaleWeaponDamage;
		nDamage = nDamage + loc2;
	}

	if (IsSpell() && GetBaseClassOf(char_ref->classId) == CLASS_MAGE)
	{
		loc3 = 1 + char_ref->GetSpellpowerForSkill(this) * SPELLPOWER_BONUS_PC_DAMAGE;
		nDamage = nDamage * loc3;
	}

	nDamage = nDamage * char_ref->GetDamageMultiplier();

	return FMath::RoundToInt(nDamage);
}

bool USkill::IsSpell()
{
	if (!UsesMana(char_ref->classId))
	{
		return false;
	}

	if (IsWeaponAttack())
	{
		return false;
	}

	if (consumable)
	{
		return false;
	}

	return true;
}

bool USkill::IsWeaponAttack()
{
	if (addWeaponDamage)
	{
		return true;
	}

	if (scaleWeaponDamage != 1)
	{
		return true;
	}

	return false;
}

bool USkill::GetAlwaysHits()
{
	if (_alwaysHits)
	{
		return true;
	}

	if (targetsFriendlies)
	{
		return true;
	}

	if (IsSpell())
	{
		return true;
	}

	return false;
}

FString USkill::GetSkillDescription()
{
	UEffect* _effect = NewObject<UEffect>();
	FString sString;

	if (targetEffectId != -1)
	{
		_effect->InitializeEffect(targetEffectId, char_ref, nullptr, char_ref->level);
	}

	switch (skillId)
	{
	case SKILL_MOVE:
	{
		return "Basic movement.";
	}
	case SKILL_STRIKE:
	{
		return "Basic melee attack on single target. ";
	}
	case SKILL_BOW_FIRE:
	{
		return "Basic ranged attack on single target. ";
	}
	case SKILL_MAGE_BOLT:
	{
		return "Basic magic attack on single target. ";
	}
	case SKILL_CONSUMABLE:
	{
		return consumable->ItemStatsString("") + "\n(" + FString::FromInt(consumable->stack) + " remaining)";
	}
	case SKILL_OVERRUN:
	{
		return "The character has learnt to flow freely from one felled opponent to the next. Whenever this character kills an enemy, they may move additional spaces on their next turn.";
	}
	case SKILL_WEAPON_SWEEP:
	{
		return "The character swings a two-handed weapon through enemies in a vicious arc. Enemies on either side of the target are also hit.";
	}
	case SKILL_MIGHTY_BLOW:
	{
		return "The character puts extra weight and effort behind a single strike, gaining a bonus to accuracy. If it hits, the blow deals increased damage and slows the target\'s movement speed for several turns.";
	}
	case SKILL_POWERFUL_SWINGS:
	{
		return "While in this mode, the character puts extra muscle behind each swing, gaining a bonus to damage but suffering penalties to attack and defense.";
	}
	case SKILL_SHATTERING_BLOWS:
	{
		return "The character\'s blows have become terrifyingly destructive, shattering and turning enemies\' armor against them. All attacks with two-handed weapons against armored targets now cause additional damage and reduce the target\'s armor value.";
	}
	case SKILL_SUNDER_ARMOR:
	{
		return "The character aims a destructive blow at the target\'s armor or natural defenses. The attack deals normal damage, but also damages the armor unless the target passes a physical resistance check.";
	}
	case SKILL_INDOMITABLE:
	{
		return "Through sheer force of will, the character remains in control on the battlefield, gaining a slight increase to their damage dealt while being immune to stun or slowing effects for the duration of this mode.";
	}
	case SKILL_STUNNING_BLOWS:
	{
		return "The character\'s fondness for massive two-handed weapons means that each attack automatically offers a small chance to stun the opponent, due to the sheer weight behind the blow.";
	}
	case SKILL_CRITICAL_STRIKE:
	{
		return "The character makes a single massive swing at the target, gaining a bonus to attack. If the strike connects, it is an automatic critical hit, possibly killing the opponent outright if its health is low enough.";
	}
	case SKILL_SPELL_BOOST1:
	{
		return "The mage has increased their proficiency in all forms of magic. They receive a permanent +5 bonus to their spellpower.";
	}
	case SKILL_SPELL_BOOST2:
	{
		return "The mage has further increased their proficiency in all forms of magic. Their permanent bonus to spellpower increases to +10.";
	}
	case SKILL_SPELL_BOOST3:
	{
		return "The mage has advanced proficiency in all forms of magic. Their permanent bonus to spellpower increases to +15.";
	}
	case SKILL_SPELL_BOOST4:
	{
		return "The mage has become an expert at wielding all forms of magic. Their permanent bonus to spellpower increases to +20.";
	}
	case SKILL_LIGHTNING:
	{
		return "The caster fires a bolt of lightning at a single target.";
	}
	case SKILL_SHOCK:
	{
		return "The caster\'s hands erupt with a cone of lightning, damaging all targets in the area. Friendly fire possible.";
	}
	case SKILL_TEMPEST:
	{
		return "The caster unleashes a fierce lightning storm that deals electricity damage to anyone in the targeted area. Friendly fire possible.";
	}
	case SKILL_CHAIN_LIGHTNING:
	{
		return "The caster\'s hands erupt with a bolt of lightning that inflicts electricity damage on a target, then forks, sending smaller bolts jumping to those nearby, which fork again. Each fork does less damage than the previous.";
	}
	case SKILL_FLAME_BLAST:
	{
		return "The caster\'s hand erupts with a cone of flame, inflicting fire damage on all targets up to four squares directly ahead. Friendly fire possible.";
	}
	case SKILL_FLAMING_WEAPONS:
	{
		return "While this spell is active, the caster enchants the party\'s melee weapons with flame so that they deal additional fire damage with each successful attack.";
	}
	case SKILL_FIREBALL:
	{
		return "The caster\'s hands erupt with an explosive ball of flame, inflicting fire damage on all characters in the target area. Friendly fire possible.";
	}
	case SKILL_INFERNO:
	{
		return "The caster summons a huge column of swirling flame. All targets in the area take constant fire damage as they burn. Friendly fire possible.";
	}
	case SKILL_MIND_BLAST:
	{
		return "The caster projects a wave of telekinetic force that knocks the target backwards, dealing minor damage. If the target fails a mental resistance check they are also stunned.";
	}
	case SKILL_FORCE_FIELD:
	{
		return "The caster erects a telekinetic barrier around a target, who becomes completely immune to damage for the duration of the spell but cannot move. Friendly fire possible.";
	}
	case SKILL_TELEKENETIC_WEAPONS:
	{
		return "While this mode is active, the caster enchants the party\'s melee weapons with telekinetic energy that increases armor penetration and accuracy.";
	}
	case SKILL_CRUSHING_PRISON:
	{
		return "The caster encloses a target in a collapsing cage of telekinetic force. The target takes damage every round and is unable to move until they pass a physical resistance check.";
	}
	case SKILL_DRAIN_LIFE:
	{
		return "The caster creates a sinister bond with the target, draining its life energy in order to heal the caster.";
	}
	case SKILL_DEATH_MAGIC:
	{
		return "While active, the caster draws in nearby entropic energy, gaining a bonus to health regeneration and spellpower for every dead or dying character on the battlefield.";
	}
	case SKILL_CURSE_OF_MORTALITY:
	{
		return "The caster curses a target with the inevitability of true death. While cursed, the target cannot heal or regenerate health and takes continuous spirit damage.";
	}
	case SKILL_DEATH_CLOUD:
	{
		return "The caster summons a cloud of leeching entropic energy that deals continuous spirit damage to all who enter. Friendly fire possible.";
	}
	case SKILL_HEAL:
	{
		return "The caster causes flesh to knit miraculously, instantly healing an ally by a moderate amount.";
	}
	case SKILL_CURE:
	{
		return "The caster magically soothes and restores the target, removing all negative effects.";
	}
	case SKILL_REGENERATION:
	{
		return "The caster infuses an ally with beneficial energy, greatly accelerating health regeneration for a short time.";
	}
	case SKILL_MASS_REJUVENATION:
	{
		return "The caster channels a stream of rejuvenating energy to all members of the party, significantly increasing mana and stamina regeneration for a short duration.";
	}
	case SKILL_GROUP_HEAL:
	{
		return "The caster bathes allies in benevolent energy, instantly healing them by a moderate amount.";
	}
	case SKILL_REVIVAL:
	{
		return "The caster revives a fallen party member, raising them from unconsciousness and restoring some health.";
	}
	case SKILL_LIFEWARD:
	{
		return "The caster places a protective ward on an ally that automatically restores health when the ally falls close to death.";
	}
	case SKILL_CLEANSING_AURA:
	{
		return "While this mode is active, waves of healing and cleansing energy emanate from the caster, restoring health to all nearby allies every round.";
	}
	case SKILL_BELOW_THE_BELT:
	{
		return "The rogue strikes low, dealing normal combat damage as well as imposing penalties to defense and movement speed unless the target passes a physical resistance check.";
	}
	case SKILL_DIRTY_FIGHTING:
	{
		return "The rogue incapacitates a target, who takes no damage from the attack but is stunned for a short time.";
	}
	case SKILL_COMBAT_MOVEMENT:
	{
		return "The quick-stepping rogue can more easily outmaneuver opponents, granting an additional movement hex during battle.";
	}
	case SKILL_LETHALITY:
	{
		return "The rogue has a keen eye for weak spots, and thus gains a bonus to the chances of landing a critical hit for all attacks.";
	}
	case SKILL_EVASION:
	{
		return "The rogue gains an almost preternatural ability to sense and avoid danger. This talent grants a one-in-five chance of evading physical attacks.";
	}
	case SKILL_FEIGN_DEATH:
	{
		return "The rogue collapses at enemies\' feet, making them lose interest and seek other targets until the rogue gives up the ruse.";
	}
	case SKILL_COUP_DE_GRACE:
	{
		return "When a target is incapacitated, the opportunistic rogue strikes where it hurts the most, inflicting automatic backstabs against stunned or paralyzed targets.";
	}
	case SKILL_STEALTH1:
	{
		sString = "When in this mode, the rogue can move around the battlefield unseen. Incurs a penalty to movement range while active, and moving too close to enemies may break your stealth. Standard attacks made from stealth gain an automatic backstab damage bonus, even from the front.";
		if (GetMinimumEnemyDistance() > 1)
		{
			sString = sString + (" Can only be initiated when no enemies are within " + FString::FromInt((GetMinimumEnemyDistance() - 1)) + " spaces of character.");
		}
		return sString;
	}
	case SKILL_STEALTH2:
	{
		return "Reduces the likelihood of being spotted when moving near enemies, and reduces the movement penalty while stealthed.";
	}
	case SKILL_STEALTH3:
	{
		return "Significantly reduces the likelihood of being spotted when moving near enemies, and reduces the distance from enemies necessary to enter stealth.";
	}
	case SKILL_STEALTH4:
	{
		return "Greatly reduces the likelihood of being spotted when moving near enemies, and eliminates the movement penalty while stealthed.";
	}
	case SKILL_DEADLY_STRIKE:
	{
		return "The rogue makes a swift strike at a vulnerable area on the target, dealing normal damage but gaining a bonus to armor penetration. The target cannot counter-attack.";
	}
	case SKILL_DUAL_DOUBLE_STRIKE:
	{
		return "When in this mode, the character strikes with the full potential of both weapons simultaneously. Attacks cause more damage, but the character cannot inflict regular critical hits or backstabs.";
	}
	case SKILL_DUAL_CRIPPLE:
	{
		return "The character strikes low, crippling the target with penalties to movement speed, attack, and defense.";
	}
	case SKILL_DUAL_RIPOSTE:
	{
		return "Perform a standard attack with both weapons, and enable a counter-attack if attacked during the next turn.";
	}
	case SKILL_PUNISHER:
	{
		return "Strike three times.";
	}
	case SKILL_DUAL_WEAPON_MOMENTUM:
	{
		return "The character has learned to carry one attack through to the next. Whenever they successfully hit an opponent they will receive a bonus to their initiative roll for the next round, making it more likely they will act before enemies.";
	}
	case SKILL_WHIRLWIND:
	{
		return "The character flies into a whirling dance of death, striking out at surrounding enemies with both weapons.";
	}
	case SKILL_DUAL_WEAPON_SWEEP:
	{
		return "The character sweeps both weapons in a broad forward arc, striking nearby enemies with one or both weapons and inflicting significantly more damage than normal.";
	}
	case SKILL_DUAL_WEAPON_FLURRY:
	{
		return "The character lashes out with a flurry blows, dealing high damage to a single target.";
	}
	case SKILL_THREATEN:
	{
		return "The warrior adopts a challenging posture that increases enemy hostility, drawing them away from other allies while this mode is active.";
	}
	case SKILL_TAUNT:
	{
		return "A mocking bellow catches the attention of a nearby foe, forcing their attention onto the warrior for the next turn.";
	}
	case SKILL_DISENGAGE:
	{
		return "A relaxed position makes the warrior seem less threatening, reducing the hostility of enemies, who may seek other targets instead.";
	}
	case SKILL_AIM:
	{
		return "The archer carefully places each shot for maximum effect while in this mode. This increases stamina costs but grants bonuses to attack, damage, armor penetration, and critical chance.";
	}
	case SKILL_DEFENSIVE_FIRE:
	{
		return "While active, the archer changes stance, receiving a bonus to defense but suffering a penalty to hit.";
	}
	case SKILL_MELEE_ARCHER:
	{
		return "Experience fighting in tight quarters has taught the archer to fire without interruption, even when being attacked. Removes the minimum range requirement when using bows.";
	}
	case SKILL_MASTER_ARCHER:
	{
		return "Deadly with the bow, master archers receive a permanent bonus to hit, increased damage from critical hits, and reduced stamina costs for archery talents.";
	}
	case SKILL_PINNING_SHOT:
	{
		return "A shot to the target\'s legs disables the foe, greatly slowing movement speed temporarily.";
	}
	case SKILL_SUPPRESSING_FIRE:
	{
		return "A barrage of fire near the target unsettles them, temporarily inflicting a large penalty to their attack. In addition, the target is unable to use ranged attacks for a time. This attack causes no damage.";
	}
	case SKILL_CRIPPLING_SHOT:
	{
		return "A carefully aimed shot hampers the target\'s ability to fight by reducing attack and defense if it hits, although the shot inflicts only normal damage.";
	}
	case SKILL_CRITICAL_SHOT:
	{
		return "The archer generates an automatic critical hit if this shot finds its target. The archer suffers reduced stamina regeneration for a time.";
	}
	case SKILL_PRECISE_STRIKING:
	{
		return "The warrior tries to make each attack count, gaining a bonus to attack as well as an increased chance to score critical hits. However, all attacks consume additional stamina while this mode is active.";
	}
	case SKILL_PERFECT_STRIKE:
	{
		return "Increases the positive effects of Precise Striking. In addition, the warrior\'s critical hits while the mode is active will now do bonus damage.";
	}
	case SKILL_POWERFUL:
	{
		return "Through training and hard work, the warrior has gained greater health and reduced the fatigue penalty for wearing heavy armor.";
	}
	case SKILL_BRAVERY:
	{
		return "The warrior\'s unwavering courage grants bonuses to damage, physical resistance, and mental resistance.";
	}
	case SKILL_DEATH_BLOW:
	{
		return "Each time the warrior fells an enemy, the end of the battle seems closer at hand, restoring a portion of the warrior\'s stamina.";
	}
	case SKILL_SHIELD_DEFENSE:
	{
		return "While this mode is active, the character drops into a defensive stance that favors the shield, gaining a bonus to defense and an increased chance to shrug off missile attacks, but taking a penalty to attack.";
	}
	case SKILL_SHIELD_BALANCE:
	{
		return "The character has learned to compensate for the weight of a shield in combat and no longer suffers an attack penalty while using Shield Defense.";
	}
	case SKILL_SHIELD_WALL:
	{
		return "In this mode, the character\'s shield becomes nearly a fortress, adding a significant bonus to armor and a greater likelihood of shrugging off missile attacks, but at the cost of reduced damage. Shield Expertise makes the character immune to physical effects while in this mode, and Shield Mastery gives a bonus to defense.";
	}
	case SKILL_SHIELD_EXPERTISE:
	{
		return "The character\'s experience using a shield in combat has made certain abilities more efficient, increasing the defense bonus for Shield Defense and making the character immune to physical effects while using Shield Wall. In addition, the fatigue penalty for heavy shields is reduced.";
	}
	case SKILL_SHIELD_BLOCK:
	{
		return "Practice fighting with a shield improves the character\'s guard. With a shield equipped, enemies can no longer gain flanking bonuses against this character.";
	}
	case SKILL_SHIELD_COVER:
	{
		return "The warrior has learned to use their shield to partially protect nearby allies. Friendly characters adjacent to the warrior gain a defensive bonus.";
	}
	case SKILL_SHIELD_TACTICS:
	{
		return "The character has become proficient enough with a shield to defend from all angles. With a shield equipped, attackers may no longer inflict flanking strikes, backstabs, or critical hits against this character.";
	}
	case SKILL_SHIELD_MASTERY:
	{
		return "The character has mastered the use of the shield and receives additional benefits when using Shield Defense, Shield Wall, and Shield Cover. In addition, the fatigue penalty for heavy shields is reduced.";
	}
	case SKILL_WAR_CRY:
	{
		return "The champion lets out a fearsome cry that gives nearby enemies a penalty to attack.";
	}
	case SKILL_RALLY:
	{
		return "The champion\'s presence inspires nearby allies, giving them bonuses to defense while this mode is active.";
	}
	case SKILL_MOTIVATE:
	{
		return "The champion inspires allies to attack with renewed vigor. The Rally talent now increases attack, in addition to its defense bonus.";
	}
	case SKILL_SUPERIORITY:
	{
		return "The champion is so fearsome that War Cry now stuns nearby opponents unless they pass a physical resistance check.";
	}
	case SKILL_SONG_OF_VALOR:
	{
		return "The bard sings an ancient tale of valorous heroes, granting the party bonuses to mana or stamina regeneration at a rate affected by the bard\'s cunning. The bard can only sing one song at a time.";
	}
	case SKILL_DISTRACTION:
	{
		return "The bard\'s performance, replete with dizzying flourishes, is designed to distract and confuse. The target forgets who it was fighting and becomes disoriented unless it passes a mental resistance check.";
	}
	case SKILL_SONG_OF_COURAGE:
	{
		return "The bard launches into an epic song of the party\'s exploits, granting them bonuses to attack, damage, and critical chance. The size of the bonuses are affected by the bard\'s cunning. The bard can only sing one song at a time.";
	}
	case SKILL_CAPTIVATE:
	{
		return "The bard begins an entrancing song that stuns hostile targets nearby unless they pass a mental resistance check. Will cancel any other active song.";
	}
	case SKILL_BERSERK:
	{
		return "The stench of blood and death drives the berserker into a willing fury, providing a bonus to damage. Rages incur a penalty to stamina regeneration, however, which Constraint removes. Resilience adds a bonus to health regeneration in this mode.";
	}
	case SKILL_RESILIENCE:
	{
		return "Rages no longer wear so heavily on the berserker\'s body. The stamina regeneration penalty applied by Berserk is reduced, health regeneration is increased while Berserk, and the berserker gains a permanent bonus to physical resistance.";
	}
	case SKILL_CONSTRAINT:
	{
		return "The berserker has learned to retain control during rages, entirely removing Berserk\'s penalty to stamina regeneration.";
	}
	case SKILL_FINAL_BLOW:
	{
		return "All the berserker\'s stamina goes into a single swing. If the blow connects, the attack inflicts extra damage proportional to the amount of stamina lost.";
	}
	case SKILL_BLOOD_MAGIC:
	{
		return "For as long as this mode is active, the blood mage sacrifices health to power spells instead of expending mana, but effects that heal the blood mage are much less effective than normal.";
	}
	case SKILL_BLOOD_SACRIFICE:
	{
		return "The blood mage sucks the life-force from an ally, healing the caster but potentially killing the ally. This healing is not affected by the healing penalty of Blood Magic.";
	}
	case SKILL_BLOOD_WOUND:
	{
		return "The blood of all hostile targets in the area boils within their veins, inflicting severe damage. Targets stand twitching, unable to move unless they pass a physical resistance check. Creatures without blood are immune.";
	}
	case SKILL_BLOOD_CONTROL:
	{
		return "The blood mage forcibly controls the target\'s blood, making the target an ally of the caster unless it passes a mental resistance check. If the target resists, it instead takes great damage from the manipulation of its blood. Creatures without blood are immune.";
	}
	case SKILL_MARK_OF_DEATH:
	{
		return "The assassin marks a target, revealing weaknesses that others can exploit. All attacks against a marked target deal additional damage.";
	}
	case SKILL_EXPLOIT_WEAKNESS:
	{
		return "A keen eye and a killer instinct help the assassin exploit a target\'s weak points. During a successful backstab attack, the assassin gains additional damage based on Cunning.";
	}
	case SKILL_LACERATE:
	{
		return "Whenever a backstab deals enough damage, the assassin\'s foe is riddled with bleeding wounds that inflict additional damage for a short time.";
	}
	case SKILL_FEAST_OF_THE_FALLEN:
	{
		return "The assassin thrives on the moment of death. Stamina is partially restored whenever the assassin fells an opponent with a backstab.";
	}
	case SKILL_DWARF_RACIAL:
	{
		return DWARF_STRING + "\n\nDwarves have +1 to strength and dexterity, as well as +2 to constitution.";
	}
	case SKILL_ELF_RACIAL:
	{
		return ELF_STRING + "\n\nElves have +2 to willpower and magic.";
	}
	case SKILL_HUMAN_RACIAL:
	{
		return HUMAN_STRING + "\n\nHumans have +1 to strength, dexterity, cunning and magic.";
	}
	case SKILL_GREY_WARDEN:
	{
		return "The Grey Wardens have been the defenders of Thedas against the Darkspawn since the first Blight. Their lives are dedicated to destroying the tainted creatures, and as such Grey Wardens receive a bonus to all damage inflicted against Darkspawn.";
	}
	case 0:
	default:
	{
		break;
	}
	}

	return "<not implemented description> for " + FString::FromInt(skillId);
}

USkill::~USkill()
{
}

void USkill::SetSkillProperties()
{
	switch (skillId)
	{
	default:
	{
		break;
	}
	case SKILL_DUMMY_SKILL:
	{
		skillname = "Dummy Skill";
		damage = 0;
		powerCost = 0;
		iconFrame = 1;
		break;
	}
	case SKILL_CONSUMABLE:
	{
		skillname = "Consumable";
		damage = 0;
		powerCost = 0;
		break;
	}
	case SKILL_MOVE:
	{
		skillname = "Move";
		powerCost = 0;
		iconFrame = 1;
		moveOnly = true;
		break;
	}
	case SKILL_MAGE_BOLT:
	{
		skillname = "Staff Bolt";
		powerCost = 0;
		damageType = EFFECT_MAGIC;
		damage = 0;
		_minimumRange = 2;
		addWeaponDamage = true;
		usesWeaponRange = true;
		_range = 7;
		rangedAttack = true;
		spawnsProjectile = true;
		drawsBlood = false;
		characterAnimation = ANIMATION_ATTACK;
		iconFrame = 2;
		overrideCursor = SPELL_HARM;
		_alwaysHits = true;
		sound_actionFrame = "spell_bolt";
		actionSoundEvenOnMiss = true;
		break;
	}
	case SKILL_BOW_FIRE:
	{
		skillname = "Bow Fire";
		powerCost = 10;
		damageType = EFFECT_PHYSICAL;
		damage = 0;
		addWeaponDamage = true;
		usesWeaponRange = true;
		requiresRanged = true;
		rangedAttack = true;
		spawnsProjectile = true;
		drawsBlood = true;
		characterAnimation = ANIMATION_ATTACK;
		iconFrame = 2;
		SetArrowSounds();
		break;
	}
	case SKILL_STRIKE:
	{
		skillname = "Strike";
		damage = 0;
		addWeaponDamage = true;
		monsterDamageScale = 1;
		iconFrame = 2;
		requiresMelee = true;
		sound_actionFrame = "impact_generic";
		sound_miss = "miss_swing";
		characterAnimation = ANIMATION_ATTACK;
		playCharacterAttackGrunt = true;
		aiPriority = 5;
		break;
	}
	case SKILL_WEAPON_SWEEP:
	{
		skillname = "Weapon Sweep";
		damage = 0;
		addWeaponDamage = true;
		scaleWeaponDamage = 1.2;
		friendlyFire = false;
		arcRadius = 1;
		powerCost = 50;
		iconFrame = 3;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		break;
	}
	case SKILL_HEAL:
	{
		skillname = "Heal";
		damageType = EFFECT_HOLY;
		_range = 5;
		directAttack = true;
		targetsFriendlies = true;
		_healing = 45;
		powerCost = 25;
		characterAnimation = ANIMATION_SPELL;
		hasTargetAnimation = true;
		iconFrame = 51;
		sound_actionFrame = "heal_little";
		break;
	}
	case SKILL_CURE:
	{
		skillname = "Cure";
		type = SKILL_TYPE_TARGETED;
		directAttack = true;
		_range = 3;
		targetsFriendlies = true;
		powerCost = 35;
		characterAnimation = ANIMATION_SPELL2;
		targetEffectId = EFFECT_REMOVE_NEGATIVE_EFFECTS;
		hasTargetAnimation = true;
		iconFrame = 97;
		sound_actionFrame = "heal_little";
		break;
	}
	case SKILL_REGENERATION:
	{
		skillname = "Regeneration";
		iconFrame = 98;
		sound_actionFrame = "heal_little";
		break;
	}
	case SKILL_MASS_REJUVENATION:
	{
		skillname = "Mass Rejuvenation";
		iconFrame = 99;
		sound_actionFrame = "heal_little";
		break;
	}
	case SKILL_GROUP_HEAL:
	{
		skillname = "Group Heal";
		damageType = EFFECT_HOLY;
		targetsFriendlies = true;
		hitsAllFriendlies = true;
		_healing = 35;
		powerCost = 60;
		characterAnimation = ANIMATION_SPELL2;
		hasTargetAnimation = true;
		iconFrame = 52;
		sound_actionFrame = "heal_big";
		break;
	}
	case SKILL_REVIVAL:
	{
		skillname = "Revival";
		damageType = EFFECT_HOLY;
		_range = 5;
		directAttack = true;
		targetsFriendlies = true;
		targetsDead = true;
		_healing = 30;
		resurrects = true;
		powerCost = 75;
		characterAnimation = ANIMATION_SPELL;
		hasTargetAnimation = true;
		iconFrame = 129;
		sound_actionFrame = "heal_big";
		break;
	}
	case SKILL_LIFEWARD:
	{
		type = SKILL_TYPE_TARGETED;
		skillname = "Life Ward";
		directAttack = true;
		_range = 4;
		powerCost = 50;
		iconFrame = 127;
		break;
	}
	case SKILL_CLEANSING_AURA:
	{
		skillname = "Cleansing Aura";
		type = SKILL_TYPE_TOGGLED;
		auraRadius = 4;
		powerCost = 40;
		iconFrame = 128;
		break;
	}
	case SKILL_LIGHTNING:
	{
		skillname = "Lightning";
		damageType = EFFECT_LIGHTNING;
		damage = 20;
		powerCost = 40;
		_range = 4;
		rangedAttack = true;
		spawnsBeam = true;
		drawsBlood = false;
		characterAnimation = ANIMATION_SPELL;
		iconFrame = 93;
		sound_actionFrame = "lightning";
		break;
	}
	case SKILL_SHOCK:
	{
		skillname = "Shock";
		_range = 3;
		rangedAttack = true;
		type = SKILL_TYPE_TARGETED;
		targetsTemplate = TEMPLATE_CONE;
		damageType = EFFECT_LIGHTNING;
		damage = 15;
		powerCost = 75;
		friendlyFire = true;
		characterAnimation = ANIMATION_SPELL;
		hasCasterAnimation = true;
		drawsBlood = false;
		iconFrame = 4;
		sound_actionFrame = "lightning";
		break;
	}
	case SKILL_TEMPEST:
	{
		skillname = "Tempest";
		type = SKILL_TYPE_TARGETED;
		damageType = EFFECT_LIGHTNING;
		_range = 3;
		iconFrame = 94;
		powerCost = 75;
		sound_actionFrame = "lightning";
		break;
	}
	case SKILL_CHAIN_LIGHTNING:
	{
		skillname = "Chain Lightning";
		type = SKILL_TYPE_TARGETED;
		damageType = EFFECT_LIGHTNING;
		_range = 4;
		powerCost = 90;
		iconFrame = 95;
		sound_actionFrame = "lightning";
		break;
	}
	case SKILL_FIREBALL:
	{
		skillname = "Fireball";
		damageType = EFFECT_FIRE;
		damage = 20;
		powerCost = 60;
		_range = 6;
		rangedAttack = true;
		targetsEmpty = true;
		areaEffectRadius = 1;
		friendlyFire = true;
		spawnsProjectile = true;
		drawsBlood = false;
		characterAnimation = ANIMATION_SPELL;
		iconFrame = 5;
		sound_begin = "fireball_cast";
		sound_impact = "fireball_impact";
		break;
	}
	case SKILL_FLAME_BLAST:
	{
		skillname = "Flame Blast";
		_range = 4;
		rangedAttack = true;
		type = SKILL_TYPE_TARGETED;
		targetsTemplate = TEMPLATE_HORIZONTAL4;
		damageType = EFFECT_FIRE;
		damage = 15;
		powerCost = 30;
		friendlyFire = true;
		characterAnimation = ANIMATION_SPELL;
		hasCasterAnimation = true;
		drawsBlood = false;
		iconFrame = 54;
		sound_actionFrame = "fire_blast";
		break;
	}
	case SKILL_FLAMING_WEAPON:
	{
		skillname = "Flaming Weapons";
		type = SKILL_TYPE_TOGGLED;
		targetsFriendlies = true;
		hitsAllFriendlies = true;
		doesNotEffectSelf = true;
		doesNotEffectMages = true;
		powerCost = 20;
		targetEffectId = EFFECT_FLAMING_WEAPON;
		characterAnimation = ANIMATION_SPELL2;
		iconFrame = 6;
		sound_begin = "fireball_cast";
		break;
	}
	case SKILL_INFERNO:
	{
		skillname = "Inferno";
		iconFrame = 96;
		type = SKILL_TYPE_TARGETED;
		damageType = EFFECT_FIRE;
		powerCost = 80;
		sound_begin = "fireball_cast";
		break;
	}
	case SKILL_SPELL_BOOST1:
	{
		skillname = "Spell Boost I";
		type = SKILL_TYPE_PASSIVE;
		passiveSpellpowerBonus = 5;
		iconFrame = 81;
		break;
	}
	case SKILL_SPELL_BOOST2:
	{
		skillname = "Spell Boost II";
		type = SKILL_TYPE_PASSIVE;
		passiveSpellpowerBonus = 5;
		iconFrame = 82;
		break;
	}
	case SKILL_SPELL_BOOST3:
	{
		skillname = "Spell Boost III";
		type = SKILL_TYPE_PASSIVE;
		passiveSpellpowerBonus = 5;
		iconFrame = 83;
		break;
	}
	case SKILL_SPELL_BOOST4:
	{
		skillname = "Spell Boost IV";
		type = SKILL_TYPE_PASSIVE;
		passiveSpellpowerBonus = 5;
		iconFrame = 84;
		break;
	}
	case SKILL_MELEE_ARCHER:
	{
		skillname = "Melee Archer";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 55;
		break;
	}
	case SKILL_MASTER_ARCHER:
	{
		skillname = "Master Archer";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 78;
		break;
	}
	case SKILL_AIM:
	{
		skillname = "Aim";
		_range = -1;
		requiresRanged = true;
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		powerCost = 15;
		toggleEndsTurn = false;
		selfEffectId = EFFECT_AIM;
		hasCasterAnimation = true;
		iconFrame = 28;
		break;
	}
	case SKILL_DEFENSIVE_FIRE:
	{
		skillname = "Defensive Fire";
		_range = -1;
		type = SKILL_TYPE_TOGGLED;
		toggleEndsTurn = false;
		targetsSelf = true;
		powerCost = 10;
		selfEffectId = EFFECT_DEFENSIVE_FIRE;
		requiresRanged = true;
		iconFrame = 56;
		break;
	}
	case SKILL_PINNING_SHOT:
	{
		skillname = "Pinning Shot";
		type = SKILL_TYPE_TARGETED;
		targetEffectId = EFFECT_PINNING_SHOT;
		damage = 0;
		addWeaponDamage = true;
		requiresRanged = true;
		rangedAttack = true;
		usesWeaponRange = true;
		powerCost = 40;
		iconFrame = 57;
		spawnsProjectile = true;
		drawsBlood = true;
		characterAnimation = ANIMATION_ATTACK;
		SetArrowSounds();
		break;
	}
	case SKILL_SUPPRESSING_FIRE:
	{
		skillname = "Suppressing Fire";
		type = SKILL_TYPE_TARGETED;
		damage = 0;
		_alwaysHits = true;
		requiresRanged = true;
		rangedAttack = true;
		usesWeaponRange = true;
		areaEffectRadius = 1;
		targetsEmpty = true;
		targetEffectId = EFFECT_SUPPRESSING_FIRE;
		powerCost = 30;
		iconFrame = 59;
		hasCasterAnimation = true;
		drawsBlood = false;
		characterAnimation = ANIMATION_SPECIAL1;
		spawnsProjectile = true;
		friendlyFire = false;
		targetsFriendlies = false;
		SetArrowSounds();
		break;
	}
	case SKILL_CRIPPLING_SHOT:
	{
		skillname = "Crippling Shot";
		iconFrame = 58;
		type = SKILL_TYPE_TARGETED;
		targetEffectId = EFFECT_CRIPPLING_SHOT;
		damage = 0;
		addWeaponDamage = true;
		requiresRanged = true;
		rangedAttack = true;
		usesWeaponRange = true;
		powerCost = 30;
		spawnsProjectile = true;
		drawsBlood = true;
		characterAnimation = ANIMATION_ATTACK;
		SetArrowSounds();
		break;
	}
	case SKILL_CRITICAL_SHOT:
	{
		skillname = "Critical Shot";
		iconFrame = 60;
		type = SKILL_TYPE_TARGETED;
		selfEffectId = EFFECT_TIRED_3;
		damage = 0;
		addWeaponDamage = true;
		automaticCritical = true;
		requiresRanged = true;
		rangedAttack = true;
		usesWeaponRange = true;
		powerCost = 50;
		spawnsProjectile = true;
		drawsBlood = true;
		characterAnimation = ANIMATION_ATTACK;
		SetArrowSounds();
		break;
	}
	case SKILL_THREATEN:
	{
		skillname = "Threaten";
		type = SKILL_TYPE_TOGGLED;
		toggleEndsTurn = false;
		targetsSelf = true;
		powerCost = 10;
		_range = -1;
		selfEffectId = EFFECT_THREATEN;
		iconFrame = 43;
		break;
	}
	case SKILL_TAUNT:
	{
		skillname = "Taunt";
		type = SKILL_TYPE_TARGETED;
		targetEffectId = EFFECT_TAUNT;
		rangedAttack = true;
		_range = 3;
		powerCost = 30;
		iconFrame = 42;
		hasCasterAnimation = true;
		characterAnimation = ANIMATION_SPELL;
		playCharacterAttackGrunt = true;
		ignoreActionFrame = true;
		break;
	}
	case SKILL_DISENGAGE:
	{
		skillname = "Disengage";
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		powerCost = 10;
		toggleEndsTurn = false;
		_range = -1;
		selfEffectId = EFFECT_DISENGAGE;
		iconFrame = 47;
		break;
	}
	case SKILL_PRECISE_STRIKING:
	{
		skillname = "Precise Striking";
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		requiresMelee = true;
		powerCost = 20;
		toggleEndsTurn = false;
		_range = -1;
		selfEffectId = EFFECT_PRECISE_STRIKING;
		iconFrame = 48;
		break;
	}
	case SKILL_PERFECT_STRIKE:
	{
		skillname = "Perfect Striking";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 49;
		break;
	}
	case SKILL_POWERFUL:
	{
		skillname = "Powerful";
		type = SKILL_TYPE_PASSIVE;
		passiveHealthMultiplier = 1.1;
		iconFrame = 44;
		break;
	}
	case SKILL_BRAVERY:
	{
		skillname = "Bravery";
		type = SKILL_TYPE_PASSIVE;
		passiveDamageMultiplier = 1.1;
		passivePhysicalResistance = 25;
		passiveMentalResistance = 25;
		iconFrame = 45;
		break;
	}
	case SKILL_DEATH_BLOW:
	{
		skillname = "Death Blow";
		type = SKILL_TYPE_PASSIVE;
		onTargetKillSelfEffectId = EFFECT_DEATH_BLOW;
		iconFrame = 46;
		break;
	}
	case SKILL_WAR_CRY:
	{
		skillname = "War Cry";
		type = SKILL_TYPE_INSTANT;
		pointBlankAE = true;
		directAttack = true;
		_range = 5;
		friendlyFire = false;
		targetEffectId = EFFECT_WAR_CRY;
		hasCasterAnimation = true;
		characterAnimation = ANIMATION_SPELL;
		playCharacterAttackGrunt = true;
		ignoreActionFrame = true;
		powerCost = 30;
		iconFrame = 53;
		break;
	}
	case SKILL_RALLY:
	{
		skillname = "Rally";
		type = SKILL_TYPE_TOGGLED;
		toggleEndsTurn = false;
		_range = -1;
		powerCost = 10;
		iconFrame = 112;
		auraEffectId = EFFECT_RALLY;
		targetsFriendlies = true;
		hasCasterAnimation = true;
		characterAnimation = ANIMATION_SPELL;
		playCharacterAttackGrunt = true;
		ignoreActionFrame = true;
		auraRadius = 3;
		powerCost = 20;
		break;
	}
	case SKILL_MOTIVATE:
	{
		skillname = "Motivate";
		iconFrame = 113;
		break;
	}
	case SKILL_SUPERIORITY:
	{
		skillname = "Superiority";
		iconFrame = 114;
		break;
	}
	case SKILL_BERSERK:
	{
		skillname = "Berserk";
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		powerCost = 10;
		_range = -1;
		toggleEndsTurn = false;
		selfEffectId = EFFECT_BERSERK;
		powerCost = 20;
		iconFrame = 7;
		break;
	}
	case SKILL_RESILIENCE:
	{
		skillname = "Resilience";
		type = SKILL_TYPE_PASSIVE;
		passivePhysicalResistance = 10;
		iconFrame = 66;
		break;
	}
	case SKILL_CONSTRAINT:
	{
		skillname = "Constraint";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 67;
		break;
	}
	case SKILL_FINAL_BLOW:
	{
		skillname = "Final Blow";
		type = SKILL_TYPE_TARGETED;
		requiresMelee = true;
		damage = 0;
		addWeaponDamage = true;
		costsRemainingPower = true;
		addsPowerToDamageRatio = 0.5;
		_range = 1;
		powerCost = 1;
		characterAnimation = ANIMATION_SPECIAL1;
		iconFrame = 8;
		break;
	}
	case SKILL_SHIELD_DEFENSE:
	{
		skillname = "Shield Defense";
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		powerCost = 10;
		selfEffectId = EFFECT_SHIELD_DEFENSE;
		requiresShield = true;
		iconFrame = 85;
		hasCasterAnimation = true;
		break;
	}
	case SKILL_SHIELD_BALANCE:
	{
		skillname = "Shield Balance";
		type = SKILL_TYPE_PASSIVE;
		requiresShield = true;
		iconFrame = 86;
		break;
	}
	case SKILL_SHIELD_WALL:
	{
		skillname = "Shield Wall";
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		powerCost = 35;
		requiresShield = true;
		iconFrame = 87;
		break;
	}
	case SKILL_SHIELD_EXPERTISE:
	{
		skillname = "Shield Expertise";
		type = SKILL_TYPE_PASSIVE;
		requiresShield = true;
		iconFrame = 88;
		break;
	}
	case SKILL_SHIELD_BLOCK:
	{
		skillname = "Shield Block";
		requiresShield = true;
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 89;
		break;
	}
	case SKILL_SHIELD_COVER:
	{
		skillname = "Shield Cover";
		type = SKILL_TYPE_PASSIVE;
		requiresShield = true;
		auraRadius = 1;
		auraEffectId = EFFECT_SHIELD_COVER;
		iconFrame = 90;
		break;
	}
	case SKILL_SHIELD_TACTICS:
	{
		skillname = "Shield Tactics";
		type = SKILL_TYPE_PASSIVE;
		requiresShield = true;
		iconFrame = 91;
		break;
	}
	case SKILL_SHIELD_MASTERY:
	{
		type = SKILL_TYPE_PASSIVE;
		skillname = "Shield Mastery";
		requiresShield = true;
		iconFrame = 92;
		break;
	}
	case SKILL_OVERPOWER:
	{
		skillname = "Overpower";
		powerCost = 20;
		iconFrame = 9;
		break;
	}
	case SKILL_INDOMITABLE:
	{
		skillname = "Indomitable";
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		powerCost = 10;
		selfEffectId = EFFECT_INDOMITABLE;
		iconFrame = 29;
		powerCost = 20;
		break;
	}
	case SKILL_STUNNING_BLOWS:
	{
		skillname = "Stunning Blows";
		type = SKILL_TYPE_PASSIVE;
		requiresTwoHander = true;
		iconFrame = 30;
		break;
	}
	case SKILL_CRITICAL_STRIKE:
	{
		skillname = "Critical Strike";
		requiresTwoHander = true;
		type = SKILL_TYPE_TARGETED;
		addWeaponDamage = true;
		usesWeaponRange = true;
		automaticCritical = true;
		powerCost = 90;
		iconFrame = 31;
		break;
	}
	case SKILL_SUNDER_ARMS:
	{
		skillname = "Sunder Arms";
		requiresTwoHander = true;
		iconFrame = 32;
		break;
	}
	case SKILL_SHATTERING_BLOWS:
	{
		skillname = "Shattering Blows";
		requiresTwoHander = true;
		iconFrame = 33;
		break;
	}
	case SKILL_DESTROYER:
	{
		skillname = "Destroyer";
		requiresTwoHander = true;
		iconFrame = 34;
		break;
	}
	case SKILL_MIGHTY_BLOW:
	{
		skillname = "Mighty Blow";
		requiresTwoHander = true;
		addWeaponDamage = true;
		type = SKILL_TYPE_TARGETED;
		attacksWithAccuracyMod = 10;
		scaleWeaponDamage = 1.2;
		targetEffectId = EFFECT_MIGHTY_BLOW;
		powerCost = 35;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_impact = "impact_generic";
		playCharacterAttackGrunt = true;
		iconFrame = 35;
		break;
	}
	case SKILL_POWERFUL_SWINGS:
	{
		skillname = "Powerful Swings";
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		powerCost = 10;
		selfEffectId = EFFECT_POWERFUL_SWINGS;
		requiresTwoHander = true;
		iconFrame = 115;
		powerCost = 20;
		break;
	}
	case SKILL_OVERRUN:
	{
		skillname = "Overrun";
		type = SKILL_TYPE_PASSIVE;
		onTargetKillSelfEffectId = EFFECT_OVERRUN;
		iconFrame = 10;
		break;
	}
	case SKILL_OLD_OVERRUN:
	{
		skillname = "Overrun";
		_range = -1;
		moveOnly = true;
		type = SKILL_TYPE_TARGETED;
		targetsEmpty = true;
		movementBonus = 3;
		disablesOwnCounterAttack = true;
		powerCost = 10;
		iconFrame = 10;
		break;
	}
	case SKILL_SUNDER_ARMOR:
	{
		skillname = "Sunder Armor";
		requiresTwoHander = true;
		powerCost = 20;
		iconFrame = 11;
		break;
	}
	case SKILL_BLIZZARD:
	{
		skillname = "Blizzard";
		powerCost = 20;
		iconFrame = 13;
		break;
	}
	case SKILL_DRAIN_LIFE:
	{
		skillname = "Drain Life";
		damageType = EFFECT_CURSE;
		damage = 20;
		directAttack = true;
		_range = 4;
		userConvertsDamageToHealthRatio = 0.5;
		hasTargetAnimation = true;
		powerCost = 50;
		iconFrame = 16;
		drawsBlood = false;
		characterAnimation = ANIMATION_SPELL2;
		sound_begin = "entropy_drain";
		break;
	}
	case SKILL_DEATH_MAGIC:
	{
		skillname = "Death Magic";
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		damageType = EFFECT_CURSE;
		powerCost = 10;
		selfEffectId = EFFECT_DEATH_MAGIC;
		iconFrame = 106;
		hasCasterAnimation = true;
		powerCost = 10;
		sound_begin = "death_magic";
		break;
	}
	case SKILL_CURSE_OF_MORTALITY:
	{
		skillname = "Curse of Mortality";
		sound_begin = "curse_of_mortality";
		damageType = EFFECT_CURSE;
		iconFrame = 126;
		break;
	}
	case SKILL_DEATH_CLOUD:
	{
		skillname = "Death Cloud";
		damageType = EFFECT_CURSE;
		powerCost = 20;
		iconFrame = 15;
		break;
	}
	case SKILL_MIND_BLAST:
	{
		skillname = "Mind Blast";
		damageType = EFFECT_MENTAL;
		rangedAttack = true;
		damage = 15;
		powerCost = 50;
		_range = 5;
		targetEffectId = EFFECT_MIND_BLAST;
		drawsBlood = false;
		spawnsProjectile = true;
		knockback = 1;
		hasCasterAnimation = true;
		characterAnimation = ANIMATION_SPELL;
		sound_actionFrame = "spirit_mind";
		iconFrame = 108;
		break;
	}
	case SKILL_FORCE_FIELD:
	{
		skillname = "Force Field";
		targetEffectId = EFFECT_FORCE_FIELD;
		damageType = EFFECT_MENTAL;
		directAttack = true;
		friendlyFire = true;
		targetsAllCharacters = true;
		_range = 6;
		drawsBlood = false;
		powerCost = 30;
		iconFrame = 111;
		hasCasterAnimation = true;
		characterAnimation = ANIMATION_SPELL;
		hasTargetAnimation = true;
		sound_actionFrame = "force_field";
		break;
	}
	case SKILL_TELEKENETIC_WEAPONS:
	{
		skillname = "Telekinetic Weapons";
		sound_actionFrame = "telekinetic_weapons";
		iconFrame = 110;
		break;
	}
	case SKILL_CRUSHING_PRISON:
	{
		skillname = "Crushing Prison";
		sound_actionFrame = "crushing_prison";
		damageType = EFFECT_MENTAL;
		iconFrame = 125;
		break;
	}
	case SKILL_BLOOD_MAGIC:
	{
		skillname = "Blood Magic";
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		powerCost = 0;
		_range = -1;
		damageType = EFFECT_BLOOD;
		toggleEndsTurn = false;
		selfEffectId = EFFECT_BLOOD_MAGIC;
		iconFrame = 68;
		break;
	}
	case SKILL_BLOOD_SACRIFICE:
	{
		skillname = "Blood Sacrifice";
		type = SKILL_TYPE_TARGETED;
		directAttack = true;
		damageType = EFFECT_BLOOD;
		_range = 6;
		targetsFriendlies = true;
		damage = 50;
		userConvertsDamageToHealthRatio = 1;
		powerCost = 20;
		iconFrame = 14;
		characterAnimation = ANIMATION_SPELL2;
		overrideCursor = SPELL_HARM;
		break;
	}
	case SKILL_BLOOD_WOUND:
	{
		skillname = "Blood Wound";
		type = SKILL_TYPE_TARGETED;
		rangedAttack = true;
		targetsEmpty = true;
		_range = 4;
		areaEffectRadius = 1;
		friendlyFire = false;
		damage = 80;
		powerCost = 100;
		damageType = EFFECT_BLOOD;
		targetEffectId = EFFECT_BLOOD_WOUND;
		iconFrame = 69;
		characterAnimation = ANIMATION_SPELL;
		break;
	}
	case SKILL_BLOOD_CONTROL:
	{
		skillname = "Blood Control";
		targetEffectId = EFFECT_BLOOD_CONTROL;
		iconFrame = 70;
		break;
	}
	case SKILL_MARK_OF_DEATH:
	{
		skillname = "Mark of Death";
		directAttack = true;
		_range = 6;
		damageType = EFFECT_CURSE;
		powerCost = 20;
		characterAnimation = ANIMATION_SPELL;
		targetEffectId = EFFECT_MARK_OF_DEATH;
		iconFrame = 71;
		break;
	}
	case SKILL_EXPLOIT_WEAKNESS:
	{
		skillname = "Exploit Weakness";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 72;
		break;
	}
	case SKILL_LACERATE:
	{
		skillname = "Lacerate";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 73;
		break;
	}
	case SKILL_FEAST_OF_THE_FALLEN:
	{
		skillname = "Feast of the Fallen";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 74;
		break;
	}
	case SKILL_BELOW_THE_BELT:
	{
		skillname = "Below the Belt";
		powerCost = 25;
		addWeaponDamage = true;
		requiresMelee = true;
		_range = 1;
		targetEffectId = EFFECT_BELOW_THE_BELT;
		iconFrame = 17;
		characterAnimation = ANIMATION_ATTACK;
		playCharacterAttackGrunt = true;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		break;
	}
	case SKILL_COMBAT_MOVEMENT:
	{
		skillname = "Combat Movement";
		type = SKILL_TYPE_PASSIVE;
		passiveMovementIncrease = 1;
		iconFrame = 79;
		break;
	}
	case SKILL_LETHALITY:
	{
		skillname = "Lethality";
		type = SKILL_TYPE_PASSIVE;
		passiveCriticalChanceIncrease = 4;
		iconFrame = 18;
		break;
	}
	case SKILL_COUP_DE_GRACE:
	{
		skillname = "Coup de Grace";
		type = SKILL_TYPE_TARGETED;
		targetsStunned = true;
		iconFrame = 19;
		break;
	}
	case SKILL_DEADLY_STRIKE:
	{
		skillname = "Deadly Strike";
		type = SKILL_TYPE_TARGETED;
		powerCost = 30;
		requiresMelee = true;
		addWeaponDamage = true;
		usesWeaponRange = true;
		armorPenetrationBonus = 10;
		cannotBeCountered = true;
		characterAnimation = ANIMATION_ATTACK;
		playCharacterAttackGrunt = true;
		iconFrame = 21;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		break;
	}
	case SKILL_DIRTY_FIGHTING:
	{
		skillname = "Dirty Fighting";
		requiresMelee = true;
		type = SKILL_TYPE_TARGETED;
		powerCost = 40;
		targetEffectId = EFFECT_DIRTY_FIGHTING;
		_range = 1;
		characterAnimation = ANIMATION_ATTACK;
		iconFrame = 22;
		characterAnimation = ANIMATION_ATTACK;
		sound_begin = "swing_edged";
		sound_actionFrame = "swing_edged";
		playCharacterAttackGrunt = true;
		break;
	}
	case SKILL_EVASION:
	{
		skillname = "Evasion";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 80;
		break;
	}
	case SKILL_FEIGN_DEATH:
	{
		skillname = "Feign Death";
		iconFrame = 23;
		break;
	}
	case SKILL_STEALTH1:
	{
		skillname = "Stealth";
		type = SKILL_TYPE_TOGGLED;
		powerCost = 20;
		_range = -1;
		toggleEndsTurn = false;
		targetsSelf = true;
		selfEffectId = EFFECT_STEALTH;
		requiresMediumArmor = true;
		_minimumEnemyDistance = 5;
		powerCost = 20;
		iconFrame = 41;
		break;
	}
	case SKILL_STEALTH2:
	{
		skillname = "Improved Stealth";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 38;
		break;
	}
	case SKILL_STEALTH3:
	{
		skillname = "Advanced Stealth";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 39;
		break;
	}
	case SKILL_STEALTH4:
	{
		skillname = "Master Stealth";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 40;
		break;
	}
	case SKILL_DUAL_DOUBLE_STRIKE:
	{
		skillname = "Double Striking";
		requiresDualWield = true;
		selfEffectId = EFFECT_DUAL_DOUBLE_STRIKE;
		type = SKILL_TYPE_TOGGLED;
		targetsSelf = true;
		powerCost = 16;
		_range = -1;
		toggleEndsTurn = false;
		iconFrame = 24;
		break;
	}
	case SKILL_DUAL_RIPOSTE:
	{
		skillname = "Riposte";
		characterAnimation = ANIMATION_SPECIAL1;
		requiresDualWield = true;
		addWeaponDamage = true;
		enablesOwnCounterAttack = true;
		powerCost = 30;
		iconFrame = 26;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		break;
	}
	case SKILL_DUAL_CRIPPLE:
	{
		skillname = "Cripple";
		targetEffectId = EFFECT_DUAL_CRIPPLE;
		requiresDualWield = true;
		addWeaponDamage = true;
		characterAnimation = ANIMATION_ATTACK;
		playCharacterAttackGrunt = true;
		powerCost = 30;
		iconFrame = 25;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		break;
	}
	case SKILL_PUNISHER:
	{
		skillname = "Punisher";
		requiresDualWield = true;
		addWeaponDamage = true;
		attacksMultipleTimes = 3;
		characterAnimation = ANIMATION_SPECIAL1;
		accelerateAnimation = true;
		powerCost = 80;
		iconFrame = 27;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		playCharacterAttackGrunt = true;
		break;
	}
	case SKILL_DUAL_WEAPON_SWEEP:
	{
		skillname = "Dual Weapon Sweep";
		requiresDualWield = true;
		damage = 0;
		addWeaponDamage = true;
		scaleWeaponDamage = 1.2;
		friendlyFire = false;
		arcRadius = 1;
		powerCost = 40;
		iconFrame = 50;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		playCharacterAttackGrunt = true;
		break;
	}
	case SKILL_DUAL_WEAPON_FLURRY:
	{
		skillname = "Flurry";
		requiresDualWield = true;
		iconFrame = 75;
		damage = 10;
		addWeaponDamage = true;
		scaleWeaponDamage = 1.4;
		characterAnimation = ANIMATION_SPECIAL1;
		powerCost = 60;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		playCharacterAttackGrunt = true;
		break;
	}
	case SKILL_DUAL_WEAPON_MOMENTUM:
	{
		skillname = "Momentum";
		type = SKILL_TYPE_PASSIVE;
		onTargetHitSelfEffectId = EFFECT_MOMENTUM;
		iconFrame = 76;
		break;
	}
	case SKILL_WHIRLWIND:
	{
		skillname = "Whirlwind";
		type = SKILL_TYPE_TARGETED;
		damage = 30;
		addWeaponDamage = true;
		_range = 1;
		friendlyFire = false;
		pointBlankAE = true;
		powerCost = 80;
		iconFrame = 77;
		break;
	}
	case SKILL_DISTRACTION:
	{
		skillname = "Distraction";
		_range = 5;
		type = SKILL_TYPE_TARGETED;
		directAttack = true;
		targetEffectId = EFFECT_DISTRACTION;
		characterAnimation = ANIMATION_SPELL;
		hasCasterAnimation = true;
		hasTargetAnimation = true;
		ignoreActionFrame = true;
		sound_actionFrame = "bard_song";
		powerCost = 50;
		iconFrame = 63;
		break;
	}
	case SKILL_SONG_OF_VALOR:
	{
		skillname = "Song of Valor";
		type = SKILL_TYPE_TOGGLED;
		_range = -1;
		toggleEndsTurn = false;
		hitsAllFriendlies = true;
		targetsFriendlies = true;
		untogglesSongs = true;
		targetEffectId = EFFECT_SONG_VALOR;
		characterAnimation = ANIMATION_SPELL;
		hasCasterAnimation = true;
		hasTargetAnimation = true;
		ignoreActionFrame = true;
		sound_actionFrame = "bard_song";
		powerCost = 30;
		iconFrame = 65;
		break;
	}
	case SKILL_SONG_OF_COURAGE:
	{
		skillname = "Song of Courage";
		type = SKILL_TYPE_TOGGLED;
		_range = -1;
		toggleEndsTurn = false;
		hitsAllFriendlies = true;
		targetsFriendlies = true;
		untogglesSongs = true;
		sound_actionFrame = "bard_song";
		targetEffectId = EFFECT_SONG_COURAGE;
		characterAnimation = ANIMATION_SPELL;
		powerCost = 80;
		iconFrame = 62;
		break;
	}
	case SKILL_CAPTIVATE:
	{
		skillname = "Captivating Song";
		type = SKILL_TYPE_INSTANT;
		pointBlankAE = true;
		directAttack = true;
		_range = 3;
		untogglesSongs = true;
		sound_actionFrame = "bard_song";
		targetEffectId = EFFECT_SONG_CAPTIVATE;
		characterAnimation = ANIMATION_SPELL;
		powerCost = 80;
		iconFrame = 64;
		break;
	}
	case SKILL_DWARF_RACIAL:
	{
		skillname = "Race: Dwarf";
		type = SKILL_TYPE_PASSIVE;
		if (char_ref->gender == GENDER_MALE)
		{
			iconFrame = 100;
		}
		else
		{
			iconFrame = 101;
		}
		break;
	}
	case SKILL_HUMAN_RACIAL:
	{
		skillname = "Race: Human";
		type = SKILL_TYPE_PASSIVE;
		if (char_ref->gender == GENDER_MALE)
		{
			iconFrame = 102;
		}
		else
		{
			iconFrame = 103;
		}
		break;
	}
	case SKILL_ELF_RACIAL:
	{
		skillname = "Race: Elf";
		type = SKILL_TYPE_PASSIVE;
		if (char_ref->gender == GENDER_MALE)
		{
			iconFrame = 104;
		}
		else
		{
			iconFrame = 105;
		}
		break;
	}
	case SKILL_GREY_WARDEN:
	{
		skillname = "Grey Warden";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 130;
		break;
	}
	case SKILL_ORIGIN_HUMAN_NOBLE:
	{
		skillname = "Origin: Human Noble";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 116;
		break;
	}
	case SKILL_ORIGIN_MAGE:
	{
		skillname = "Origin: Mage";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 117;
		break;
	}
	case SKILL_ORIGIN_ELF_DALISH:
	{
		skillname = "Origin: Dalish Elf";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 118;
		break;
	}
	case SKILL_ORIGIN_ELF_CITY:
	{
		skillname = "Origin: City Elf";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 119;
		break;
	}
	case SKILL_ORIGIN_DWARF_NOBLE:
	{
		skillname = "Origin: Dwarf Noble";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 120;
		break;
	}
	case SKILL_ORIGIN_DWARF_COMMONER:
	{
		skillname = "Origin: Dwarf Commoner";
		type = SKILL_TYPE_PASSIVE;
		iconFrame = 121;
		break;
	}
	case SKILL_HURLOCK_BLADES:
	{
		skillname = "Hurlock Blades";
		monsterDamageScale = 1.1;
		powerCost = 40;
		targetEffectId = EFFECT_HURLOCK_DEEP_WOUNDS;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		characterAnimation = ANIMATION_SPECIAL1;
		playCharacterAttackGrunt = true;
		break;
	}
	case SKILL_HURLOCK_KICK:
	{
		skillname = "Kick";
		monsterDamageScale = 0.9;
		powerCost = 25;
		requiresHorizontalLine = true;
		knockback = 1;
		sound_actionFrame = "impact_generic";
		characterAnimation = ANIMATION_SPECIAL2;
		playCharacterAttackGrunt = true;
		break;
	}
	case SKILL_GENLOCK_SHIELD:
	{
		skillname = "Shield";
		type = SKILL_TYPE_INSTANT;
		aiNotAlone = true;
		aiDesireAllyProximity = true;
		targetsSelf = true;
		enablesOwnCounterAttack = true;
		powerCost = 20;
		selfEffectId = EFFECT_GENLOCK_SHIELD_SELF;
		characterAnimation = ANIMATION_SPECIAL2;
		sound_actionFrame = "genlock_shield";
		iconFrame = 85;
		aiPriority = 1;
		break;
	}
	case SKILL_GENLOCK_TRAP:
	{
		skillname = "Trap";
		monsterDamageScale = 0.5;
		targetsEmpty = true;
		_range = 1;
		powerCost = 50;
		aiCastLimit = 2;
		aiPriority = 2;
		spawnsTrap = true;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_actionFrame = "genlock_trap";
		break;
	}
	case SKILL_GENLOCK_ACID:
	{
		skillname = "Acid Flask";
		rangedAttack = true;
		powerCost = 50;
		monsterDamageScale = 1.1;
		damageType = EFFECT_POISON;
		targetEffectId = EFFECT_GENLOCK_ACID_VIAL;
		_range = 4;
		_minimumRange = 2;
		_alwaysHits = true;
		drawsBlood = false;
		spawnsProjectile = true;
		areaEffectRadius = 1;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_actionFrame = "throw_woosh";
		sound_impact = "genlock_acid";
		break;
	}
	case SKILL_GENLOCK_MELEE:
	{
		skillname = "Crossbow Smack";
		_range = 1;
		monsterDamageScale = 0.8;
		drawsBlood = true;
		characterAnimation = ANIMATION_SPECIAL2;
		playCharacterAttackGrunt = true;
		break;
	}
	case SKILL_ALPHA_STRIKE:
	{
		skillname = "Alpha Strike";
		_range = 1;
		monsterDamageScale = 1;
		iconFrame = 12;
		arcRadius = 1;
		friendlyFire = false;
		sound_actionFrame = "impact_generic";
		characterAnimation = ANIMATION_ATTACK;
		playCharacterAttackGrunt = true;
		aiPriority = 5;
		break;
	}
	case SKILL_ALPHA_SPEAR:
	{
		skillname = "Alpha Spear";
		_range = 1;
		aiOnlyWhenNearlyDead = true;
		monsterDamageScale = 1;
		powerCost = 20;
		automaticCritical = true;
		drawsBlood = true;
		characterAnimation = ANIMATION_SPECIAL1;
		playCharacterAttackGrunt = true;
		break;
	}
	case SKILL_GENLOCK_ARTILLERY:
	{
		skillname = "Genlock Artillery";
		_range = 10;
		powerCost = 75;
		directAttack = true;
		_alwaysHits = true;
		monsterDamageScale = 1.2;
		areaEffectRadius = 2;
		drawsBlood = true;
		characterAnimation = ANIMATION_SPECIAL2;
		playCharacterAttackGrunt = true;
		sound_impact = "genlock_artillery";
		sound_miss = sound_impact;
		spawnsProjectile = true;
		break;
	}
	case SKILL_HURLOCK_XBOW_MELEE:
	{
		skillname = "Crossbow Smack";
		_range = 1;
		monsterDamageScale = 0.8;
		addWeaponDamage = true;
		drawsBlood = true;
		characterAnimation = ANIMATION_SPECIAL1;
		playCharacterAttackGrunt = true;
		actionSoundEvenOnMiss = true;
		break;
	}
	case SKILL_HURLOCK_SNIPE:
	{
		skillname = "Hurlock Snipe";
		monsterDamageScale = 0.7;
		powerCost = 50;
		damageType = EFFECT_PHYSICAL;
		_minimumRange = 6;
		_range = 10;
		targetEffectId = EFFECT_HEADSHOT;
		rangedAttack = true;
		spawnsProjectile = true;
		drawsBlood = true;
		characterAnimation = ANIMATION_SPECIAL2;
		SetArrowSounds();
		break;
	}
	case SKILL_OGRE_ROCK:
	{
		skillname = "Rock Throw";
		monsterDamageScale = 0.7;
		damageType = EFFECT_PHYSICAL;
		powerCost = 30;
		_range = 10;
		rangedAttack = true;
		spawnsProjectile = true;
		_minimumRange = 4;
		drawsBlood = true;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_begin = "ogre_rock";
		sound_impact = "ogre_stone_impact";
		sound_miss = "ogre_stone_impact";
		aiPriority = 3;
		break;
	}
	case SKILL_OGRE_SMASH:
	{
		skillname = "Ogre Smash";
		monsterDamageScale = 1;
		arcRadius = 1;
		sound_impact = "ogre_shockwave";
		sound_miss = "ogre_shockwave";
		characterAnimation = ANIMATION_ATTACK;
		playCharacterAttackGrunt = true;
		friendlyFire = false;
		doesNotEffectSelf = true;
		aiPriority = 5;
		break;
	}
	case SKILL_OGRE_GRAB:
	{
		skillname = "Ogre Grab";
		monsterDamageScale = 9000;
		requiresHorizontalLine = true;
		powerCost = 50;
		drawsBlood = false;
		characterAnimation = ANIMATION_SPECIAL2;
		_alwaysHits = true;
		_range = 1;
		sound_begin = "ogre_smash";
		sound_actionFrame = "ogre_shockwave";
		sound_miss = "ogre_shockwave";
		aiPriority = 9;
		aiTargetOnlyLowHealth = true;
		break;
	}
	case SKILL_SHRIEK_LEAP:
	{
		skillname = "Leap";
		monsterDamageScale = 1;
		rangedAttack = true;
		_range = 6;
		_minimumRange = 3;
		powerCost = 40;
		specialMovementSkill = true;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_begin = "shriek_leap";
		sound_actionFrame = "impact_generic";
		playCharacterAttackGrunt = true;
		aiPriority = 1;
		break;
	}
	case SKILL_SHRIEK_SCREAM:
	{
		skillname = "Shriek";
		_range = 3;
		pointBlankAE = true;
		_minimumEnemyDistance = 2;
		friendlyFire = false;
		targetEffectId = EFFECT_SHRIEK_SCREAM;
		characterAnimation = ANIMATION_SPECIAL2;
		sound_actionFrame = "shriek_scream";
		break;
	}
	case SKILL_DARKSPAWN_CROSSBOW:
	{
		skillname = "Darkspawn Crossbow";
		monsterDamageScale = 0.6;
		damageType = EFFECT_PHYSICAL;
		_range = 8;
		_minimumRange = 3;
		armorPenetrationBonus = 5;
		rangedAttack = true;
		spawnsProjectile = true;
		drawsBlood = true;
		armorPenetrationBonus = 4;
		characterAnimation = ANIMATION_ATTACK;
		SetArrowSounds();
		break;
	}
	case SKILL_SPIDER_POISON:
	{
		skillname = "Spider Poison";
		monsterDamageScale = 1;
		damageType = EFFECT_POISON;
		_range = 1;
		drawsBlood = true;
		targetEffectId = EFFECT_SPIDER_POISON;
		powerCost = 25;
		characterAnimation = ANIMATION_SPECIAL1;
		playCharacterAttackGrunt = true;
		aiPriority = 8;
		break;
	}
	case SKILL_SPIDER_WEB:
	{
		skillname = "Spider Web";
		_range = 10;
		rangedAttack = true;
		_alwaysHits = true;
		damage = 0;
		spawnsBeam = true;
		targetEffectId = EFFECT_SPIDER_WEB;
		powerCost = 50;
		characterAnimation = ANIMATION_SPECIAL2;
		playCharacterAttackGrunt = true;
		spawnsProjectile = true;
		_minimumRange = 2;
		aiPriority = 9;
		playCharacterAttackGrunt = true;
		actionSoundEvenOnMiss = true;
		sound_actionFrame = "spider_web_throw";
		sound_impact = "spider_web_impact";
		sound_miss = "miss_silent";
		break;
	}
	case SKILL_SPIDER_STRIKE:
	{
		skillname = "Spider Strike";
		_range = 1;
		monsterDamageScale = 1;
		critsAgainstTargetsWithEffectId = EFFECT_SPIDER_WEB;
		sound_actionFrame = "impact_generic";
		characterAnimation = ANIMATION_ATTACK;
		playCharacterAttackGrunt = true;
		aiPriority = 8;
		break;
	}
	case SKILL_STALKER_SPIT:
	{
		skillname = "Spit";
		_range = 4;
		_minimumRange = 2;
		rangedAttack = true;
		spawnsProjectile = true;
		monsterDamageScale = 0.4;
		targetEffectId = EFFECT_CORROSIVE_SPIT;
		powerCost = 20;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_begin = "deep_stalker_attack_3";
		sound_impact = "deep_stalker_spit";
		aiPriority = 1;
		break;
	}
	case SKILL_STALKER_LEAP:
	{
		skillname = "Pounce";
		_range = 5;
		_minimumRange = 5;
		rangedAttack = true;
		specialMovementSkill = true;
		requiresHorizontalLine = true;
		monsterDamageScale = 1.5;
		powerCost = 40;
		characterAnimation = ANIMATION_SPECIAL2;
		playCharacterAttackGrunt = true;
		aiPriority = 6;
		break;
	}
	case SKILL_STALKER_BABY_LEAP:
	{
		skillname = "Pounce";
		_range = 2;
		_minimumRange = 2;
		rangedAttack = true;
		specialMovementSkill = true;
		requiresHorizontalLine = true;
		monsterDamageScale = 1.5;
		powerCost = 40;
		characterAnimation = ANIMATION_SPECIAL2;
		playCharacterAttackGrunt = true;
		aiPriority = 6;
		break;
	}
	case SKILL_BANDIT_KNIVES:
	{
		skillname = "Throwing Daggers";
		rangedAttack = true;
		_range = 5;
		powerCost = 20;
		monsterDamageScale = 0.5;
		_minimumRange = 2;
		spawnsProjectile = true;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_begin = "bandit_begin_throw";
		sound_actionFrame = "bandit_throw";
		sound_impact = "bandit_throw_impact";
		aiPriority = 2;
		break;
	}
	case SKILL_BANDIT_BACKSTAB:
	{
		skillname = "Bandit Backstab";
		monsterDamageScale = 1;
		automaticCritical = true;
		_alwaysHits = true;
		_range = 1;
		requiresHorizontalLine = true;
		requiresBehindTarget = true;
		suppressTargetFacing = true;
		cannotBeCountered = true;
		characterAnimation = ANIMATION_SPECIAL2;
		adjustDepthToBehind = true;
		playCharacterAttackGrunt = true;
		sound_begin = "swing_edged";
		sound_actionFrame = "impact_generic";
		aiPriority = 9;
		break;
	}
	case SKILL_BRIGAND_DIZZY:
	{
		skillname = "Dizzying Blow";
		monsterDamageScale = 0.8;
		_range = 1;
		_powerDelta = -25;
		targetEffectId = EFFECT_DIZZY;
		characterAnimation = ANIMATION_SPECIAL1;
		playCharacterAttackGrunt = true;
		sound_actionFrame = "impact_generic";
		aiPriority = 7;
		break;
	}
	case SKILL_EMISSARY_BOLT:
	{
		skillname = "Emissary Bolt";
		powerCost = 0;
		damageType = EFFECT_CURSE;
		monsterDamageScale = 1;
		_minimumRange = 2;
		_range = 6;
		rangedAttack = true;
		spawnsProjectile = true;
		drawsBlood = false;
		characterAnimation = ANIMATION_SPECIAL2;
		_alwaysHits = true;
		sound_actionFrame = "emissary_bolt";
		actionSoundEvenOnMiss = true;
		break;
	}
	case SKILL_EMISSARY_PLOT_BOLT:
	{
		skillname = "Plot Bolt";
		powerCost = 20;
		damageType = EFFECT_CURSE;
		monsterDamageScale = 9000;
		_range = 15;
		rangedAttack = true;
		spawnsProjectile = true;
		drawsBlood = false;
		characterAnimation = ANIMATION_SPECIAL2;
		sound_actionFrame = "emissary_bolt";
		_alwaysHits = true;
		break;
	}
	case SKILL_EMISSARY_SHOCKWAVE:
	{
		skillname = "Emissary Shockwave";
		powerCost = 20;
		damageType = EFFECT_PHYSICAL;
		monsterDamageScale = 0.6;
		pointBlankAE = true;
		doesNotEffectSelf = true;
		friendlyFire = false;
		_range = 1;
		knockback = 1;
		targetEffectId = EFFECT_SHOCKWAVE;
		aiNotWithMove = true;
		drawsBlood = false;
		_alwaysHits = true;
		characterAnimation = ANIMATION_ATTACK;
		playCharacterAttackGrunt = true;
		aiPriority = 10;
		sound_actionFrame = "emissary_shockwave";
		actionSoundEvenOnMiss = true;
		break;
	}
	case SKILL_EMISSARY_PLOT_DRAIN:
	{
		skillname = "Plot Leech Vitality";
		directAttack = true;
		powerCost = 10;
		monsterDamageScale = 9000;
		_alwaysHits = true;
		drawsBlood = false;
		characterAnimation = ANIMATION_SPECIAL1;
		hasTargetAnimation = true;
		sound_begin = "emissary_drain";
		break;
	}
	case SKILL_EMISSARY_DRAIN:
	{
		skillname = "Leech Vitality";
		damageType = EFFECT_CURSE;
		targetEffectId = EFFECT_WEAKNESS;
		directAttack = true;
		monsterDamageScale = 0.8;
		_range = 5;
		userConvertsDamageToHealthRatio = 0.6;
		powerCost = 50;
		iconFrame = 16;
		_alwaysHits = true;
		drawsBlood = false;
		characterAnimation = ANIMATION_SPECIAL1;
		hasTargetAnimation = true;
		aiNotUnlessHurt = true;
		sound_begin = "emissary_drain";
		break;
	}
	case SKILL_EMISSARY_ESCAPE:
	{
		skillname = "Flash Escape";
		powerCost = 0;
		targetsSelf = true;
		aiOnlyWhenNearlyDead = true;
		aiNotWithMove = true;
		aiPriority = 9999;
		characterAnimation = ANIMATION_SPECIAL1;
		sound_begin = "emissary_escape";
		break;
	}
	}
	if (type == SKILL_TYPE_PASSIVE)
	{
		_range = -1;
	}
}

void USkill::SetArrowSounds()
{
	playCharacterAttackGrunt = false;
	actionSoundEvenOnMiss = true;
	sound_begin = "arrow_load";
	sound_actionFrame = "arrow_fire";
	sound_miss = "miss_silent";
}

bool USkill::AiFlagsOkay()
{
	if (aiCastLimit != -1 && aiCastCount > aiCastLimit)
	{
		return false;
	}

	if (aiNotUnlessHurt && char_ref->health >= char_ref->GetHealthMax())
	{
		return false;
	}

	if (aiNotUnlessLowPower && char_ref->GetPowerPercent() > 25)
	{
		return false;
	}

	if (aiOnlyWhenNearlyDead && char_ref->GetHealthPercent() > 10)
	{
		return false;
	}

	if (aiNotAlone)
	{
		if (char_ref->group->chars.Num() == 1)
		{
			return false;
		}

		int32 nTeamAlive = 0;
		if (GetMain()->Battlefield)
		{
			for (int32 i = 0; i < char_ref->group->chars.Num(); i++)
			{
				UGameCharacter* loc2 = char_ref->group->chars[i];
				if (loc2 && loc2->health > 0 && loc2 != char_ref)
				{
					++nTeamAlive;
				}
			}

			if (nTeamAlive == 0)
			{
				return false;
			}
		}
	}

	if (!aiAllowSuicide && targetsSelf)
	{
		if (damage >= char_ref->health)
		{
			return false;
		}
	}

	return true;
}
