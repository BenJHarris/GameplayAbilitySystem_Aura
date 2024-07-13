// Copyright Ben Harris

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraFirstBlast.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFirstBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category="FireBlast")
	int32 NumFireBalls = 12;
	
};
