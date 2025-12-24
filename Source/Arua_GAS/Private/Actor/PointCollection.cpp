// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#define CREATE_AND_SETUP_PT(Pt_X) \
Pt_X = CreateDefaultSubobject<USceneComponent>(TEXT(#Pt_X)); \
ImmutablePts.Add(Pt_X); \
Pt_X->SetupAttachment(GetRootComponent());

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	ImmutablePts.Add(Pt_0);
	SetRootComponent(Pt_0);
	
	CREATE_AND_SETUP_PT(Pt_1);
	CREATE_AND_SETUP_PT(Pt_2);
	CREATE_AND_SETUP_PT(Pt_3);
	CREATE_AND_SETUP_PT(Pt_4);
	CREATE_AND_SETUP_PT(Pt_5);
	CREATE_AND_SETUP_PT(Pt_6);
	CREATE_AND_SETUP_PT(Pt_7);
	CREATE_AND_SETUP_PT(Pt_8);
	CREATE_AND_SETUP_PT(Pt_9);
	CREATE_AND_SETUP_PT(Pt_10);
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,float YawOverride)
{
	// 该断言是确保数组数量大于限制的数量，防止有人误改
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("Attempted to access ImmutablePts out of bounds."))

	TArray<USceneComponent*> ArrayCopy;

	for (USceneComponent* Pt : ImmutablePts)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;//呵呵，最终数量都大于限定数量了，还有啥必要继续

		if (Pt != Pt_0)//原点不参与变换
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();//原点指向点的方向和长度
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaiseLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
		
		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;//这里命名可能会有误导性，这里不是输入，而是输出，指获取到的这些角色被排除后面的射线检测
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(
			this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		//GetWorld()->LineTraceSingleByProfile(HitResult, RaiseLocation, LoweredLocation, FName("BlockAll"), QueryParams);
		const bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,RaiseLocation,LoweredLocation,ECC_Visibility,QueryParams);


		if (bHit && HitResult.bBlockingHit)
		{
			const FVector AdjustedLocation = FVector(RaiseLocation.X, RaiseLocation.Y, HitResult.ImpactPoint.Z);//到这一步就得到正确的高度了
			Pt->SetWorldLocation(AdjustedLocation);
			Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
		}

		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}


void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}


