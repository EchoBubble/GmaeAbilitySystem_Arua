// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AuraUserWiget.h"

void UAuraUserWiget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
