// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraArcaneShards.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"

using namespace AuraGameplayTags;

FString UAuraArcaneShards::GetDescription(int32 Level, UAbilityInfo* AbilityInfo)
{
	const int32 ScaledDamage = FMath::RoundToInt(Damage.GetValueAtLevel(Level));
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (!AbilityInfo)
	{
		return FString("AbilityInfo is null");
	}
	const FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(Abilities_Arcane_ArcaneShards);

	if (Level == 1)
	{
		return AbilityInfo->FormatDescription(Info,Level,ManaCost,Cooldown,0,ScaledDamage,false);
	}
	return AbilityInfo->FormatDescription(Info,Level,ManaCost,Cooldown,FMath::Min(Level, MaxNumShards),ScaledDamage,true);
	
}

FString UAuraArcaneShards::GetNextLevelDescription(int32 Level, UAbilityInfo* AbilityInfo)
{
	const int32 ScaledDamage = FMath::RoundToInt(Damage.GetValueAtLevel(Level));
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (!AbilityInfo)
	{
		return FString("AbilityInfo is null");
	}
	const FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(Abilities_Arcane_ArcaneShards);
	return AbilityInfo->FormatDescription(Info,Level,ManaCost,Cooldown,FMath::Min(Level, MaxNumShards),ScaledDamage,true);
}
