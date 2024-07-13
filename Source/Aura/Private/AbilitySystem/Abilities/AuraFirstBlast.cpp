// Copyright Ben Harris


#include "AbilitySystem/Abilities/AuraFirstBlast.h"

FString UAuraFirstBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown();
	return FString::Printf(TEXT(
			/* Title */
			"<Title>Fire Blast</>\n\n"

			/* Details */
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<small>Cooldown: </><Cooldown>%.1f</>\n\n"

			/* Description */
			"<Default>Launches %d fire balls in all directions, each coming back and exploding upon return, causing: </><Damage>%d</><Default> radial fire damage with a chance to burn</>"),
			Level,
			ManaCost,
			Cooldown,
			NumFireBalls,
			ScaledDamage
			);
}

FString UAuraFirstBlast::GetNextLevelDescription(int32 Level)
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
			"<Default>Launches %d fire balls in all directions, each coming back and exploding upon return, causing: </><Damage>%d</><Default> radial fire damage with a chance to burn</>"),
			Level,
			ManaCost,
			Cooldown,
			NumFireBalls,
			ScaledDamage
			);
}
