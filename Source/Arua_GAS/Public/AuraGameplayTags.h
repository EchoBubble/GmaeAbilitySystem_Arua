// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * Singleton containing nativeGameplay Tags
 */

struct FAuraGameplayTags
{
public:
	static FAuraGameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;
protected:
	
private:
	static FAuraGameplayTags GameplayTags;	//开辟新的变量，创建全局唯一的实例
};