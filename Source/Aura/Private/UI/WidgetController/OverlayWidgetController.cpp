// Copyright Ben Harris


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
	GetAuraPlayerState()->OnPlayerXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXpChanged);
	GetAuraPlayerState()->OnPlayerLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel, bool bLevelUp)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
		});

	GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		
		if (GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		} else
		{
			GetAuraAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
	
		GetAuraAbilitySystemComponent()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				for (const FGameplayTag& Tag : AssetTags)
				{
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}


}

void UOverlayWidgetController::OnXpChanged(const int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPlayerState()->LevelUpInfo.Get();
	checkf(LevelUpInfo, TEXT("Unable to find level Up Info. Please fill out AuraPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
