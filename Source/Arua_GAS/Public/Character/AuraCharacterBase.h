// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class ARUA_GAS_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface

{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet()const {return Attributes;};

	/* Combat Interface*/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	/* End Combat Interface*/
	
	UFUNCTION(NetMulticast, reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TMap<FGameplayTag, FName> CombatSocketMap;

	bool bDead = false;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> Attributes;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass,float Level)const;

	virtual void InitializeDefaultAttributes()const;

	void AddCharacterAbilities();

	/* Dissolve Effects */

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UNiagaraSystem* BloodEffect;
private:

	UPROPERTY(EditAnywhere,Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere,Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
