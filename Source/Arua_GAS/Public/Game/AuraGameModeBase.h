// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UCharacterClassInfo;
class UAbilityInfo;
/**
 * 
 */
UCLASS()
class ARUA_GAS_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityInfo")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;
};
