// Copyright Ben Harris

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()

public:

	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* End Save Interface */

protected:

	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	/* End Highlight Interface */

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
