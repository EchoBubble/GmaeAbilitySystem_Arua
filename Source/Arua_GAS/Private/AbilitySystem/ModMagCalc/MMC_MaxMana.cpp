// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "Player/AuraPlayerState.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
	
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef,Spec,EvaluateParameters,Intelligence);
	Intelligence = FMath::Max<float>(Intelligence,0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 50.f + 2.5f * Intelligence + 15.f * PlayerLevel;
	
}

FOnExternalGameplayModifierDependencyChange* UMMC_MaxMana::GetExternalModifierDependencyMulticast(
	const FGameplayEffectSpec& Spec, UWorld* World) const
{
	if (UAbilitySystemComponent* InstigatorASC = Spec.GetContext().GetInstigatorAbilitySystemComponent())
	{
		if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(InstigatorASC))
		{
			return AuraASC->GetOnModifierDependencyChanged();
		}
	}
	
	// 问题发现于 EntranceMap 章节
	// source object 可能在退出、切换关卡时先销毁或不可用，这样就可能导致 GE 生效期间找不到指针，无法返回原本的地址，于是就触发断点
	
	/*if (const UObject* SourceObj = Spec.GetContext().GetSourceObject())
	{
		if (const AActor* SourceActor = Cast<AActor>(SourceObj))
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(const_cast<AActor*>(SourceActor)))
			{
				return &Cast<UAuraAbilitySystemComponent>(ASC)->OnModifierDependencyChanged;
			}
		}
	}*/
	
	return Super::GetExternalModifierDependencyMulticast(Spec, World);
}
