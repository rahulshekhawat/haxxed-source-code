// Fill out your copyright notice in the Description page of Project Settings.

#include "TerminalWidget.h"
#include "Core/Haxxed.h"

#include "Components/MultiLineEditableTextBox.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Engine/World.h"


bool UTerminalWidget::Initialize()
{
	if (Super::Initialize() && Terminal && Credits && TerminalPrompt && TerminalLog && BackButton)
	{
		TerminalPrompt->OnTextCommitted.AddDynamic(this, &UTerminalWidget::EnterPressed);
		BackButton->OnClicked.AddDynamic(this, &UTerminalWidget::SwitchToTerminal);
		return true;
	}

	return false;
}

void UTerminalWidget::Show(UWidget* ToFocus)
{
	Super::Show(TerminalPrompt);
}

void UTerminalWidget::Println(const FString& String)
{
	if (!TerminalLog)
	{
		return;
	}
	else if (String.IsEmpty())
	{
		TerminalLog->SetText(FText::GetEmpty());
	}
	else
	{
		FString TerminalLogContent = TerminalLog->GetText().ToString();
		FString ToPrint{ FString::Printf(
			TEXT("%s\n%s %s\n"),
			*TerminalLogContent,
			bIsSuperUser? TEXT("#") : TEXT(">"),
			*String
		) 
		};

		TerminalLog->SetText(FText::FromString(ToPrint));
		TerminalLogScroll->ScrollToEnd();
	}
}

void UTerminalWidget::EnterPressed(const FText& Text, ETextCommit::Type CommitType)
{
	FString Command{ Text.ToString() };

	if (!FHaxxedCmdParser::ParseCommand(Command, GetWorld()) && !Command.IsEmpty())
	{
		Println(Command + TEXT(": incorrect command!"));
	}

	TerminalPrompt->SetText(FText::GetEmpty());
}

void UTerminalWidget::SwitchToCredits()
{
	if (WindowSwitcher && Credits)
	{
		WindowSwitcher->SetActiveWidget(Credits);
	}
}

void UTerminalWidget::SwitchToTerminal()
{
	if (WindowSwitcher && Terminal)
	{
		WindowSwitcher->SetActiveWidget(Terminal);
	}
}
