// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

/**
 * 
 */
UCLASS()
class ARUA_GAS_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	void InitializeSlot(ESaveSlotStatus InStatus);
	
	UPROPERTY()
	FString SlotIndex;

	/* Filed Notifies */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter);
	FText PlayerName;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	void SetPlayerName(const FText& InPlayerName);
	void SetLoadSlotName(const FText& InLoadSlotName);
	
	FText GetPlayerName() const {return PlayerName;};
	FText GetLoadSlotName() const {return LoadSlotName;}

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"));
	FText LoadSlotName;

};
