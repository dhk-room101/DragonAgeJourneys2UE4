#include "Classes.h"
#include "GameCharacter.h"
#include "Race.h"
#include "Gender.h"
#include "Slot.h"
#include "MonsterItems.h"
#include "UniqueItems.h"
#include "TalentLine.h"

void SetBaseStats(UGameCharacter* gameCharacter)
{
	int32 nClassId = GetBaseClassOf(gameCharacter->classId);

	if (gameCharacter->IsMonster())
	{
		gameCharacter->gear[SLOT_WEAPON] = GetMonsterEquipment(MONSTERITEM_MELEE_WEAPON, 1);
	}

	switch (nClassId)
	{
	case CLASS_MAGE:
	{
		gameCharacter->SetBaseWillpower(gameCharacter->GetBaseWillpower() + 4);
		gameCharacter->SetBaseMagic(gameCharacter->GetBaseMagic() + 5);
		gameCharacter->SetBaseCunning(gameCharacter->GetBaseCunning() + 1);
		break;
	}
	case CLASS_WARRIOR:
	{
		gameCharacter->SetBaseStrength(gameCharacter->GetBaseStrength() + 4);
		gameCharacter->SetBaseConstitution(gameCharacter->GetBaseConstitution() + 3);
		gameCharacter->SetBaseDexterity(gameCharacter->GetBaseDexterity() + 3);
		break;
	}
	case CLASS_ROGUE:
	{
		gameCharacter->SetBaseDexterity(gameCharacter->GetBaseDexterity() + 4);
		gameCharacter->SetBaseWillpower(gameCharacter->GetBaseWillpower() + 2);
		gameCharacter->SetBaseCunning(gameCharacter->GetBaseCunning() + 4);
		break;
	}
	case CLASS_STONE_GOLEM:
	{
		gameCharacter->SetRace(RACE_CONSTRUCT);
		gameCharacter->creatureWidth = 3;
		gameCharacter->base_initiative = 5;
		break;
	}
	case CLASS_GENLOCK:
	{
		gameCharacter->SetRace(RACE_GENLOCK);
		gameCharacter->monsterHealthMultiplier = 1;
		gameCharacter->monsterDamageMultiplier = 1;
		gameCharacter->base_initiative = 15;
		break;
	}
	case CLASS_GENLOCK_ARCHER:
	{
		gameCharacter->SetRace(RACE_GENLOCK);
		gameCharacter->monsterHealthMultiplier = 1;
		gameCharacter->monsterDamageMultiplier = 1;
		gameCharacter->base_initiative = 22;
		break;
	}
	case CLASS_GENLOCK_ALPHA:
	{
		gameCharacter->SetRace(RACE_GENLOCK);
		gameCharacter->monsterHealthMultiplier = 2;
		gameCharacter->monsterDamageMultiplier = 2.5;
		gameCharacter->base_initiative = 15;
		break;
	}
	case CLASS_BANDIT:
	case CLASS_CARTA_ROGUE:
	case CLASS_BRIGAND_ROGUE:
	{
		gameCharacter->SetRace(RACE_DWARF);
		gameCharacter->gender = GENDER_MALE;
		gameCharacter->monsterHealthMultiplier = 1;
		gameCharacter->monsterDamageMultiplier = 1.25;
		gameCharacter->base_initiative = 30;
		break;
	}
	case CLASS_BRIGAND_WARRIOR:
	{
		gameCharacter->SetRace(RACE_DWARF);
		gameCharacter->gender = GENDER_MALE;
		gameCharacter->hairType = 6;
		gameCharacter->hairColor = 2;
		gameCharacter->skinColor = 4;
		gameCharacter->gear[SLOT_OFFHAND] = GetUniqueItem(UNIQUEITEM_STARTER_BUCKLER);
		gameCharacter->monsterHealthMultiplier = 1.3;
		gameCharacter->monsterDamageMultiplier = 1.1;
		break;
	}
	case CLASS_DEEP_STALKER:
	{
		gameCharacter->SetRace(RACE_DEEPSTALKER);
		gameCharacter->monsterHealthMultiplier = 1.2;
		gameCharacter->monsterDamageMultiplier = 1;
		gameCharacter->creatureWidth = 3;
		gameCharacter->base_initiative = 15;
		break;
	}
	case CLASS_HURLOCK:
	{
		gameCharacter->SetRace(RACE_HURLOCK);
		gameCharacter->monsterHealthMultiplier = 1.3;
		gameCharacter->monsterDamageMultiplier = 1.3;
		gameCharacter->base_initiative = 15;
		break;
	}
	case CLASS_HURLOCK_ARCHER:
	{
		gameCharacter->SetRace(RACE_HURLOCK);
		gameCharacter->monsterHealthMultiplier = 1.3;
		gameCharacter->monsterDamageMultiplier = 1.3;
		gameCharacter->base_initiative = 22;
		break;
	}
	case CLASS_HURLOCK_EMISSARY:
	{
		gameCharacter->SetRace(RACE_HURLOCK);
		gameCharacter->immortal = true;
		gameCharacter->monsterHealthMultiplier = 4;
		gameCharacter->monsterDamageMultiplier = 3;
		gameCharacter->base_initiative = 10;
		break;
	}
	case CLASS_SPIDER_GIANT:
	{
		gameCharacter->SetRace(RACE_SPIDER);
		gameCharacter->monsterHealthMultiplier = 3;
		gameCharacter->monsterDamageMultiplier = 3;
		gameCharacter->creatureWidth = 3;
		gameCharacter->base_initiative = 15;
		break;
	}
	case CLASS_SPIDER_GIANT_SMALL:
	{
		gameCharacter->SetRace(RACE_SPIDER);
		gameCharacter->monsterHealthMultiplier = 0.75;
		gameCharacter->monsterDamageMultiplier = 0.8;
		gameCharacter->base_initiative = 30;
		gameCharacter->animScale = 0.5;
		break;
	}
	case CLASS_OGRE:
	{
		gameCharacter->SetRace(RACE_OGRE);
		gameCharacter->creatureWidth = 3;
		gameCharacter->monsterHealthMultiplier = 5;
		gameCharacter->monsterDamageMultiplier = 3.5;
		gameCharacter->base_initiative = 5;
		break;
	}
	case CLASS_GENLOCK_RUNT:
	{
		gameCharacter->SetRace(RACE_GENLOCK);
		gameCharacter->isRunt = true;
		gameCharacter->monsterHealthMultiplier = 0.55;
		gameCharacter->monsterDamageMultiplier = 0.5;
		gameCharacter->base_initiative = 15;
		break;
	}
	case CLASS_GENLOCK_ARCHER_RUNT:
	{
		gameCharacter->SetRace(RACE_GENLOCK);
		gameCharacter->isRunt = true;
		gameCharacter->monsterHealthMultiplier = 0.55;
		gameCharacter->monsterDamageMultiplier = 0.5;
		gameCharacter->base_initiative = 22;
		break;
	}
	case CLASS_HURLOCK_RUNT:
	{
		gameCharacter->SetRace(RACE_HURLOCK);
		gameCharacter->isRunt = true;
		gameCharacter->monsterHealthMultiplier = 0.65;
		gameCharacter->monsterDamageMultiplier = 0.65;
		gameCharacter->base_initiative = 15;
		break;
	}
	case CLASS_HURLOCK_ARCHER_RUNT:
	{
		gameCharacter->SetRace(RACE_HURLOCK);
		gameCharacter->isRunt = true;
		gameCharacter->monsterHealthMultiplier = 0.65;
		gameCharacter->monsterDamageMultiplier = 0.65;
		gameCharacter->base_initiative = 22;
		break;
	}
	case CLASS_DEEP_STALKER_BABY:
	{
		gameCharacter->SetRace(RACE_DEEPSTALKER);
		gameCharacter->monsterHealthMultiplier = 0.6;
		gameCharacter->monsterDamageMultiplier = 0.5;
		gameCharacter->creatureWidth = 1;
		gameCharacter->base_initiative = 15;
		gameCharacter->animScale = 0.4;
		break;
	}
	default:
	{
		break;
	}
	}
}

bool UsesMana(int32 nClassId)
{
	switch (nClassId)
	{
	case CLASS_MAGE:
	case CLASS_BLOODMAGE:
	case CLASS_SPIRIT_HEALER:
	case CLASS_HURLOCK_EMISSARY:
	case CLASS_GENLOCK_EMISSARY:
	case CLASS_CULTIST:
	case CLASS_STONE_GOLEM:
	{
		return true;
	}
	default:
	{
		break;
	}
	}
	return false;
}

int32 BaseSpeedFor(int32 nClassId)
{
	switch (nClassId)
	{
	case CLASS_BERSERKER:
	case CLASS_CHAMPION:
	case CLASS_WARRIOR:
	{
		return 3;
	}
	case CLASS_ASSASSIN:
	case CLASS_BARD:
	case CLASS_ROGUE:
	{
		return 3;
	}
	case CLASS_SPIRIT_HEALER:
	case CLASS_BLOODMAGE:
	case CLASS_MAGE:
	{
		return 2;
	}
	case CLASS_HURLOCK:
	{
		return 3;
	}
	case CLASS_GENLOCK:
	{
		return 3;
	}
	case CLASS_SPIDER_GIANT_SMALL:
	case CLASS_SPIDER_CORRUPTED_SMALL:
	{
		return 5;
	}
	case CLASS_SPIDER_GIANT:
	case CLASS_SPIDER_CORRUPTED:
	{
		return 4;
	}
	case CLASS_OGRE:
	{
		return 4;
	}
	case CLASS_SHRIEK:
	{
		return 4;
	}
	case CLASS_CORRUPTED_DWARF:
	{
		return 4;
	}
	case CLASS_GENLOCK_RUNT:
	{
		return 3;
	}
	case CLASS_GENLOCK_ARCHER_RUNT:
	{
		return 3;
	}
	case CLASS_HURLOCK_RUNT:
	{
		return 3;
	}
	case CLASS_HURLOCK_ARCHER_RUNT:
	{
		return 3;
	}
	case CLASS_DEEP_STALKER_BABY:
	{
		return 3;
	}
	default:
	{
		break;
	}
	}

	return 3;
}

int32 BaseAttackFor(int32 nClassId)
{
	switch (nClassId)
	{
	case CLASS_HURLOCK:
	{
		return 12;
	}
	case CLASS_HURLOCK_ARCHER:
	{
		return 14;
	}
	case CLASS_GENLOCK_ARCHER:
	{
		return 12;
	}
	case CLASS_GENLOCK_ALPHA:
	{
		return 15;
	}
	case CLASS_OGRE:
	{
		return 15;
	}
	case CLASS_HURLOCK_EMISSARY:
	{
		return 15;
	}
	case CLASS_SHRIEK:
	{
		return 20;
	}
	case CLASS_SPIDER_GIANT_SMALL:
	{
		return 20;
	}
	case CLASS_SPIDER_GIANT:
	{
		return 15;
	}
	case CLASS_CARTA_ROGUE:
	case CLASS_BANDIT:
	{
		return 20;
	}
	case CLASS_GENLOCK_RUNT:
	{
		return 8;
	}
	case CLASS_GENLOCK_ARCHER_RUNT:
	{
		return 10;
	}
	case CLASS_HURLOCK_RUNT:
	{
		return 10;
	}
	case CLASS_HURLOCK_ARCHER_RUNT:
	{
		return 12;
	}
	case CLASS_DEEP_STALKER_BABY:
	{
		return 8;
	}
	default:
	{
		break;
	}
	}

	return 10;
}

int32 BaseDefenseFor(int32 nClassId)
{
	switch (nClassId)
	{
	case CLASS_HURLOCK:
	{
		return 10;
	}
	case CLASS_HURLOCK_ARCHER:
	{
		return 10;
	}
	case CLASS_GENLOCK_ARCHER:
	{
		return 10;
	}
	case CLASS_GENLOCK_ALPHA:
	{
		return 15;
	}
	case CLASS_OGRE:
	{
		return 5;
	}
	case CLASS_HURLOCK_EMISSARY:
	{
		return 15;
	}
	case CLASS_SHRIEK:
	{
		return 20;
	}
	case CLASS_SPIDER_GIANT_SMALL:
	{
		return 25;
	}
	case CLASS_SPIDER_GIANT:
	{
		return 7;
	}
	case CLASS_BRIGAND_WARRIOR:
	{
		return 15;
	}
	case CLASS_CARTA_ROGUE:
	case CLASS_BANDIT:
	{
		return 20;
	}
	case CLASS_GENLOCK_RUNT:
	{
		return 8;
	}
	case CLASS_GENLOCK_ARCHER_RUNT:
	{
		return 8;
	}
	case CLASS_HURLOCK_RUNT:
	{
		return 8;
	}
	case CLASS_HURLOCK_ARCHER_RUNT:
	{
		return 8;
	}
	case CLASS_DEEP_STALKER_BABY:
	{
		return 8;
	}
	default:
	{
		break;
	}
	}

	return 10;
}

int32 BaseArmorFor(int32 nClassId)
{
	switch (nClassId)
	{
	case CLASS_GENLOCK_ARCHER:
	case CLASS_GENLOCK:
	case CLASS_HURLOCK_ARCHER:
	case CLASS_HURLOCK:
	{
		return 2;
	}
	case CLASS_GENLOCK_ALPHA:
	case CLASS_HURLOCK_ALPHA:
	{
		return 5;
	}
	case CLASS_STONE_GOLEM:
	{
		return 10;
	}
	case CLASS_BRIGAND_WARRIOR:
	{
		return 7;
	}
	case CLASS_SPIDER_GIANT:
	case CLASS_SPIDER_CORRUPTED:
	{
		return 5;
	}
	case CLASS_GENLOCK_RUNT:
	{
		return 1;
	}
	case CLASS_GENLOCK_ARCHER_RUNT:
	{
		return 1;
	}
	case CLASS_HURLOCK_RUNT:
	{
		return 1;
	}
	case CLASS_HURLOCK_ARCHER_RUNT:
	{
		return 1;
	}
	case CLASS_DEEP_STALKER_BABY:
	{
		return 0;
	}
	default:
	{
		break;
	}
	}

	return 0;
}

int32 GetBaseClassOf(int32 nClassId)
{
	switch (nClassId)
	{
	case CLASS_WARRIOR:
	case CLASS_BERSERKER:
	case CLASS_CHAMPION:
	{
		return CLASS_WARRIOR;
	}
	case CLASS_MAGE:
	case CLASS_BLOODMAGE:
	case CLASS_SPIRIT_HEALER:
	{
		return CLASS_MAGE;
	}
	case CLASS_ROGUE:
	case CLASS_BARD:
	case CLASS_ASSASSIN:
	{
		return CLASS_ROGUE;
	}
	default:
	{
		break;
	}
	}

	return nClassId;
}

FString GetDescriptionForClass(int32 nClassId)
{
	switch (nClassId)
	{
	case CLASS_WARRIOR:
	{
		return "Warrior";
	}
	case CLASS_ROGUE:
	{
		return "Rogue";
	}
	case CLASS_MAGE:
	{
		return "Mage";
	}
	case CLASS_BERSERKER:
	{
		return "Berserker";
	}
	case CLASS_CHAMPION:
	{
		return "Grey Warden";
	}
	case CLASS_BARD:
	{
		return "Bard";
	}
	case CLASS_ASSASSIN:
	{
		return "Assassin";
	}
	case CLASS_BLOODMAGE:
	{
		return "Blood Mage";
	}
	case CLASS_SPIRIT_HEALER:
	{
		return "Spirit Healer";
	}
	case CLASS_HURLOCK:
	case CLASS_HURLOCK_ARCHER:
	case CLASS_HURLOCK_RUNT:
	case CLASS_HURLOCK_ARCHER_RUNT:
	{
		return "Brutish darkspawn, larger and more powerful than a man.";
	}
	case CLASS_GENLOCK:
	case CLASS_GENLOCK_ARCHER:
	case CLASS_GENLOCK_RUNT:
	case CLASS_GENLOCK_ARCHER_RUNT:
	{
		return "Smaller, devious darkspawn. Known to use traps and fortifications.";
	}
	case CLASS_GENLOCK_ALPHA:
	case CLASS_HURLOCK_ALPHA:
	{
		return "Some darkspawn are born faster, stronger and smarter than the rest. These are the Alphas, leaders on the battlefield.";
	}
	case CLASS_HURLOCK_EMISSARY:
	{
		return "Rare darkspawn with sorcerous abilities and the power of speech.";
	}
	case CLASS_GENLOCK_EMISSARY:
	{
		return "Rare darkspawn with sorcerous abilities and the power of speech.";
	}
	case CLASS_OGRE:
	{
		return "Enormous and savagely powerful darkspawn.";
	}
	case CLASS_SPIDER_CORRUPTED:
	case CLASS_SPIDER_CORRUPTED_SMALL:
	{
		return "Giant spider-like creature, corrupted by the taint.";
	}
	case CLASS_SPIDER_GIANT:
	{
		return "Giant spider-like creature.";
	}
	case CLASS_SPIDER_GIANT_SMALL:
	{
		return "Giant spider-like creature. Not fully grown.";
	}
	case CLASS_DEEP_STALKER:
	case CLASS_DEEP_STALKER_BABY:
	{
		return "Underground predators that hide amongst the rocks.";
	}
	case CLASS_BANDIT:
	case CLASS_BRIGAND_ROGUE:
	{
		return "Dwarven cutthroats who prey on traffic through the Deep Roads.";
	}
	case CLASS_BRIGAND_WARRIOR:
	{
		return "Dwarven cutthroats who prey on traffic through the Deep Roads.";
	}
	case CLASS_CARTA_ROGUE:
	{
		return "Thieves and murderers in the employ of the Carta, the dwarven crime lords.";
	}
	case CLASS_STONE_GOLEM:
	{
		return "Powerful construct, ancient weapon of the dwarves.";
	}
	default:
	{
		break;
	}
	}

	return "[Blank Description]";
}

FString GetConversationDescription(int32 nClassId)
{
	switch (nClassId)
	{
	case CLASS_WARRIOR:
	{
		return "Warrior";
	}
	case CLASS_ROGUE:
	{
		return "Rogue";
	}
	case CLASS_MAGE:
	{
		return "Mage";
	}
	case CLASS_BERSERKER:
	{
		return "Berserker";
	}
	case CLASS_CHAMPION:
	{
		return "Grey Warden";
	}
	case CLASS_BARD:
	{
		return "Bard";
	}
	case CLASS_ASSASSIN:
	{
		return "Assassin";
	}
	case CLASS_BLOODMAGE:
	{
		return "Blood Mage";
	}
	case CLASS_SPIRIT_HEALER:
	{
		return "Spirit Healer";
	}
	case CLASS_HURLOCK:
	{
		return "Brutish darkspawn";
	}
	case CLASS_GENLOCK:
	{
		return "Darkspawn";
	}
	case CLASS_GENLOCK_ALPHA:
	case CLASS_HURLOCK_ALPHA:
	{
		return "Darkspawn Leader";
	}
	case CLASS_HURLOCK_EMISSARY:
	{
		return "Hurlock Emissary";
	}
	case CLASS_GENLOCK_EMISSARY:
	{
		return "Genlock Emissary";
	}
	case CLASS_OGRE:
	{
		return "Powerful Darkspawn";
	}
	case CLASS_SPIDER_CORRUPTED:
	case CLASS_SPIDER_CORRUPTED_SMALL:
	{
		return "Spider";
	}
	case CLASS_SPIDER_GIANT:
	{
		return "Giant Spider";
	}
	case CLASS_SPIDER_GIANT_SMALL:
	{
		return "Giant Spider";
	}
	case CLASS_DEEP_STALKER:
	{
		return "Deep Stalker";
	}
	case CLASS_BANDIT:
	case CLASS_BRIGAND_ROGUE:
	{
		return "Dwarven Cutthroat";
	}
	case CLASS_BRIGAND_WARRIOR:
	{
		return "Dwarven Cutthroat";
	}
	case CLASS_CARTA_ROGUE:
	{
		return "Murdering Thief";
	}
	case CLASS_STONE_GOLEM:
	{
		return "Ancient Weapon";
	}
	default:
	{
		break;
	}
	}

	return "";
}

FString GetNameForClass(int32 nClassId)
{
	switch (nClassId)
	{
	case CLASS_WARRIOR:
	{
		return "Warrior";
	}
	case CLASS_ROGUE:
	{
		return "Rogue";
	}
	case CLASS_MAGE:
	{
		return "Mage";
	}
	case CLASS_BERSERKER:
	{
		return "Berserker";
	}
	case CLASS_CHAMPION:
	{
		return "Champion";
	}
	case CLASS_BARD:
	{
		return "Bard";
	}
	case CLASS_ASSASSIN:
	{
		return "Assassin";
	}
	case CLASS_BLOODMAGE:
	{
		return "Blood Mage";
	}
	case CLASS_SPIRIT_HEALER:
	{
		return "Spirit Healer";
	}
	case CLASS_HURLOCK:
	{
		return "Hurlock Warrior";
	}
	case CLASS_HURLOCK_ARCHER:
	{
		return "Hurlock Archer";
	}
	case CLASS_HURLOCK_ALPHA:
	{
		return "Hurlock Alpha";
	}
	case CLASS_HURLOCK_EMISSARY:
	{
		return "Hurlock Emissary";
	}
	case CLASS_SHRIEK:
	{
		return "Shriek";
	}
	case CLASS_OGRE:
	{
		return "Ogre";
	}
	case CLASS_SPIDER_CORRUPTED:
	{
		return "Corrupted Spider";
	}
	case CLASS_SPIDER_CORRUPTED_SMALL:
	{
		return "Corrupted Spiderling";
	}
	case CLASS_GENLOCK:
	{
		return "Genlock";
	}
	case CLASS_GENLOCK_ARCHER:
	{
		return "Genlock Archer";
	}
	case CLASS_GENLOCK_ALPHA:
	{
		return "Genlock Alpha";
	}
	case CLASS_GENLOCK_EMISSARY:
	{
		return "Genlock Emissary";
	}
	case CLASS_SPIDER_GIANT:
	{
		return "Giant Spider";
	}
	case CLASS_SPIDER_GIANT_SMALL:
	{
		return "Giant Spiderling";
	}
	case CLASS_DEEP_STALKER:
	{
		return "Deep Stalker Matriarch";
	}
	case CLASS_STONE_GOLEM:
	{
		return "Stone Golem";
	}
	case CLASS_CORRUPTED_DWARF:
	{
		return "Corrupted Dwarf";
	}
	case CLASS_BRIGAND_ROGUE:
	{
		return "Highwayman";
	}
	case CLASS_BANDIT:
	{
		return "Bandit";
	}
	case CLASS_CARTA_ROGUE:
	{
		return "Carta Rogue";
	}
	case CLASS_BRIGAND_WARRIOR:
	{
		return "Brigand";
	}
	case CLASS_GENLOCK_ARCHER_RUNT:
	{
		return "Genlock Archer Runt";
	}
	case CLASS_GENLOCK_RUNT:
	{
		return "Genlock Runt";
	}
	case CLASS_HURLOCK_ARCHER_RUNT:
	{
		return "Hurlock Archer Runt";
	}
	case CLASS_HURLOCK_RUNT:
	{
		return "Hurlock Runt";
	}
	case CLASS_DEEP_STALKER_BABY:
	{
		return "Deep Stalker";
	}
	default:
	{
		break;
	}
	}

	return "Unknown";
}

FString PowerString(int32 nClassId)
{
	if (UsesMana(nClassId))
	{
		return "mana";
	}
	
	return "stamina";
}

TArray<UTalentLine*> GetTalentsForClass(UGameCharacter* gameCharacter)
{
	TArray<UTalentLine*> talents;
	UTalentLine* talentLine = nullptr;
	int32 nUnlocked = 0;

	if (gameCharacter->classId == CLASS_UNDEFINED)
	{
		return talents;
	}

	if (gameCharacter->IsMonster())
	{
		talentLine = NewObject<UTalentLine>();
		talentLine->InitializeTalentLine(gameCharacter->classId);
		talents.Add(talentLine);
	}
	else
	{
		switch (gameCharacter->classId)
		{
		case CLASS_WARRIOR:
		case CLASS_BERSERKER:
		case CLASS_CHAMPION:
		{
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_WARRIOR1, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_WARRIOR2, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_TWO_HANDED1, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_TWO_HANDED2, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_SHIELD1, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_SHIELD2, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_DUAL_WEAPON1, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_DUAL_WEAPON2, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_ARCHERY1, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_ARCHERY2, nUnlocked);
			talents.Add(talentLine);
			break;
		}
		case CLASS_ROGUE:
		case CLASS_ASSASSIN:
		case CLASS_BARD:
		{
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_ROGUE1, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_ROGUE2, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_ROGUE_STEALTH, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_DUAL_WEAPON1, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_DUAL_WEAPON2, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_ARCHERY1, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_ARCHERY2, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_TWO_HANDED1, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_TWO_HANDED2, nUnlocked);
			talents.Add(talentLine);
			break;
		}
		case CLASS_MAGE:
		case CLASS_SPIRIT_HEALER:
		case CLASS_BLOODMAGE:
		{
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_MAGE_POWER, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_MAGE_FIRE, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_MAGE_LIGHTNING, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_MAGE_HEAL, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_MAGE_DEATH, nUnlocked);
			talents.Add(talentLine);
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_MAGE_PSIONICS, nUnlocked);
			talents.Add(talentLine);
			break;
		}
		default:
		{
			break;
		}
		}
		switch (gameCharacter->classId)
		{
		case CLASS_BERSERKER:
		{
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_BERSERKER, nUnlocked);
			talents.Add(talentLine);
			break;
		}
		case CLASS_BLOODMAGE:
		{
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_BLOODMAGE, nUnlocked);
			talents.Add(talentLine);
			break;
		}
		case CLASS_CHAMPION:
		{
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_CHAMPION, nUnlocked);
			talents.Add(talentLine);
			break;
		}
		case CLASS_SPIRIT_HEALER:
		{
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_SPIRIT_HEALER, nUnlocked);
			talents.Add(talentLine);
			break;
		}
		case CLASS_BARD:
		{
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_BARD, nUnlocked);
			talents.Add(talentLine);
			break;
		}
		case CLASS_ASSASSIN:
		{
			talentLine = NewObject<UTalentLine>();
			talentLine->InitializeTalentLine(TALENT_ASSASSIN, nUnlocked);
			talents.Add(talentLine);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	return talents;
}

