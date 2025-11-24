// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

using namespace AuraGameplayTags;

struct AuraDamageStatics
{
	//声明一个捕获定义，名字得和AS中的属性名一致
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		//这个宏会根据传入的设置找到目标身上的属性，写构造是因为要实时运行
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage, Source, false)
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance, Target, false)

		//const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	
		TagsToCaptureDefs.Add(Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Attributes_Secondary_BlockChance,BlockChanceDef);
		TagsToCaptureDefs.Add(Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

		TagsToCaptureDefs.Add(Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Attributes_Resistance_Physical, PhysicalResistanceDef);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;//返回全局唯一实例
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//将捕获的定义连带设置传递数组，不然GAS读不到数据
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}


void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParameters
	) const
{
	//const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	for (TPair<FGameplayTag, FGameplayTag> Pair : DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType,false, -1.f);
		if (TypeDamage > -0.5f) //.5 padding for floating point [im]precision
		{
			// Determine if there was a successful Debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(Debuff_Chance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = DamageTypesToResistance[DamageType];//从指定的伤害类型得到对应抗性的标签
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
				DamageStatics().TagsToCaptureDefs[ResistanceTag],EvaluateParameters,TargetDebuffResistance);//这里捕获对应抗性的数值
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0);//以防是负值
			const float EffectiveDebuffChance = SourceDebuffChance * (100.f - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1,100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
				
				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(EffectContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageType(EffectContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(Debuff_Damage, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(Debuff_Frequency, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(Debuff_Duration, false, -1.f);

				UAuraAbilitySystemLibrary::SetDebuffDamage(EffectContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(EffectContextHandle, DebuffFrequency);
				UAuraAbilitySystemLibrary::SetDebuffDuration(EffectContextHandle, DebuffDuration);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//获取源和目标的ASC组件，EC只能在GE应用到目标时才能获取TargetASC
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	//获取源和目标的角色实例，也就是场景中实际存在的角色
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceActor->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceActor);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetActor->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetActor);
	}

	//获取当前GE的实例
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	//通过GE实例获取源和目标的角色以及GE上的所有标签Tag
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//评估器，将源和目标的tag全部传递进来
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Debuff
	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters);

	//Get Damage Set by Caller Magnitude/通过Set by Caller Magnitude获取伤害值，这里需要确保tag和之前设置的一致
	float DamageValue = 0.f;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : DamageTypesToResistance)
	{
		//遍历顺序和在标签中添加的顺序一致
		const FGameplayTag DamageTagTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		
		checkf(DamageStatics().TagsToCaptureDefs.Contains(ResistanceTag),
			TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = (DamageStatics().TagsToCaptureDefs[ResistanceTag]);

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTagTag,false);//根据遍历，只有设置了的才有数据，否则为 0
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f , 100.f);
		DamageTypeValue *= ( 100.f - Resistance ) / 100.f;
		
		DamageValue += DamageTypeValue;
	}
	
	//Capture BlockChance on Target, and determine if there was a successful Block/捕获目标格挡几率，确定是否成功阻
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParameters,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	//If Block, halve the damage.阻挡成功，减半伤害
	DamageValue = bBlocked ? DamageValue/2.f : DamageValue;
	
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters,TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceActor);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	
	//ArmorPenetration ignores a percentage of the Target's Armor/护甲穿透将会忽略一定比例的护甲
	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	//Armor ignores a  percentage of incoming Damage.护甲会抵消一定比例的伤害
	DamageValue *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParameters,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,EvaluateParameters,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluateParameters,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"),FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);
	
	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage|暴击百分比会根据暴击抗性而减少
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,bCriticalHit);

	//Double damage plus a bonus if critical hit|双倍伤害，若造成致命一击则额外获得奖励。
	DamageValue = bCriticalHit ? 2.f * DamageValue + SourceCriticalHitDamage : DamageValue;

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive, DamageValue);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
