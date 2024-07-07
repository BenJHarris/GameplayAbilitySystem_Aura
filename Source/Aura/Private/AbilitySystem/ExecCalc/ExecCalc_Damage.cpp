// Copyright Ben Harris


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DamageStatics;
	return DamageStatics;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	const AuraDamageStatics& DS = DamageStatics();
	RelevantAttributesToCapture.Add(DS.ArmorDef);
	RelevantAttributesToCapture.Add(DS.BlockChanceDef);
	RelevantAttributesToCapture.Add(DS.ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DS.CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DS.CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DS.CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DS.FireResistanceDef);
	RelevantAttributesToCapture.Add(DS.LightningResistanceDef);
	RelevantAttributesToCapture.Add(DS.ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DS.PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	for (const TTuple<FGameplayTag, FGameplayTag> Pair: GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, 0.f);
		if (TypeDamage > 0.f)
		{
			// Determine if there was a successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, 0.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluateParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;

			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, 0.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, 0.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, 0.f);

				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
				
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const AuraDamageStatics& DS = DamageStatics();
	const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DS.ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DS.ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DS.BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DS.CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DS.CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DS.CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DS.FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DS.LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DS.ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DS.PhysicalResistanceDef);

	
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}

	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Debuff
	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters, TagsToCaptureDefs);

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;

	for (const TTuple<FGameplayTag, FGameplayTag> Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs does not contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;

		if (DamageTypeValue <= 0)
		{
			continue;
		}
		
		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			DamageTypeValue = UAuraAbilitySystemLibrary::GetRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,
				0.f,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f);
		}
		
		
		Damage += DamageTypeValue;
	}
	
	// Block Chance

	float TargetBlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DS.BlockChanceDef, EvaluateParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	const bool bBlocked = FMath::RandRange(0, 100) < TargetBlockChance;

	// Set blocked on the effect context
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	if (bBlocked)
	{
		Damage = Damage / 2.f;
	}

	// Armor Penetration

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DS.ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	// Armor

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DS.ArmorDef, EvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	EffectiveArmor = FMath::Max<float>(EffectiveArmor, 0.f);

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);

	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	// Critical Hit

	float SourceCriticalChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DS.CriticalHitChanceDef, EvaluateParameters, SourceCriticalChance);
	SourceCriticalChance = FMath::Max(SourceCriticalChance, 0.f);

	float SourceCriticalHitDamage = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DS.CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(SourceCriticalHitDamage, 0.f);

	float TargetCriticalHitResistance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DS.CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max(TargetCriticalHitResistance, 0.f);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	const float EffectiveCriticalHitChance = SourceCriticalChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	bool bCriticalHit = FMath::RandRange(1, 100) <= EffectiveCriticalHitChance;

	// Set critical hit on the effect context
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	if (bCriticalHit)
	{
		Damage = 2.f * Damage + SourceCriticalHitDamage;
	}
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
