// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItem()
{
	TArray<FLootItem> ReturnLootItems;
	for (FLootItem& Item: LootItems)
	{
		for (int32 i = 0; i < Item.MaxNumberToSpawn; ++i)//遍历每一个结构体的生成次数
		{
			if (Item.ChanceToSpawn > FMath::RandRange(1, 100))//若几率大于随机值
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;
				ReturnLootItems.Add(NewItem);
			}
		}
	}
	return ReturnLootItems;
}
