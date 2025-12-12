// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"//宏版本的必须带这个

/**
 * AuraGameplayTags
 *
 * Singleton containing nativeGameplay Tags
 */

/*struct FAuraGameplayTags
{
public:
	static FAuraGameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag Attributes_Meta_IncomingXP;
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Frequency;
	FGameplayTag Debuff_Duration;
	
	FGameplayTag Abilities_None;

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;

	FGameplayTag Cooldown_Fire_FireBolt;

	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_Tail;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	
	FGameplayTag Effects_HitReact;
private:
	static FAuraGameplayTags GameplayTags;	//开辟新的变量，创建全局唯一的实例
};*/

namespace AuraGameplayTags
{
	/*
	 * Primary Attributes
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);

	/*
	 * Secondary Attributes
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana);

	/*
	 * Meta Attributes
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Meta_IncomingXP);

	/*
	 * Input Tags
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_2);

	/*
	 * Damage
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);

	/*
	 * Resistance
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Lightning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Arcane);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Physical);

	/*
	 * Debuffs
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Chance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Frequency);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Duration);

	/*
	 * Abilities
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Passive);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_None);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Summon);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_HitReact);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Locked);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Eligible);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Unlocked);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Equipped);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_Offensive);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_Passive);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_None);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Fire_FireBolt);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Lightning_Electrocute);
	
	/*
	 *  Passive Spells
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Passive_HaloOfProtection);// 保护光环
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Passive_LifeSiphon);// 生命虹吸
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Passive_ManaSiphon);// 法力虹吸

	/*
	 * Cooldown
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Fire_FireBolt);

	/*
	 * Combat Sockets
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Weapon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_LeftHand);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_RightHand);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Tail);

	/*
	 * Montage Tags
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_4);

	/*
	 * Maps
	 *  伤害类型 -> 抗性
	 *  伤害类型 -> Debuff 类型
	 */
	extern const TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;
	extern const TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	/*
	 * Effects
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effects_HitReact);

	/*
	 *  Player Tags
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_CursorTrace);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputPressed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputReleased);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputHeld);
}

