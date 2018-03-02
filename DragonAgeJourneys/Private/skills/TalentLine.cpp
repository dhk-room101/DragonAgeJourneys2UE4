#include "TalentLine.h"
#include "Skills.h"
#include "Classes.h"
#include "GameCharacter.h"

UTalentLine::UTalentLine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UTalentLine::InitializeTalentLine(int32 nTreeId, int32 nUnlocked)
{
	treeId = nTreeId;

	SetTalentLineProperties();

	if ((nTreeId >= 500) && (nUnlocked == -1))
	{
		unlocked = skillIds.Num();
	}
	else if (type == TALENT_TYPE_ORIGIN)
	{
		unlocked = skillIds.Num();
	}
	else if (type == TALENT_TYPE_RACIAL)
	{
		unlocked = skillIds.Num();
	}
	else
	{
		unlocked = nUnlocked;
	}
}

FString UTalentLine::GetLineTitle(int32 nTreeId, UGameCharacter* gameCharacter)
{
	switch (nTreeId)
	{
	case TALENT_TYPE_ARCHERY:
	{
		return "Archery";
	}
	case TALENT_TYPE_DUAL_WEAPON:
	{
		return "Dual Weapon";
	}
	case TALENT_TYPE_TWO_HANDED:
	{
		return "Two-Handed";
	}
	case TALENT_TYPE_SHIELD:
	{
		return "Weapon and Shield";
	}
	case TALENT_TYPE_RACIAL:
	{
		return "Racial Bonuses";
	}
	case TALENT_TYPE_ORIGIN:
	{
		return "Background";
	}
	case TALENT_TYPE_SPECIALIZATION:
	{
		return GetNameForClass(gameCharacter->classId);/*gameCharacter->className*/
	}
	case TALENT_TYPE_CLASS:
	{
		return GetNameForClass(GetBaseClassOf(gameCharacter->classId));
	}
	case TALENT_TYPE_PRIMAL:
	{
		return "Primal";
	}
	case TALENT_TYPE_CREATION:
	{
		return "Creation";
	}
	case TALENT_TYPE_SPIRIT:
	{
		return "Spirit";
	}
	case TALENT_TYPE_ENTROPY:
	{
		return "Entropy";
	}
	default:
	{
		break;
	}
	}

	return "Unknown";
}

UTalentLine::~UTalentLine()
{
}

void UTalentLine::SetTalentLineProperties()
{
	switch (treeId)
	{
	case TALENT_ELF_RACIAL:
	{
		skillIds.Add(SKILL_ELF_RACIAL);
		type = TALENT_TYPE_RACIAL;
		break;
	}
	case TALENT_DWARF_RACIAL:
	{
		skillIds.Add(SKILL_DWARF_RACIAL);
		type = TALENT_TYPE_RACIAL;
		break;
	}
	case TALENT_HUMAN_RACIAL:
	{
		skillIds.Add(SKILL_HUMAN_RACIAL);
		type = TALENT_TYPE_RACIAL;
		break;
	}
	case TALENT_GREY_WARDEN:
	{
		skillIds.Add(SKILL_GREY_WARDEN);
		type = TALENT_TYPE_ORIGIN;
		break;
	}
	case TALENT_ORIGIN_DWARF_COMMONER:
	{
		skillIds.Add(SKILL_ORIGIN_DWARF_COMMONER);
		type = TALENT_TYPE_ORIGIN;
		break;
	}
	case TALENT_ORIGIN_DWARF_NOBLE:
	{
		skillIds.Add(SKILL_ORIGIN_DWARF_NOBLE);
		type = TALENT_TYPE_ORIGIN;
		break;
	}
	case TALENT_ORIGIN_MAGE:
	{
		skillIds.Add(SKILL_ORIGIN_MAGE);
		type = TALENT_TYPE_ORIGIN;
		break;
	}
	case TALENT_ORIGIN_HUMAN_NOBLE:
	{
		skillIds.Add(SKILL_ORIGIN_HUMAN_NOBLE);
		type = TALENT_TYPE_ORIGIN;
		break;
	}
	case TALENT_ORIGIN_ELF_CITY:
	{
		skillIds.Add(SKILL_ORIGIN_ELF_CITY);
		type = TALENT_TYPE_ORIGIN;
		break;
	}
	case TALENT_ORIGIN_ELF_DALISH:
	{
		skillIds.Add(SKILL_ORIGIN_ELF_DALISH);
		type = TALENT_TYPE_ORIGIN;
		break;
	}
	case TALENT_MAGE_POWER:
	{
		skillIds.Add(SKILL_SPELL_BOOST1); skillIds.Add(SKILL_SPELL_BOOST2); skillIds.Add(SKILL_SPELL_BOOST3); skillIds.Add(SKILL_SPELL_BOOST4);
		type = TALENT_TYPE_CLASS;
		break;
	}
	case TALENT_MAGE_FIRE:
	{
		skillIds.Add(SKILL_FLAME_BLAST); skillIds.Add(SKILL_FLAMING_WEAPONS); skillIds.Add(SKILL_FIREBALL); skillIds.Add(SKILL_INFERNO);
		type = TALENT_TYPE_PRIMAL;
		break;
	}
	case TALENT_MAGE_COLD:
	{
		skillIds.Add(SKILL_WINTERS_GRASP); skillIds.Add(SKILL_FROSTWALL); skillIds.Add(SKILL_CONE_OF_COLD); skillIds.Add(SKILL_BLIZZARD);
		type = TALENT_TYPE_PRIMAL;
		break;
	}
	case TALENT_MAGE_LIGHTNING:
	{
		skillIds.Add(SKILL_LIGHTNING); skillIds.Add(SKILL_SHOCK); skillIds.Add(SKILL_TEMPEST); skillIds.Add(SKILL_CHAIN_LIGHTNING);
		type = TALENT_TYPE_PRIMAL;
		break;
	}
	case TALENT_MAGE_STONE:
	{
		skillIds.Add(SKILL_ROCK_ARMOR); skillIds.Add(SKILL_STONEFIST); skillIds.Add(SKILL_EARTHQUAKE); skillIds.Add(SKILL_PETRIFY);
		type = TALENT_TYPE_PRIMAL;
		break;
	}
	case TALENT_MAGE_HEAL:
	{
		skillIds.Add(SKILL_HEAL); skillIds.Add(SKILL_CURE); skillIds.Add(SKILL_REGENERATION); skillIds.Add(SKILL_MASS_REJUVENATION);
		type = TALENT_TYPE_CREATION;
		break;
	}
	case TALENT_MAGE_DEATH:
	{
		skillIds.Add(SKILL_DRAIN_LIFE); skillIds.Add(SKILL_DEATH_MAGIC); skillIds.Add(SKILL_CURSE_OF_MORTALITY); skillIds.Add(SKILL_DEATH_CLOUD);
		type = TALENT_TYPE_ENTROPY;
		break;
	}
	case TALENT_MAGE_PSIONICS:
	{
		skillIds.Add(SKILL_MIND_BLAST); skillIds.Add(SKILL_FORCE_FIELD); skillIds.Add(SKILL_TELEKENETIC_WEAPONS); skillIds.Add(SKILL_CRUSHING_PRISON);
		type = TALENT_TYPE_SPIRIT;
		break;
	}
	case TALENT_ROGUE1:
	{
		skillIds.Add(SKILL_BELOW_THE_BELT); skillIds.Add(SKILL_COMBAT_MOVEMENT); skillIds.Add(SKILL_LETHALITY); skillIds.Add(SKILL_COUP_DE_GRACE);
		type = TALENT_TYPE_CLASS;
		break;
	}
	case TALENT_ROGUE2:
	{
		skillIds.Add(SKILL_DEADLY_STRIKE); skillIds.Add(SKILL_DIRTY_FIGHTING); skillIds.Add(SKILL_EVASION); skillIds.Add(SKILL_FEIGN_DEATH);
		type = TALENT_TYPE_CLASS;
		break;
	}
	case TALENT_ROGUE_STEALTH:
	{
		skillIds.Add(SKILL_STEALTH1); skillIds.Add(SKILL_STEALTH2); skillIds.Add(SKILL_STEALTH3); skillIds.Add(SKILL_STEALTH4);
		type = TALENT_TYPE_CLASS;
		break;
	}
	case TALENT_WARRIOR1:
	{
		skillIds.Add(SKILL_POWERFUL); skillIds.Add(SKILL_THREATEN); skillIds.Add(SKILL_BRAVERY); skillIds.Add(SKILL_DEATH_BLOW);
		type = TALENT_TYPE_CLASS;
		break;
	}
	case TALENT_WARRIOR2:
	{
		skillIds.Add(SKILL_PRECISE_STRIKING); skillIds.Add(SKILL_TAUNT); skillIds.Add(SKILL_DISENGAGE); skillIds.Add(SKILL_PERFECT_STRIKE);
		type = TALENT_TYPE_CLASS;
		break;
	}
	case TALENT_BERSERKER:
	{
		skillIds.Add(SKILL_BERSERK); skillIds.Add(SKILL_RESILIENCE); skillIds.Add(SKILL_CONSTRAINT); skillIds.Add(SKILL_FINAL_BLOW);
		type = TALENT_TYPE_SPECIALIZATION;
		break;
	}
	case TALENT_CHAMPION:
	{
		skillIds.Add(SKILL_WAR_CRY); skillIds.Add(SKILL_RALLY); skillIds.Add(SKILL_MOTIVATE); skillIds.Add(SKILL_SUPERIORITY);
		type = TALENT_TYPE_SPECIALIZATION;
		break;
	}
	case TALENT_BLOODMAGE:
	{
		skillIds.Add(SKILL_BLOOD_MAGIC); skillIds.Add(SKILL_BLOOD_SACRIFICE); skillIds.Add(SKILL_BLOOD_WOUND); skillIds.Add(SKILL_BLOOD_CONTROL);
		type = TALENT_TYPE_SPECIALIZATION;
		break;
	}
	case TALENT_SPIRIT_HEALER:
	{
		skillIds.Add(SKILL_GROUP_HEAL); skillIds.Add(SKILL_REVIVAL); skillIds.Add(SKILL_LIFEWARD); skillIds.Add(SKILL_CLEANSING_AURA);
		type = TALENT_TYPE_SPECIALIZATION;
		break;
	}
	case TALENT_BARD:
	{
		skillIds.Add(SKILL_SONG_OF_VALOR); skillIds.Add(SKILL_DISTRACTION); skillIds.Add(SKILL_SONG_OF_COURAGE); skillIds.Add(SKILL_CAPTIVATE);
		type = TALENT_TYPE_SPECIALIZATION;
		break;
	}
	case TALENT_ASSASSIN:
	{
		skillIds.Add(SKILL_MARK_OF_DEATH); skillIds.Add(SKILL_EXPLOIT_WEAKNESS); skillIds.Add(SKILL_LACERATE); skillIds.Add(SKILL_FEAST_OF_THE_FALLEN);
		type = TALENT_TYPE_SPECIALIZATION;
		break;
	}
	case TALENT_DUAL_WEAPON1:
	{
		skillIds.Add(SKILL_DUAL_DOUBLE_STRIKE); skillIds.Add(SKILL_DUAL_RIPOSTE); skillIds.Add(SKILL_DUAL_CRIPPLE); skillIds.Add(SKILL_PUNISHER);
		type = TALENT_TYPE_DUAL_WEAPON;
		break;
	}
	case TALENT_DUAL_WEAPON2:
	{
		skillIds.Add(SKILL_DUAL_WEAPON_SWEEP); skillIds.Add(SKILL_DUAL_WEAPON_FLURRY); skillIds.Add(SKILL_DUAL_WEAPON_MOMENTUM); skillIds.Add(SKILL_WHIRLWIND);
		type = TALENT_TYPE_DUAL_WEAPON;
		break;
	}
	case TALENT_ARCHERY1:
	{
		skillIds.Add(SKILL_AIM); skillIds.Add(SKILL_DEFENSIVE_FIRE); skillIds.Add(SKILL_MELEE_ARCHER); skillIds.Add(SKILL_MASTER_ARCHER);
		type = TALENT_TYPE_ARCHERY;
		break;
	}
	case TALENT_ARCHERY2:
	{
		skillIds.Add(SKILL_PINNING_SHOT); skillIds.Add(SKILL_SUPPRESSING_FIRE); skillIds.Add(SKILL_CRIPPLING_SHOT); skillIds.Add(SKILL_CRITICAL_SHOT);
		type = TALENT_TYPE_ARCHERY;
		break;
	}
	case TALENT_TWO_HANDED1:
	{
		skillIds.Add(SKILL_OVERRUN); skillIds.Add(SKILL_INDOMITABLE); skillIds.Add(SKILL_STUNNING_BLOWS); skillIds.Add(SKILL_CRITICAL_STRIKE);
		type = TALENT_TYPE_TWO_HANDED;
		break;
	}
	case TALENT_TWO_HANDED2:
	{
		skillIds.Add(SKILL_MIGHTY_BLOW); skillIds.Add(SKILL_POWERFUL_SWINGS); skillIds.Add(SKILL_SHATTERING_BLOWS); skillIds.Add(SKILL_WEAPON_SWEEP);
		type = TALENT_TYPE_TWO_HANDED;
		break;
	}
	case TALENT_SHIELD1:
	{
		skillIds.Add(SKILL_SHIELD_DEFENSE); skillIds.Add(SKILL_SHIELD_BALANCE); skillIds.Add(SKILL_SHIELD_WALL); skillIds.Add(SKILL_SHIELD_EXPERTISE);
		type = TALENT_TYPE_SHIELD;
		break;
	}
	case TALENT_SHIELD2:
	{
		skillIds.Add(SKILL_SHIELD_BLOCK); skillIds.Add(SKILL_SHIELD_COVER); skillIds.Add(SKILL_SHIELD_TACTICS); skillIds.Add(SKILL_SHIELD_MASTERY);
		type = TALENT_TYPE_SHIELD;
		break;
	}
	case CLASS_HURLOCK:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_HURLOCK_BLADES); skillIds.Add(SKILL_HURLOCK_KICK);
		break;
	}
	case CLASS_GENLOCK:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_GENLOCK_SHIELD); skillIds.Add(SKILL_GENLOCK_TRAP);
		break;
	}
	case CLASS_GENLOCK_ALPHA:
	{
		skillIds.Add(SKILL_ALPHA_STRIKE); skillIds.Add(SKILL_ALPHA_SPEAR); skillIds.Add(SKILL_GENLOCK_ARTILLERY);
		break;
	}
	case CLASS_OGRE:
	{
		skillIds.Add(SKILL_OGRE_SMASH); skillIds.Add(SKILL_OGRE_ROCK); skillIds.Add(SKILL_OGRE_GRAB);
		break;
	}
	case CLASS_SHRIEK:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_SHRIEK_SCREAM); skillIds.Add(SKILL_SHRIEK_LEAP);
		break;
	}
	case CLASS_SPIDER_CORRUPTED_SMALL:
	case CLASS_SPIDER_GIANT_SMALL:
	{
		skillIds.Add(SKILL_SPIDER_STRIKE);
		break;
	}
	case CLASS_SPIDER_CORRUPTED:
	case CLASS_SPIDER_GIANT:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_SPIDER_POISON); skillIds.Add(SKILL_SPIDER_WEB);
		break;
	}
	case CLASS_DEEP_STALKER:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_STALKER_SPIT); skillIds.Add(SKILL_STALKER_LEAP);
		break;
	}
	case CLASS_BANDIT:
	case CLASS_CARTA_ROGUE:
	case CLASS_BRIGAND_ROGUE:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_BANDIT_KNIVES); skillIds.Add(SKILL_BANDIT_BACKSTAB);
		break;
	}
	case CLASS_BRIGAND_WARRIOR:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_BRIGAND_DIZZY);
		break;
	}
	case CLASS_GENLOCK_ARCHER:
	{
		skillIds.Add(SKILL_DARKSPAWN_CROSSBOW); skillIds.Add(SKILL_GENLOCK_ACID); skillIds.Add(SKILL_GENLOCK_MELEE);
		break;
	}
	case CLASS_HURLOCK_ARCHER:
	{
		skillIds.Add(SKILL_DARKSPAWN_CROSSBOW); skillIds.Add(SKILL_HURLOCK_XBOW_MELEE); skillIds.Add(SKILL_HURLOCK_SNIPE);
		break;
	}
	case CLASS_HURLOCK_EMISSARY:
	{
		skillIds.Add(SKILL_EMISSARY_BOLT); skillIds.Add(SKILL_EMISSARY_SHOCKWAVE); skillIds.Add(SKILL_EMISSARY_DRAIN); skillIds.Add(SKILL_EMISSARY_ESCAPE);
		break;
	}
	case CLASS_GENLOCK_RUNT:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_GENLOCK_SHIELD); skillIds.Add(SKILL_GENLOCK_TRAP);
		break;
	}
	case CLASS_HURLOCK_RUNT:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_HURLOCK_BLADES); skillIds.Add(SKILL_HURLOCK_KICK);
		break;
	}
	case CLASS_GENLOCK_ARCHER_RUNT:
	{
		skillIds.Add(SKILL_DARKSPAWN_CROSSBOW); skillIds.Add(SKILL_GENLOCK_ACID); skillIds.Add(SKILL_GENLOCK_MELEE);
		break;
	}
	case CLASS_HURLOCK_ARCHER_RUNT:
	{
		skillIds.Add(SKILL_DARKSPAWN_CROSSBOW); skillIds.Add(SKILL_HURLOCK_XBOW_MELEE); skillIds.Add(SKILL_HURLOCK_SNIPE);
		break;
	}
	case CLASS_DEEP_STALKER_BABY:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_STALKER_BABY_LEAP);
		break;
	}
	default:
	{
		skillIds.Add(SKILL_STRIKE); skillIds.Add(SKILL_DUMMY_SKILL); skillIds.Add(SKILL_DUMMY_SKILL); skillIds.Add(SKILL_DUMMY_SKILL);
		break;
	}
	}
}
