// Copyright Ben Harris


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	const int32 NumProjectiles = FMath::Min(MaxProjectiles, GetAbilityLevel());
		
	if (NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Direction * 100.f, 5.f, FLinearColor::White, 120.f, 5.f);
		}
	} else
	{
		// Single Projectile
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f, 5.f, FLinearColor::White, 120.f, 5.f);

	}

	
}
