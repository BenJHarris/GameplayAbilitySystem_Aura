// Copyright Ben Harris


#include "AbilitySystem/Abilities/AuraFireBolt.h"

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