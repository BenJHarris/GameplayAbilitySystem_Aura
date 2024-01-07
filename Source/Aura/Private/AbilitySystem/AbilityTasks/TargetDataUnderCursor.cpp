// Copyright Ben Harris


#include "AbilitySystem/AbilityTasks/TargetDataUnderCursor.h"

UTargetDataUnderCursor* UTargetDataUnderCursor::CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderCursor* Obj = NewAbilityTask<UTargetDataUnderCursor>(OwningAbility);
	return Obj;
}

void UTargetDataUnderCursor::Activate()
{
	Super::Activate();
	const APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	ValidData.Broadcast(CursorHit.Location);
}
