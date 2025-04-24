// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;//可以修改这个指针的值本身，比如赋一个新的指针，只要不是在运行时动态改就行

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};
/**
 * 
 */
UCLASS()
class ARUA_GAS_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
	
};
