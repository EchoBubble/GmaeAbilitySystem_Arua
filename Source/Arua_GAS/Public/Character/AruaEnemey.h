// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AruaCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AruaEnemey.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_GAS_API AAruaEnemey : public AAruaCharacterBase, public  IEnemyInterface
{
	GENERATED_BODY()

public:
	AAruaEnemey();
	virtual void HighlightActor()override;
	virtual void UnHighlightActor()override;

	/*UPROPERTY(BlueprintReadOnly)
	bool bHightlighted = false;*/
	
};
