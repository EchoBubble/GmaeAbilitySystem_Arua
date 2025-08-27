// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::ClientEffectApplied);
	
	/*const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(-1,
		10.f,FColor::Orange,
		FString::Printf(TEXT("Tags: %s"),*GameplayTags.Attributes_Secondary_Armor.ToString()));*/
	
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		if (const UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))//这里转化目标是AbilitySpec.Ability是因为要获得类默认对象的数据，AbilityClass是纯类型信息
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->StartupInputTag);//通过DynamicAbilityTags添加标签
			GiveAbility(AbilitySpec);
		}
		//GiveAbilityAndActivateOnce(AbilitySpec);//参数不接受const类型
	}
	bStartupAbilities = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())//这里遍历的是所有可激活的能力列表
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))//判断这个容器里面是否有InputTag
		{
			AbilitySpecInputPressed(AbilitySpec);//属于GAS的Pressed和Released，作用是同步信息，增强输入只是玩家层面，这里是GAS同步到服务器
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
	
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	//GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Blue,FString("Effect Applied!"));
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
	
	
}
