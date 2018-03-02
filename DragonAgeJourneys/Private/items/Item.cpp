#include "Item.h"
#include "ItemTypes.h"

UItem::UItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UItem::~UItem()
{
}

FString UItem::ItemStatsString(FString sStats)
{
	if (typeId == ITEMTYPE_MONEY_BAG)
	{
		return "These coins are commonly used by the dwarves of Orzammar.\n\n";
		// +packages.tooltips.Tooltip.FONT_TAG_GREEN + packages.items.LootGenerator->GetCashString(temporaryValue) + "</font>";
	}

	return sStats;
}
