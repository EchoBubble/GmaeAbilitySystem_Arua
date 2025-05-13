// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)//判断当前机器是否控制着这个角色，只有客户端 + listen server才会为true
	{
		SendMouseCursorData();
	}
	else
	{
		//TODO: We are on the server,so listen for target data.
	}
	
	
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	//打开预测窗口 ➜ 生成新的 PredictionKey ➜ 存到 ASC->ScopedPredictionKey,默认asc的预测键为空
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorResult;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,CursorResult);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();//打包命中结果
	Data->HitResult = CursorResult;
	DataHandle.Add(Data);//传入给句柄

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);
	//判断是否应该广播，因为有可能这个技能已经取消，打断，结束了，会导致无效，此时继续广播可能会导致选空指针
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}



}
