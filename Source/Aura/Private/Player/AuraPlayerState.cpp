// Copyright Ben Harris


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

void AAuraPlayerState::SetXP(const int32 InXP)
{
	XP = InXP;
	OnPlayerXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToXP(const int32 InXP)
{
	XP += InXP;
	OnPlayerXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::SetLevel(const int32 InLevel)
{
	Level = InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(Level, false);
}

void AAuraPlayerState::AddToLevel(const int32 InLevel)
{
	Level += InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(Level, true);
}

void AAuraPlayerState::SetAttributePoints(const int32 InAttributePoints)
{
	AttributePoints = InAttributePoints;
	OnPlayerAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToAttributePoints(const int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnPlayerAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSpellPoints(const int32 InSpellPoints)
{
	SpellPoints = InSpellPoints;
	OnPlayerSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::AddToSpellPoints(const int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnPlayerSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnPlayerLevelChangedDelegate.Broadcast(Level, true);
}

void AAuraPlayerState::OnRep_XP(const int32 OldXP)
{
	OnPlayerXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnPlayerAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnPlayerSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
