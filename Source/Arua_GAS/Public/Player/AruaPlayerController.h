// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../../../../software_all/unrealEnginAll/UE_5.3/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "AruaPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class ARUA_GAS_API AAruaPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAruaPlayerController();
protected:
	virtual void BeginPlay() override;
	virtual  void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AruaContext;
};
