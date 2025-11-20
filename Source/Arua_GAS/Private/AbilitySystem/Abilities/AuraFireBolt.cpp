// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level,UAbilityInfo* AbilityInfo)
{
	//const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (!AbilityInfo)
	{
		return FString("AbilityInfo is null");
	}
	const FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(FAuraGameplayTags::Get().Abilities_Fire_FireBolt);

	if (Level == 1)
	{
		return AbilityInfo->FormatDescription(Info,Level,ManaCost,Cooldown,1,ScaledDamage,false);
	}
	return AbilityInfo->FormatDescription(Info,Level,ManaCost,Cooldown,FMath::Min(Level, NumProjectiles),ScaledDamage,true);
	
	/*if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FieBolt
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</>"
			"<Default> fire damage with a chance to burn</>"
			),
			// Values
			Level, ManaCost, Cooldown, Damage);
	} 
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FieBolt
			"<Default>Launches %d bolt of fire, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</>"
			"<Default> fire damage with a chance to burn</>"
			),
			// Values
			Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), Damage);
	}*/
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level,UAbilityInfo* AbilityInfo)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (!AbilityInfo)
	{
		return FString("AbilityInfo is null");
	}
	const FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(FAuraGameplayTags::Get().Abilities_Fire_FireBolt);
	return AbilityInfo->FormatDescription(Info,Level,ManaCost,Cooldown,FMath::Min(Level, NumProjectiles),ScaledDamage,true);
	/*return FString::Printf(TEXT(
			// Title
			"<Title>Next Level</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing: </>"
			"<Damage>%d</>"
			"<Default> fire damage with a chance to burn</>"
			),
			// Values
			Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), Damage);*/
}