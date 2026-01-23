// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Arua_GAS/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "GameFramework/Character.h"

void AAuraGameModeBase::SaveSlotData(const UMVVM_LoadSlot* LoadSlot, int32 SlotIndex) const
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName().ToString(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName().ToString(), SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName().ToString();
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName().ToString();
	LoadScreenSaveGame->MapAssetName = LoadSlot->GetMapName().ToString();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName().ToString(), SlotIndex);
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData() const
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	const FString LoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 LoadSlotIndex = AuraGameInstance->LoadSlotIndex;

	return GetSaveSlotData(LoadSlotName, LoadSlotIndex);
}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject) const
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	const FString LoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 LoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, LoadSlotName, LoadSlotIndex);
}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Pair: Maps)
	{
		if (Pair.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)//如果地图的名字和传参一致，返回 key，也就是该地图名称
		{
			return Pair.Key;
		}
	}
	return FString();
}

void AAuraGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	const ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;

	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}

void AAuraGameModeBase::SaveWorldState(const UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);//移除前缀用的

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);
	
	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			//之前 save game 的地图名字都是开局保存的默认名字，这里是指当有传参，也就是在 EntranceMap 时替换为新地图的名字，下次开局，存档也能正确显示
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		if (!SaveGame->HasMap(WorldName))//如果当前保存列表没有该地图
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}

		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);//这里是拷贝版本 && 此外因为上面的逻辑，这里必然能找到
		SavedMap.SavedActors.Empty();

		for (FActorIterator It(World); It; ++It)//遍历场景中加载的所有物体
		{
			AActor* Actor = *It;//解引用当前迭代器指向的 Actor

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;//无效或者没有实现接口就跳过

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWriter(SavedActor.Bytes);//SavedActor.Bytes等于是白纸，FMemoryWriter等于写入器，两者进行绑定

			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);//写入规则
			Archive.ArIsSaveGame = true;//代表只处理标记了 UPROPERTY(SaveGame) 的成员，不然可能会写很多不想存的东西

			Actor->Serialize(Archive);//按照 UE 规则将 Actor 的变量打包写入 Archive

			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		//前面做的操作只是对拷贝的副本做修改，所以这里要把修改后的内容放回 SaveGame 中
		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
	}
}

void AAuraGameModeBase::LoadWorldState(const UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogAura, Error, TEXT("Failed to load slot"))
			return;
		}
		
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					FMemoryReader MemoryReader(SavedActor.Bytes);//和保存不一样，这里是读取器

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);//二进制转换回节点变量，从 Archive 取出并塞回 Actor

					ISaveInterface::Execute_LoadActor(Actor);//判断是否高亮
				}
			}
		}
	}
}

void AAuraGameModeBase::TravelToMap(const UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName().ToString();
	const int32 SlotIndex = Slot->SlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName().ToString()));
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectedActor = Actor;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
