// Copyright Ben Harris


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetAuraAbilitySystemComponent()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetAuraPlayerState()->OnPlayerSpellPointsChangedDelegate.AddLambda([this](int32 Points)
	{
		SpellPointsChangedDelegate.Broadcast(Points);
	});
	
}

void USpellMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	BroadcastAbilityInfo();

	SpellPointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(Tags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);

	if (!bTagValid || bTagNone || !AbilitySpec)
	{
		AbilityStatus = Tags.Abilities_Status_Locked;
	} else
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusFromSpec(*AbilitySpec);
	}

	bool bShouldEnableSpellPointsButton;
	bool bShouldEnableEquipButton;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bShouldEnableSpellPointsButton, bShouldEnableEquipButton);
	SpellGlobeSelectedDelegate.Broadcast(bShouldEnableSpellPointsButton, bShouldEnableEquipButton);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& Status, int32 SpellPoints,
	bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	
	if (Status.MatchesTagExact(Tags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	} else if (Status.MatchesTagExact(Tags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	} else if (Status.MatchesTagExact(Tags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
