// Copyright Ben Harris


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown();
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			/* Title */
			"<Title>Fire Bolt</>\n\n"

			/* Details */
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<small>Cooldown: </><Cooldown>%.1f</>\n\n"

			/* Description */
			"<Default>Launches a bolt of fire, exploding on impact and dealing: </><Damage>%d</><Default> Fire damage with a chance to burn</>"),
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage
			);
	}
	return FString::Printf(TEXT(
		/* Title */
		"<Title>Fire Bolt</>\n\n"
		
		/* Details */
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<small>Cooldown: </><Cooldown>%.1f</>\n\n"

		/* Description */
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> Fire damage with a chance to burn</>"),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxProjectiles),
		ScaledDamage);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown();
	return FString::Printf(TEXT(
		/* Title */
		"<Title>NEXT LEVEL</>\n\n"

		/* Details */
		"<small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<small>Cooldown: </><Cooldown>%.1f</>\n\n"

		/* Description */
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> Fire damage with a chance to burn</>"),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxProjectiles),
		ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	const bool bOverridePitch, const float PitchOverride, AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	const int32 NumProjectiles = FMath::Min(MaxProjectiles, GetAbilityLevel());
	
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);
	
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetRotation(Rotator.Quaternion());
		SpawnTransform.SetLocation(SocketLocation);
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		} else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		Projectile->FinishSpawning(SpawnTransform);
	}
}
