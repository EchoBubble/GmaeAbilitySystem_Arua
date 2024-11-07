// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AruaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


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
	//绑定组件
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAruaPlayerController::Move);
}

void AAruaPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();//提取移动方向和大小，键盘或摇杆的XY轴
	const FRotator Rotation = GetControlRotation();//获取玩家当前视角
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);//只保留Yaw

	const FVector ForwardDitection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//计算向前的方向
	const FVector RightDitection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//向右方向
	//获取当前角色
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDitection,InputAxisVector.Y);//根据输入的值决定方向和速度   键盘的XY
		ControlledPawn->AddMovementInput(RightDitection,InputAxisVector.X);
	}
	
}
