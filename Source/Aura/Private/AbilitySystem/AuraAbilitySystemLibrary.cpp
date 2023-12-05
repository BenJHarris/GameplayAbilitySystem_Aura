// Copyright Ben Harris


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	UOverlayWidgetController* Result = nullptr;
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAuraAbilitySystemComponent* AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent());
			UAuraAttributeSet* AttributeSet = Cast<UAuraAttributeSet>(PlayerState->GetAttributeSet());
			
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			Result = AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return Result;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	UAttributeMenuWidgetController* Result = nullptr;
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAuraAbilitySystemComponent* AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent());
			UAuraAttributeSet* AttributeSet = Cast<UAuraAttributeSet>(PlayerState->GetAttributeSet());
			
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			Result = AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return Result;
}
