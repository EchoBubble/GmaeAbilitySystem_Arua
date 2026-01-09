// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FText::FromString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FText::FromString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FText::FromString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	SetNumLoadSlots(LoadSlots.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (const auto& LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
			SelectedLoadSlot = LoadSlots[Slot];
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedLoadSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedLoadSlot->GetLoadSlotName().ToString(), SelectedLoadSlot->SlotIndex);
		SelectedLoadSlot->SlotStatus = Vacant;
		SelectedLoadSlot->InitializeSlot(Vacant);
		SelectedLoadSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (IsValid(SelectedLoadSlot)) AuraGameMode->TravelToMap(SelectedLoadSlot);
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FText& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetMapName(FText::FromString(AuraGameMode->DefaultMapName));
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	
	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot(Taken);

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	AuraGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName().ToString();
	AuraGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}

void UMVVM_LoadScreen::LoadData()
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const auto& LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName().ToString(), LoadSlot.Key);
		
		FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetMapName(FText::FromString(SaveObject->MapName));
		
		if (!PlayerName.IsEmpty()) LoadSlot.Value->SetPlayerName(FText::FromString(PlayerName));
		
		LoadSlot.Value->InitializeSlot(LoadSlot.Value->SlotStatus);
	}
}
