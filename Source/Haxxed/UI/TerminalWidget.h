// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HaxxedWidgetBase.h"
#include "TerminalWidget.generated.h"

/**
 * In-game terminal, can call commands and display results in its log widget.
 */
UCLASS()
class HAXXED_API UTerminalWidget : public UHaxxedWidgetBase
{
	GENERATED_BODY()

	bool bIsSuperUser;

	UPROPERTY(BlueprintSetter = SetGameInstance)
	class UHaxxedGameInstance* GameInstance;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WindowSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Terminal;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Credits;
	
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TerminalPrompt;

	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableTextBox* TerminalLog;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* TerminalLogScroll;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	bool Initialize() override;
	
	UFUNCTION()
	void SwitchToTerminal();

	UFUNCTION(BlueprintCallable)
	void EnterPressed(const FText& Text, ETextCommit::Type CommitType = ETextCommit::OnEnter);

public:
	void Show(UWidget* ToFocus) override;

	void Println(const FString& String);

	FORCEINLINE void Println() { Println(TEXT("")); }

	UFUNCTION(BlueprintCallable)
	void SwitchToCredits();

	FORCEINLINE bool IsSuperUser() const { return bIsSuperUser; }

	FORCEINLINE void SetSuperUser(bool bSet) { bIsSuperUser = bSet; }

	UFUNCTION(BlueprintCallable)
	void SetGameInstance(UHaxxedGameInstance* Instance) { GameInstance = Instance; }
};
