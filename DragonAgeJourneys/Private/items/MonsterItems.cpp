#include "MonsterItems.h"
#include "Equipment.h"
#include "ItemTypes.h"
#include "Item.h"

UEquipment* GetMonsterEquipment(int32 nItemId, int32 nValue)
{
	UEquipment* equipment = NewObject<UEquipment>();
	switch (nItemId)
	{
	case MONSTERITEM_MELEE_WEAPON:
	{
		equipment->InitializeEquipment(ITEMTYPE_SWORD_LONG);
		equipment->itemName = "Monster Weapon";
		equipment->damage = nValue;
		equipment->uniqueId = nItemId;
		break;
	}
	case MONSTERITEM_RANGED_WEAPON:
	{
		equipment->InitializeEquipment(ITEMTYPE_BOW);
		equipment->itemName = "Monster Ranged Weapon";
		equipment->damage = nValue;
		equipment->uniqueId = nItemId;
		break;
	}
	case MONSTERITEM_ARMOR:
	{
		equipment->InitializeEquipment(ITEMTYPE_ROBE_NORMAL);
		equipment->itemName = "Monster Armor";
		equipment->armor = nValue;
		equipment->uniqueId = nItemId;
		break;
	}
	default:
	{
		break;
	}
	}
	return equipment;

}

