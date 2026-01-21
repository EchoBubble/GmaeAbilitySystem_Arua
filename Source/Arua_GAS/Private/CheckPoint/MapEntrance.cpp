// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint/MapEntrance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComponent);
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::LoadActor_Implementation()
{
	// Do nothing when loading a Map Entrance
}

void AMapEntrance::RemoveAllGameplayEffectsFromActor(const AActor* Actor)
{
	if (!IsValid(Actor)) return;

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
	if (!ASC) return;

	// 取消能力 (可选，但建议)
	ASC->CancelAbilities();

	// 获取当前所有激活的 GE 句柄并全部移除
	const TArray<FActiveGameplayEffectHandle> Handles = ASC->GetActiveEffects(FGameplayEffectQuery());
	for (const FActiveGameplayEffectHandle& Handle : Handles)
	{
		// -1 通常表示移除所有stack
		ASC->RemoveActiveGameplayEffect(Handle, -1);
	}

	// 清理 GC (可选)
	ASC->RemoveAllGameplayCues();
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		bReached = true;
		
		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
		
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
