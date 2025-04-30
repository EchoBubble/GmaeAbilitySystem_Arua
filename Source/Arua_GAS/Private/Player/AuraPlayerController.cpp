// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;//启用对象的网络同步

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;//最后一个Actor就是现在的Actor
	//ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());//脚本接口，转换没有必要
	ThisActor = CursorHit.GetActor();


	/*
		LineTrace from cursor .  there are several scenario
		A. LastActor is null && ThisActor is null
			--Do Nothing
		B. LastActor is null && ThisActor is valid
		    --Highlight ThisActor
		C. LastActor is valid && ThisActor is null
		    --UnHighlight LastActor
		D. Both actor is valid && LastActor != ThisActor
			--UnHighlight LastActor && Highlight ThisActor
		E. Both Actor is valid && they are same actor
		    --Do Nothing
	*/

	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			//case B
			ThisActor->HighlightActor();
		}
		else
		{
			//case A     Both are null do nothing
		}
	}
	else  //LastActor is valid
	{
		if(ThisActor == nullptr)
		{
			//case C    
			LastActor->UnHighlightActor();
		}
		else // Both Actor is valid
		{
			if (LastActor != ThisActor)
			{
				//case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//case D
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;//还未确定是不是short press，所以自动跑设置为假
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetTimeSeconds();
		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility,false,Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}
		if (APawn* ControllerPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AruaContext);

	UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//check(Subsystem);
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AruaContext,0);
	}
	

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	UAuraInputComponent* AuraInputComponent = Cast<UAuraInputComponent>(InputComponent);
	//绑定组件
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
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


