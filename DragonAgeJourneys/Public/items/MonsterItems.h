#pragma once

const int32 MONSTERITEM_MELEE_WEAPON = 601;
const int32 MONSTERITEM_RANGED_WEAPON = 603;
const int32 MONSTERITEM_ARMOR = 602;

class UEquipment;

UEquipment* GetMonsterEquipment(int32 nItemId, int32 nValue);
