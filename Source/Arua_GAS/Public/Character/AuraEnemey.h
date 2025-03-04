// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemey.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_GAS_API AAuraEnemey : public AAuraCharacterBase, public  IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemey();

	//** Enemy Interface **
	virtual void HighlightActor()override;
	virtual void UnHighlightActor()override;
	/*UPROPERTY(BlueprintReadOnly)
	bool bHightlighted = false;*/
	
	//** End Enemy Interface **
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
};
