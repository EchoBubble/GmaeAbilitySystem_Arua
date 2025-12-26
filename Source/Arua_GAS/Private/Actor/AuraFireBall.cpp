// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/PlayerState.h"

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
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			IgnoreList.Add(OtherActor);
		}
	}
}

void AAuraFireBall::EmptyIgnoreList()
{
	IgnoreList.Empty();
}
