// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HaxxedWidgetBase.h"
#include "EndCreditsWidget.generated.h"

/**
 * The widget seen in the end of the game. Similar to the one inside the terminal widget.
 */
UCLASS()
class HAXXED_API UEndCreditsWidget : public UHaxxedWidgetBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuitText;

	bool Initialize() override;

	UFUNCTION()
	void Psyche();
	
};
