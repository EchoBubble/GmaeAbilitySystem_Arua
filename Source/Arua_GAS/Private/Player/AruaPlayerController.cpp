// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AruaPlayerController.h"

#include "EnhancedInputComponent.h"
#include "../../../../../../../software_all/unrealEnginAll/UE_5.3/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"

AAruaPlayerController::AAruaPlayerController()
{
	bReplicates = true;
	
}

void AAruaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AruaContext);

	UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AruaContext,0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAruaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
}
