// Copyright Ben Harris


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
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
			Damage
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
		Damage);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
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
		Damage);
}