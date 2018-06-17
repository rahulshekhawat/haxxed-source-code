// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HaxxedWidgetBase.generated.h"

/**
 * Base class for all widgets used in this game.
 */
UCLASS()
class HAXXED_API UHaxxedWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
	bool bIsOnScreen;

	void OnLevelRemovedFromWorld(ULevel* Level, UWorld* World) override;

public:

	FORCEINLINE bool IsOnScreen() const { return bIsOnScreen; }

	virtual void Show(UWidget* ToFocus = nullptr);

	void Hide();

	UFUNCTION(BlueprintCallable)
	void ToggleWidget();
	
};
