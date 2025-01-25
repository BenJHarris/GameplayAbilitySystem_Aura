// Copyright Ben Harris

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;
	

	void InitializeSlot();

	void SetLoadSlotName(FString InLoadSlotName);

	FString GetLoadSlotName() const { return LoadSlotName; };
	
	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;

	/** Field Notifies */

	void SetPlayerName(FString InPlayerName);

	FString GetPlayerName() const { return PlayerName; }

	void SetMapName(FString InMapName);

	FString GetMapName() const { return MapName; }

	void SetPlayerLevel(int32 InLevel);

	int32 GetPlayerLevel() const { return PlayerLevel; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	int32 PlayerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	FString LoadSlotName;
	
};
