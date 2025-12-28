// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Arua_GAS/Arua_GAS.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"


AAuraProjectile::AAuraProjectile()
{

	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}


void AAuraProjectile::OnHomingTargetDeath(AActor* DeadActor)
{
	// 敌人死了：直接取消追踪，不再跟着尸体和 ragdoll 乱飞
	ProjectileMovement->bIsHomingProjectile      = false;
	ProjectileMovement->HomingTargetComponent    = nullptr;
	ProjectileMovement->bRotationFollowsVelocity = true;
 	ProjectileMovement->ProjectileGravityScale   = 1.f; // 给点重力，让它自然砸地爆炸

	GetWorld()->GetTimerManager().ClearTimer(HomingTargetTimerHandle);
}

void AAuraProjectile::InitHomingToTarget(AActor* InTarget)
{
	if (!InTarget) return;
	
	ProjectileMovement->bIsHomingProjectile      = true;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale   = 0.f; // 追踪时一般不需要重力

	if (USceneComponent* RootComp = InTarget->GetRootComponent())
	{
		ProjectileMovement->HomingTargetComponent = RootComp;
	}

	// 只在服务器绑死亡委托
	if (HasAuthority())
	{
		if (ICombatInterface* CombatTarget = Cast<ICombatInterface>(InTarget))
		{
			CombatTarget->GetOnDeathDelegate().AddUniqueDynamic(this, &AAuraProjectile::OnHomingTargetDeath);
		}
	}
}

void AAuraProjectile::InitHomingToLocation(const FVector& InLocation)
{
	ProjectileMovement->bIsHomingProjectile      = true;
	ProjectileMovement->bRotationFollowsVelocity = true;

	// 创建一个 SceneComponent 当作“假目标”
	HomingTargetSceneComponent = NewObject<USceneComponent>(this);
	HomingTargetSceneComponent->RegisterComponent();
	HomingTargetSceneComponent->SetWorldLocation(InLocation);

	ProjectileMovement->HomingTargetComponent = HomingTargetSceneComponent;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AAuraProjectile::OnSphereOverlap);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	SetReplicateMovement(true);

	if (HasAuthority() && ProjectileMovement->bIsHomingProjectile)
	{
		GetWorld()->GetTimerManager().SetTimer(
			HomingTargetTimerHandle, 
			this, 
			&AAuraProjectile::CheckHomingTargetStatus, 
			0.1f, 
			true
		);
	}
}

void AAuraProjectile::CheckHomingTargetStatus()
{
	// 1. 如果追踪已经关闭了（比如已经触发过死亡逻辑了），就没必要再查了
	if (!ProjectileMovement->bIsHomingProjectile)
	{
		GetWorld()->GetTimerManager().ClearTimer(HomingTargetTimerHandle);
		return;
	}

	// 2. 死了就直接关闭追踪并启用重力
	if (HomingTargetActor->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_IsDead(HomingTargetActor.Get()))
		{
			OnHomingTargetDeath(HomingTargetActor.Get());
		}
	}
	else
	{
		// 如果 Component 没了（比如被 Destroy 了），也应该停止追踪
		// 可以选择在这里直接清理 Timer，或者调用死亡逻辑
		GetWorld()->GetTimerManager().ClearTimer(HomingTargetTimerHandle);
	}
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}

void AAuraProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (!bHit && !HasAuthority())//特殊情况，客户端未来得及生成就被销毁时走这里补救分支
	{
		OnHit();
	}
	GetWorld()->GetTimerManager().ClearTimer(HomingTargetTimerHandle);
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* SourceAvatarActor = GetOwner();
	if (APlayerState* PS = Cast<APlayerState>(SourceAvatarActor)) SourceAvatarActor = PS->GetPawn();
	if (SourceAvatarActor == OtherActor) return;
	if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor,OtherActor))return;
	
	if (!bHit) OnHit();
	
	if (HasAuthority())//只有服务器才有权威去销毁
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;//投射物方向和冲量大小
			DamageEffectParams.DeathImpulse = DeathImpulse.RotateAngleAxis(-45.f,GetActorRightVector());//设置死亡冲击方向

			const FVector KnockbackDirection = GetActorForwardVector().RotateAngleAxis(-45.f, GetActorRightVector());
			const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
			DamageEffectParams.KnockbackForce = KnockbackForce;
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}



