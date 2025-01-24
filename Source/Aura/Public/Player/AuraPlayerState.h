// Copyright Ben Harris

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32); /* Stat Value */

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/* XP */
	void SetXP(const int32 InXP);
	void AddToXP(const int32 InXP);
	FORCEINLINE int32 GetXP() const { return  XP; }
	FOnPlayerStatChanged OnPlayerXPChangedDelegate;
	/* End XP */

	/* Level */
	void SetLevel(const int32 InLevel);
	void AddToLevel(const int32 InLevel);
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FOnPlayerStatChanged OnPlayerLevelChangedDelegate;
	/* End Level */

	/* Attribute Points */
	void SetAttributePoints(const int32 InAttributePoints);
	void AddToAttributePoints(const int32 InAttributePoints);
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	FOnPlayerStatChanged OnPlayerAttributePointsChangedDelegate;
	/* End Attribute Points */
	
	/* Spell Points */
	void SetSpellPoints(const int32 InSpellPoints);
	void AddToSpellPoints(const int32 InSpellPoints);
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FOnPlayerStatChanged OnPlayerSpellPointsChangedDelegate;
	/* End Spell Points */

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	/* XP */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	/* End XP */

	/* Level */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	/* End Level */

	/* Attribute Points */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	/* End Attribute Points */

	/* Spell Points */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0;

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	/* End Spell Points */
	
};
