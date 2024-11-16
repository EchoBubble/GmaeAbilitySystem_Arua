// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AruaPlayerState.h"

#include "AbilitySystem/AruaAbilitySystemComponent.h"
#include "AbilitySystem/AruaAttributeSet.h"

AAruaPlayerState::AAruaPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAruaAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	Attributes = CreateDefaultSubobject<UAttributeSet>("AttributeSet");
	NetUpdateFrequency = 100.f;//服务器向客户端发送对象更新频率
}

UAbilitySystemComponent* AAruaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
