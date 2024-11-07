// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AruaCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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
