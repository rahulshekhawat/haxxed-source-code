// Fill out your copyright notice in the Description page of Project Settings.

#include "EndCreditsWidget.h"
#include "Core/Haxxed.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


bool UEndCreditsWidget::Initialize()
{
	if (Super::Initialize() && QuitButton && QuitText)
	{
		QuitButton->OnClicked.AddDynamic(this, &UEndCreditsWidget::Psyche);
		return true;
	}

	return false;
}

void UEndCreditsWidget::Psyche()
{
	QuitText->SetText(FText::FromString(TEXT("PsYcHe!! quit FrOm cOnSolE n00b!!")));
}
