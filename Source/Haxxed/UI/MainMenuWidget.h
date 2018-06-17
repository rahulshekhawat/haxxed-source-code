// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HaxxedWidgetBase.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAXXED_API UMainMenuWidget : public UHaxxedWidgetBase
{
	GENERATED_BODY()
	
	static const FString TrollString;

	static const FString HintString;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreditsButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HintText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StartText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CreditsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ExitText;

	bool Initialize() override;

	UFUNCTION()
	void ChangeText();
	
};
