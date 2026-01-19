// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TargetingSphere = CreateDefaultSubobject<USphereComponent>("TargetingSphere");
	TargetingSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TargetingSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TargetingSphere->SetGenerateOverlapEvents(true);
	TargetingSphere->SetupAttachment(GetRootComponent());
	SetRootComponent(TargetingSphere);

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
	TargetingSphere->OnComponentBeginOverlap.AddDynamic(this, &AMagicCircle::OnTargetingBeingOverlap);
	TargetingSphere->OnComponentEndOverlap.AddDynamic(this, &AMagicCircle::OnTargetingEndOverlap);
}

void AMagicCircle::OnTargetingBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UEnemyInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(OtherActor);
	}
}

void AMagicCircle::OnTargetingEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UEnemyInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(OtherActor);
	}
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

