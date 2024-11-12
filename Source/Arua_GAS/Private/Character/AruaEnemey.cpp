// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AruaEnemey.h"

#include "AbilitySystem/AruaAbilitySystemComponent.h"
#include "Arua_GAS/Arua_GAS.h"

AAruaEnemey::AAruaEnemey()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAruaAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	Attributes = CreateDefaultSubobject<UAttributeSet>("AttributeSet");
}

void AAruaEnemey::HighlightActor()
{
	//bHightlighted = true;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAruaEnemey::UnHighlightActor()
{
	//bHightlighted = false;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
