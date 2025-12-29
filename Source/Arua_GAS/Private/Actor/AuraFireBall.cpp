// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerState.h"

using namespace AuraGameplayTags;

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IgnoreList.Contains(OtherActor)) return;
	
	AActor* SourceAvatarActor = GetOwner();
	if (APlayerState* PS = Cast<APlayerState>(SourceAvatarActor)) SourceAvatarActor = PS->GetPawn();
	if (SourceAvatarActor == OtherActor) return;
	if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor,OtherActor))return;
	
	//if (!bHit) OnHit();
	
	if (HasAuthority())//只有服务器才有权威去销毁
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;//投射物方向和冲量大小
			DamageEffectParams.DeathImpulse = DeathImpulse;//设置死亡冲击方向
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;

			ExplosionDamageParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			IgnoreList.Add(OtherActor);
		}
	}
}

void AAuraFireBall::EmptyIgnoreList()
{
	IgnoreList.Empty();
}

void AAuraFireBall::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), GameplayCue_FireBlast, CueParams);
	}
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}
