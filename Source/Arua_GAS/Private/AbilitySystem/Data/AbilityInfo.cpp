// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Arua_GAS/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilityInformation)
	{
		if (AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find for AbilityTag [%s] on AbilityInfo [%s]"),
			*AbilityTag.ToString(),*GetNameSafe(this));
	}
	
	return FAuraAbilityInfo();
}

FString UAbilityInfo::FormatDescription(const FAuraAbilityInfo& Info, int32 Level, float Cost, float Cooldown, int32 SpawnNumber, float DamageValue, bool bNextLevel) const
{
	FString Result = bNextLevel ? Info.NextLevelDescription : Info.Description;

	// 1) 等级相关
	Result = Result.Replace(TEXT("_Level_"),      *FString::FromInt(Level));
	Result = Result.Replace(TEXT("_NextLevel_"),  *FString::FromInt(Level + 1));

	// 2) 花费、冷却、伤害（按需替换）
	Result = Result.Replace(TEXT("_Cost_"),  *FString::SanitizeFloat(Cost));
	Result = Result.Replace(TEXT("_CD_"),    *FString::SanitizeFloat(Cooldown));
	Result = Result.Replace(TEXT("_Damage_"),   *FString::SanitizeFloat(DamageValue));
	if (bNextLevel || Level > 1)
	{
		Result = Result.Replace(TEXT("_SpawnNumber_"),   *FString::FromInt(SpawnNumber));
	}

	//return FText::FromString(Result);
	return Result;
}


