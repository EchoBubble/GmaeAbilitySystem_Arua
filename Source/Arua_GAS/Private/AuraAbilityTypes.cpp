#include "AuraAbilityTypes.h"


bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 9;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 11;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 13;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 14;
		}
		if (bShouldHitReact)
		{
			RepBits |= 1 << 15;
		}
		if (!KnockbackForce.IsZero())
		{
			RepBits |= 1 << 16;
		}
		if (bIsRadial)
		{
			RepBits |= 1 << 17;
			
			if (RadialDamageInnerRadius > 0.f)
			{
				RepBits |= 1 << 18;
			}
			if (RadialDamageOuterRadius > 0.f)
			{
				RepBits |= 1 << 19;
			}
			if (RadialDamageOrigin.IsZero())
			{
				RepBits |= 1 << 20;
			}
		}
	}

	Ar.SerializeBits(&RepBits, 21);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		//Actors 是数组，所以需要写额外的元素数量逐个序列化，这是专用的，主要不敌是防止炸包
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());//客户端可能无效，所以要提前 new 一个出来做共享指针
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);//HitResult 专用的 NetSerialize
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 10))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 13))
	{
		if (Ar.IsLoading()) // 如果现在是在“读网络数据”（客户端这一侧）
		{
			if (!DamageType.IsValid())// 指针里还没有对象
			{
				DamageType = MakeShared<FGameplayTag>();//新写法  new 一个 FGameplayTag 出来
			}
		}
		// 不管是读还是写，只要走到这里，就让 FGameplayTag 自己做 NetSerialize
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 14))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15))
	{
		Ar << bShouldHitReact;
	}
	if (RepBits & (1 << 16))
	{
		KnockbackForce.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 17))
	{
		Ar << bIsRadial;
		
		if (RepBits & (1 << 18))
		{
			Ar << RadialDamageInnerRadius;
		}
		if (RepBits & (1 << 19))
		{
			Ar << RadialDamageOuterRadius;
		}
		if (RepBits & (1 << 20))
		{
			RadialDamageOrigin.NetSerialize(Ar, Map, bOutSuccess);
		}
	}
	
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(),EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}
	bOutSuccess = true;
	return true;
}
