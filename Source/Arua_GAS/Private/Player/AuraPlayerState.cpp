// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	Attributes = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	NetUpdateFrequency = 100.f;//服务器向客户端发送对象更新频率
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
