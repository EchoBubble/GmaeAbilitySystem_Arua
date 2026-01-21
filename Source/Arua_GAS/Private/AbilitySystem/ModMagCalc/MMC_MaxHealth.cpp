// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "Player/AuraPlayerState.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();//获得要捕获的属性
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;//从目标身上获取属性
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;//聚合器，将之前得到的源和目标tags添加进来
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluateParameters,Vigor);//这里相当于是把捕获的属性传递出去并且知道效果规格上下文和所有的标签，判断是否有buff
	Vigor = FMath::Max<float>(Vigor,0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;//返回最终自定义的数值
}

FOnExternalGameplayModifierDependencyChange* UMMC_MaxHealth::GetExternalModifierDependencyMulticast(
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
	
	/*// 优先从 SourceObject 找角色/控制器，再取 PlayerState
	if (const UObject* SourceObj = Spec.GetContext().GetSourceObject())
	{
		if (const AActor* SourceActor = Cast<AActor>(SourceObj))
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(const_cast<AActor*>(SourceActor)))
			{
				//这里必须返回委托实例，不需要绑定 lambda 等，这里引擎会自己处理
				return &Cast<UAuraAbilitySystemComponent>(ASC)->OnModifierDependencyChanged;
			}
		}
	}*/

	// 兜底
	return Super::GetExternalModifierDependencyMulticast(Spec, World);
}
