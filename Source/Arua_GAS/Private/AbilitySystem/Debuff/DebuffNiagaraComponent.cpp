// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"


UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffChanged);
	}
	else if (CombatInterface)
	{
		// weak lambda 防止对象死亡后还在调用导致崩溃
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this,
			[this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffChanged);
			});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this,&UDebuffNiagaraComponent::OnOwnerDeath);
	}
}

void UDebuffNiagaraComponent::DebuffChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	if (NewCount > 0 && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
