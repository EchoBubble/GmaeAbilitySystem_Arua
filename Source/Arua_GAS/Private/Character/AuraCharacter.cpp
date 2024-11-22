// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init Ability Actor info for the sever
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//Init Ability Actor info for the client
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	
	AAuraPlayerState* AruaPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AruaPlayerState);
	AruaPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AruaPlayerState,this);
	AbilitySystemComponent = AruaPlayerState->GetAbilitySystemComponent();
	Attributes = AruaPlayerState->GetAttributeSet();
}
