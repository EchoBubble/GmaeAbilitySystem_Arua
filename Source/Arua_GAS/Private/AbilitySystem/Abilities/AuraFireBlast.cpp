// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"

using namespace AuraGameplayTags;

FString UAuraFireBlast::GetDescription(int32 Level, UAbilityInfo* AbilityInfo)
{
	const int32 ScaledDamage = FMath::RoundToInt(Damage.GetValueAtLevel(Level));
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (!AbilityInfo)
	{
		return FString("AbilityInfo is null");
	}
	const FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(Abilities_Fire_FireBlast);

	if (Level == 1)
	{
		return AbilityInfo->FormatDescription(Info,Level,FMath::RoundToInt(ManaCost),Cooldown,1,ScaledDamage,false);
	}
	return AbilityInfo->FormatDescription(Info,Level,FMath::RoundToInt(ManaCost),Cooldown,FMath::Min(Level, NumFireBlasts),ScaledDamage,true);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level, UAbilityInfo* AbilityInfo)
{
	const int32 ScaledDamage = FMath::RoundToInt(Damage.GetValueAtLevel(Level));
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	FMath::RoundToInt(ManaCost);
	const float Cooldown = GetCooldown(Level);

	if (!AbilityInfo)
	{
		return FString("AbilityInfo is null");
	}
	const FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(Abilities_Fire_FireBlast);
	return AbilityInfo->FormatDescription(Info,Level,FMath::RoundToInt(ManaCost),Cooldown,FMath::Min(Level, NumFireBlasts),ScaledDamage,true);
}
