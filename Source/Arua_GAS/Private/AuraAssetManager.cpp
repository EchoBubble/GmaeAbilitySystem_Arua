// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"



UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine)
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);//因为static没有this指针，所以没法访问当前对象，只能去全局那转化为我们的UAssetManager
	return *AuraAssetManager;
	
}
//这个函数会在引擎启动时自动调用，不过整整读取这个AssetManager还需要在ini文件中去配置
void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();//这就是静态函数，不用实例化直接就能调取函数了
	//5.3版本以后不用加，但是5.2需要，这个是初始化数据资产表格，类似字典，这样服务器就根据客户端上传的数据去查找数据编号并储存了，否则直接断联
	UAbilitySystemGlobals::Get().InitGlobalData();
}


