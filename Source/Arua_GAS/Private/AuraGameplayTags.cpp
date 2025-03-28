// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;//c++语法，.h文件中声明了一个static变量，这里必须要实例化

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	//因为函数是static，没有this指针，所以调用armor必须得用GameplayTags调用，否则无法找到对象
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),FString("Reduces damage taken, improves Block Chance"));
}
