// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AruaCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AruaAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AruaPlayerState.h"

AAruaCharacter::AAruaCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	
}

void AAruaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init Ability Actor info for the sever
	InitAbilityActorInfo();
}

void AAruaCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//Init Ability Actor info for the client
	InitAbilityActorInfo();
}

void AAruaCharacter::InitAbilityActorInfo()
{
	
	AAruaPlayerState* AruaPlayerState = GetPlayerState<AAruaPlayerState>();
	check(AruaPlayerState);
	AruaPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AruaPlayerState,this);
	AbilitySystemComponent = AruaPlayerState->GetAbilitySystemComponent();
	Attributes = AruaPlayerState->GetAttributeSet();
}
