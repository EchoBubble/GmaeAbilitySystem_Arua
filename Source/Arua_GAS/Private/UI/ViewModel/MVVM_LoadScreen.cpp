// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlot.FindChecked(Index);
}
