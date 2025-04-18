// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::EffectApplied);
	
	/*const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(-1,
		10.f,FColor::Orange,
		FString::Printf(TEXT("Tags: %s"),*GameplayTags.Attributes_Secondary_Armor.ToString()));*/
	
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	//GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Blue,FString("Effect Applied!"));
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
	
	
}
