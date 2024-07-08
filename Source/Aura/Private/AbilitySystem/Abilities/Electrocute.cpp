// Copyright Ben Harris


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown();
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			/* Title */
			"<Title>Electrocute</>\n\n"

			/* Details */
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<small>Cooldown: </><Cooldown>%.1f</>\n\n"

			/* Description */
			"<Default>Emits a beam of lightning, connecting with the target, repeatedly causing </><Damage>%d</><Default> Lightning damage with a chance to stun</>"),
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage
			);
	}
	return FString::Printf(TEXT(
		/* Title */
		"<Title>Electrocute</>\n\n"
		
		/* Details */
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<small>Cooldown: </><Cooldown>%.1f</>\n\n"

		/* Description */
		"<Default>Emits a beam of lightning, propogating to %d additional targets nearby, causing </><Damage>%d</><Default> Lightning damage with a chance to stun</>"),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShockTargets - 1),
		ScaledDamage);
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown();
	return FString::Printf(TEXT(
		/* Title */
		"<Title>NEXT LEVEL</>\n\n"
		
		/* Details */
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<small>Cooldown: </><Cooldown>%.1f</>\n\n"

		/* Description */
		"<Default>Emits a beam of lightning, propogating to %d additional targets nearby, causing </><Damage>%d</><Default> Lightning damage with a chance to stun</>"),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShockTargets - 1),
		ScaledDamage);
}
