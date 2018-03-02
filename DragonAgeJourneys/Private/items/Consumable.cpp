#include "Consumable.h"
#include "ItemTypes.h"
#include "GameCharacter.h"
#include "Classes.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Party.h"
#include "Tooltip.h"

UConsumable::UConsumable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UConsumable::InitializeConsumable(int32 nTypeId /*= 5001*/, int32 nStack /*= 1*/)
{
	itemName = "Consumable";
	itemLevel = 0;
	itemRarity = ITEM_ONE_MOD;
	iconFrame = 20;
	typeId = nTypeId;
	stack = nStack;
	SetStatsForConsumableType(typeId);
}

int32 UConsumable::GetHealing(UGameCharacter* gameCharacter)
{
	if (resurrect && gameCharacter->health > 0)
	{
		return 0;
	}

	int32 nHealing = restore_health;
	if (restore_health_pc)
	{
		nHealing = nHealing + restore_health_pc * gameCharacter->GetHealthMax() * 0.01;
	}

	nHealing = nHealing * (gameCharacter->GetMagic() * PER_MAGIC_POTION_MULTIPLIER + 1);

	return FMath::RoundToInt(nHealing);
}

int32 UConsumable::GetPower(UGameCharacter* gameCharacter)
{
	int32 nPower = restore_power + restore_mana + restore_stamina;
	if (restore_power_pc)
	{
		nPower = nPower + restore_power_pc * gameCharacter->GetPowerMax() * 0.01;
	}

	nPower = nPower * (gameCharacter->GetMagic() * PER_MAGIC_POTION_MULTIPLIER + 1);

	return FMath::RoundToInt(nPower);
}

bool UConsumable::UsableBy(UGameCharacter* gameCharacter, bool bSkill /*= false*/)
{
	if (itemRarity == ITEM_QUEST)
	{
		return false;
	}

	if (autoRevival)
	{
		return false;
	}

	if (sneaky && GetParty()->GetSneaking() < 2)
	{
		return true;
	}

	if (town_portal)
	{
		return true;
	}

	if (escapeItem && GetMain()->Battlefield == nullptr)
	{
		return false;
	}

	if (!usableInBattle && !(GetMain()->Battlefield == nullptr))
	{
		return false;
	}

	if (!UsesMana(gameCharacter->classId))
	{
		if (restore_mana)
		{
			return false;
		}
	}

	if (gameCharacter->health <= 0)
	{
		if (resurrect)
		{
			return true;
		}

		return false;
	}

	if (bSkill)
	{
		if (resurrect)
		{
			return false;
		}

		if (restore_health && gameCharacter->health < gameCharacter->GetHealthMax())
		{
			return true;
		}

		if (gameCharacter->power < gameCharacter->GetPowerMax())
		{
			if (restore_power)
			{
				return true;
			}

			if (restore_mana && UsesMana(gameCharacter->classId))
			{
				return true;
			}

			if (restore_stamina && !UsesMana(gameCharacter->classId))
			{
				return true;
			}
		}
	}
	else
	{
		if (restore_power || restore_health)
		{
			return true;
		}

		if (restore_mana && UsesMana(gameCharacter->classId))
		{
			return true;
		}

		if (restore_stamina && !UsesMana(gameCharacter->classId))
		{
			return true;
		}
	}

	if (increase_strength)
	{
		return true;
	}

	if (increase_constitution)
	{
		return true;
	}

	return false;
}

FString UConsumable::ItemStatsString(FString sStats)
{
	sStats = "";

	if (autoRevival)
	{
		return FONT_TAG_GREEN + "Revives a fallen character after battle.</font>";
	}
	if (escapeItem)
	{
		sStats = sStats + "Escape from the current battle\n";
	}
	if (town_portal)
	{
		sStats = sStats + "Teleport to the Emporium, to trade equipment and items\n";
	}
	if (reset_stats)
	{
		sStats = sStats + "Reset a character\'s strength, endurance, dexterity and intellect to base amounts for level, recover all spent Stat points.\n\n(Tip: Shift-click on increase button to spend all remaining stat points, ctrl-click to spend 2/3)\n";
	}
	if (resurrect)
	{
		sStats = sStats + "Revives a character\n";
	}
	if (sneaky)
	{
		sStats = sStats + "Allows the party to pass by one enemy horde without being attacked\n";
	}
	if (restore_health_pc)
	{
		sStats = sStats + ("Restores " + FString::FromInt(restore_health_pc) + "% maximum health\n");
	}
	if (restore_power_pc)
	{
		sStats = sStats + ("Restores " + FString::FromInt(restore_power_pc) + "% maximum power\n");
	}
	if (restore_health)
	{
		sStats = sStats + ("Restores " + FString::FromInt(restore_health) + " health\n");
	}
	if (restore_power)
	{
		sStats = sStats + ("Restores " + FString::FromInt(restore_power) + " mana/stamina\n");
	}
	if (restore_mana)
	{
		sStats = sStats + ("Restores " + FString::FromInt(restore_mana) + " mana\n");
	}
	if (restore_stamina)
	{
		sStats = sStats + ("Restores " + FString::FromInt(restore_stamina) + " stamina\n");
	}
	if (damage_resist)
	{
		sStats = sStats + ("Grants +" + FString::FromInt(damage_resist) + "% damage resistance for next battle\n");
	}
	if (damage_bonus)
	{
		sStats = sStats + ("Grants +" + FString::FromInt(damage_bonus) + "% bonus damage for next battle\n");
	}
	if (stun_resist)
	{
		sStats = sStats + ("Grants +" + FString::FromInt(stun_resist) + "% stun resistance for next battle\n");
	}
	if (poison_resist)
	{
		sStats = sStats + ("Grants +" + FString::FromInt(poison_resist) + "% poison resistance for next battle\n");
	}
	if (quickness_bonus)
	{
		sStats = sStats + ("Grants +" + FString::FromInt(quickness_bonus) + " quickness for next battle\n");
	}
	if (increase_strength)
	{
		sStats = sStats + ("Permanently increase Strength by " + FString::FromInt(increase_strength) + "\n");
	}
	if (increase_constitution)
	{
		sStats = sStats + ("Permanently increase Constitution by " + FString::FromInt(increase_constitution) + "\n");
	}
	if (increase_dexterity)
	{
		sStats = sStats + ("Permanently increase Dexterity by " + FString::FromInt(increase_dexterity )+ "\n");
	}
	if (increase_intellect)
	{
		sStats = sStats + ("Permanently increase Intellect by " + FString::FromInt(increase_intellect) + "\n");
	}
	if (campaign_combiner)
	{
		sStats = sStats + "Combine nine fragments into a Journeyman\'s Stone\n";
	}
	if (campaign_portal)
	{
		sStats = sStats + "Teleport to the Sanctum, to face the Dreadfather\n";
	}
	if (sStats != "")
	{
		sStats = FONT_TAG_GREEN + "Use: " + sStats + "</font>";
	}
	return sStats;
}

UConsumable::~UConsumable()
{
}

void UConsumable::SetStatsForConsumableType(int32 nTypeId)
{
	switch (nTypeId)
	{
	case ITEMTYPE_SCROLL_DECEPTION:
	{
		itemName = "Scroll of Deception";
		sneaky = true;
		iconFrame = 84;
		consumeSound = "book";
		break;
	}
	case ITEMTYPE_SCROLL_PORTAL:
	{
		itemName = "Portal Scroll";
		town_portal = true;
		iconFrame = 84;
		consumeSound = "book";
		break;
	}
	case ITEMTYPE_SCROLL_RESPEC:
	{
		itemName = "Clean Slate";
		lore = "It\'s not often you get a chance to start over.";
		reset_stats = true;
		iconFrame = 84;
		consumeSound = "book";
		break;
	}
	case ITEMTYPE_POTION_HEAL_SMALL:
	{
		itemName = "Lesser Healing Salve";
		restore_health = 40;
		iconFrame = 24;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_HEAL_MEDIUM:
	{
		itemName = "Greater Healing Salve";
		restore_health = 75;
		iconFrame = 25;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_HEAL_LARGE:
	{
		itemName = "Potent Healing Salve";
		restore_health = 150;
		iconFrame = 26;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_LYRIUM_SMALL:
	{
		itemName = "Lesser Lyrium Potion";
		restore_mana = 40;
		iconFrame = 21;
		lore = "Restricted to: Mage";
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_LYRIUM_MEDIUM:
	{
		itemName = "Greater Lyrium Potion";
		restore_mana = 75;
		iconFrame = 22;
		lore = "Restricted to: Mage";
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_LYRIUM_LARGE:
	{
		itemName = "Potent Lyrium Potion";
		restore_mana = 120;
		iconFrame = 23;
		lore = "Restricted to: Mage";
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_STAMINA_SMALL:
	{
		itemName = "Lesser Soothing Balm";
		restore_stamina = 40;
		iconFrame = 27;
		lore = "Restricted to: Warrior, Rogue";
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_STAMINA_MEDIUM:
	{
		itemName = "Greater Soothing Balm";
		restore_stamina = 75;
		iconFrame = 28;
		lore = "Restricted to: Warrior, Rogue";
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_STAMINA_LARGE:
	{
		itemName = "Potent Soothing Balm";
		restore_stamina = 120;
		iconFrame = 29;
		lore = "Restricted to: Warrior, Rogue";
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_INJURY_KIT:
	{
		itemName = "Injury Kit";
		lore = "These kits contain the medical supplies necessary to stabilize and revive a seriously wounded companion in the field. This requires time and great care, so they cannot be used in the midst of battle.";
		resurrect = true;
		usableInBattle = false;
		restore_health_pc = INJURY_KIT_HEAL_PERCENT;
		iconFrame = 19;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_SMOKE_BOMB:
	{
		itemName = "Smoke Bomb";
		lore = "Battle Only";
		escapeItem = true;
		iconFrame = 18;
		break;
	}
	case ITEMTYPE_POTION_REJUVE:
	{
		itemName = "Rejuvenation Potion";
		restore_health = 75;
		restore_power = 75;
		iconFrame = 20;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_REVIVE:
	{
		itemName = "Revival Potion";
		restore_health = 75;
		resurrect = true;
		iconFrame = 20;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_REVIVE_SMALL:
	{
		itemName = "Weak Revival Potion";
		restore_health = 30;
		resurrect = true;
		iconFrame = 20;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_POISON_RESIST:
	{
		itemName = "Snake\'s Blood Potion";
		restore_health = 20;
		poison_resist = 100;
		iconFrame = 20;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_SPEED:
	{
		itemName = "Elixir of Speed";
		quickness_bonus = 20;
		iconFrame = 20;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_SHIELD:
	{
		itemName = "Elixir of Protection";
		damage_resist = 20;
		stun_resist = 50;
		iconFrame = 20;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_WITS:
	{
		itemName = "Elixir of Enhanced Wits";
		increase_dexterity = 5;
		increase_intellect = 5;
		iconFrame = 20;
		consumeSound = "heal_little";
		break;
	}
	case ITEMTYPE_POTION_BODY:
	{
		itemName = "Arteme\'s Physical Tonic";
		lore = "Made to order from rare ingredients.";
		increase_strength = 2;
		increase_constitution = 2;
		iconFrame = 20;
		consumeSound = "heal_little";
		break;
	}
	default:
	{
		break;
	}
	}
}


