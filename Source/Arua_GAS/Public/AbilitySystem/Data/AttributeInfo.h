// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();//默认为空标签

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();//描述

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.0f;
	
};


/**
 * 
 */
UCLASS()
class ARUA_GAS_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag,bool bLogNotFound = false)const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;

	
};
