// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

using namespace AuraGameplayTags;

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		//CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,true);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorToIgnore;
			ActorToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamSpell::PrimaryTargetDied);
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);// 这里排除第一个被点击到的目标，后面找的都是额外目标
	TArray<AActor*> OverlappingActors;

	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation());

	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);

	for (int32 ActorIdx = OverlappingActors.Num() - 1; ActorIdx >= 0; --ActorIdx)
	{
		if (OverlappingActors[ActorIdx]->ActorHasTag(FName("Player")))
		{
			OverlappingActors.RemoveAt(ActorIdx);
		}
	}
	
	UAuraAbilitySystemLibrary::GetClosestTargets(
		NumAdditionalTargets,
		OverlappingActors,
		OutAdditionalTargets,
		MouseHitActor->GetActorLocation());

	// 遍历额外的角色，如果其中有角色死亡，就调用蓝图覆写的这个函数(移除 GC)
	for (AActor* Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
        {
        	if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalTargetDied))
        	{
        		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamSpell::AdditionalTargetDied);
        	}
        }
	}
}

void UAuraBeamSpell::RemoveOnDeathBindingFromPrimaryTarget()
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor);
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().RemoveDynamic(
			this, &ThisClass::PrimaryTargetDied);
	}
}

void UAuraBeamSpell::RemoveOnDeathBindingFromAdditionalTarget()
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor);
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().RemoveDynamic(
			this, &ThisClass::AdditionalTargetDied);
	}
}

FString UAuraBeamSpell::GetDescription(int32 Level, UAbilityInfo* AbilityInfo)
{
	const int32 ScaledDamage = FMath::RoundToInt(Damage.GetValueAtLevel(Level));
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (!AbilityInfo)
	{
		return FString("AbilityInfo is null");
	}
	const FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(Abilities_Lightning_Electrocute);

	if (Level == 1)
	{
		return AbilityInfo->FormatDescription(Info,Level,ManaCost,Cooldown,0,ScaledDamage,false);
	}
	return AbilityInfo->FormatDescription(Info,Level,ManaCost,Cooldown,FMath::Min(Level - 1,  MaxNumShockTargets),ScaledDamage,true);
}

FString UAuraBeamSpell::GetNextLevelDescription(int32 Level, UAbilityInfo* AbilityInfo)
{
	const int32 ScaledDamage = FMath::RoundToInt(Damage.GetValueAtLevel(Level));
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (!AbilityInfo)
	{
		return FString("AbilityInfo is null");
	}
	const FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(Abilities_Lightning_Electrocute);
	return AbilityInfo->FormatDescription(Info,Level,ManaCost,Cooldown,FMath::Min(Level - 1,  MaxNumShockTargets),ScaledDamage,true);
}
