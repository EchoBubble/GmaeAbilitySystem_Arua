// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	//GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Blue,FString("Effect Applied!"));
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	for (const FGameplayTag& tag : TagContainer)
	{
		//TODO: Broadcast the tag to the Widget Controller
		const FString Msg = FString::Printf(TEXT("GE Tag: %s"),*tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.0,FColor::Blue,Msg);
		
	}
	
}
