// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_GAS_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()
	
public:
	
	AMapEntrance(const FObjectInitializer& ObjectInitializer);
	
	/* Highlight Interface*/
	virtual void HighlightActor_Implementation() override;
	/* End Highlight Interface*/

	/* Save Interface*/
	virtual void LoadActor_Implementation() override;
	/* End Save Interface*/

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

	/* 该函数只是另一个解决方案，确保切换关卡时状态是干净的，具体详见 EntranceMap 章节 */
	static void RemoveAllGameplayEffectsFromActor(const AActor* Actor);
	/* end */
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComponent,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult) override;
};
