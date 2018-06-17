// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HaxxedTargetPoint.generated.h"

/**
 * In game target point, not derived from Engine.ATargetPoint
 */
UCLASS()
class HAXXED_API AHaxxedTargetPoint : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	FString InGameName;
	
	UFUNCTION(BlueprintCallable)
	void SetInGameNameAsHierarchyName() { InGameName = GetName(); }


public:
	FORCEINLINE const FString& GetInGameName() const { return InGameName; }

	
};
