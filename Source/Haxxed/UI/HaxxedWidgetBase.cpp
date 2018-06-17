// Fill out your copyright notice in the Description page of Project Settings.

#include "HaxxedWidgetBase.h"


void UHaxxedWidgetBase::Show(UWidget* ToFocus)
{
	AddToViewport();

	if (!ToFocus)
	{
		ToFocus = this;
	}

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(ToFocus->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);

	UWorld* World{ GetWorld() };

	if (World)
	{
		APlayerController* PlayerController{ World->GetFirstPlayerController() };

		if (PlayerController)
		{
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
		}
	}

	bIsOnScreen = true;
}

void UHaxxedWidgetBase::Hide()
{
	FInputModeGameOnly InputMode;

	UWorld* World{ GetWorld() };

	if (World)
	{
		APlayerController* PlayerController{ World->GetFirstPlayerController() };

		if (PlayerController)
		{
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
		}
	}

	RemoveFromViewport();

	bIsOnScreen = false;
}

void UHaxxedWidgetBase::OnLevelRemovedFromWorld(ULevel* Level, UWorld* World)
{
	Hide();

	Super::OnLevelRemovedFromWorld(Level, World);
}

void UHaxxedWidgetBase::ToggleWidget()
{
	IsOnScreen()? Hide() : Show();
}

