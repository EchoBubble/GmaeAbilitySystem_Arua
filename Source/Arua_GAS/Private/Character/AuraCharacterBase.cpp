// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Weapon->SetRelativeScale3D(FVector(1,1,1));
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void AAruaCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAruaCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/

