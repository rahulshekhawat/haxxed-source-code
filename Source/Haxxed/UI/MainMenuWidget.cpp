// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

const FString UMainMenuWidget::TrollString{ TEXT("Buttons are for pussies.") };
const FString UMainMenuWidget::HintString{ TEXT("You cannot escape!") };

bool UMainMenuWidget::Initialize()
{
	if (Super::Initialize() && StartButton && CreditsButton && ExitButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ChangeText);
		CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ChangeText);
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ChangeText);

		return true;
	}

	return false;
}

void UMainMenuWidget::ChangeText()
{
	StartText->SetText(FText::FromString(TrollString));
	CreditsText->SetText(FText::FromString(TrollString));
	ExitText->SetText(FText::FromString(TrollString));
	HintText->SetText(FText::FromString(HintString));
}
