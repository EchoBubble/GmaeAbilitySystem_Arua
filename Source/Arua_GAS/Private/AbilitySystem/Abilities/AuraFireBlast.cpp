// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Actor/AuraFireBall.h"

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

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return TArray<AAuraFireBall*>();
	
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBlasts);

	if (!IsValid(FireBlastClass)) return TArray<AAuraFireBall*>();
	
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBlastClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();

		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		
		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	}
	
	return FireBalls;
}
