// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HaxxedGameInstance.generated.h"

/**
 * Overriden Game Instance, consists of the console functions used in game
 */
UCLASS()
class HAXXED_API UHaxxedGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	bool IsMainMenuLevel() const
	{
		return UGameplayStatics::GetCurrentLevelName(GetWorld()) == MainMenuLevel.ToString();
	}

protected:
	UPROPERTY(BlueprintReadWrite)
	class UTerminalWidget* Terminal;

	UPROPERTY(BlueprintReadWrite)
	class AHaxxedCharacter* Character;
	
	UPROPERTY(EditDefaultsOnly)
	FName MainMenuLevel = TEXT("Level_MainMenu");

	UPROPERTY(EditDefaultsOnly)
	FName PlayableLevel = TEXT("Level_House");

	// The password for accessing Superuser permissions
	static const FString SuPassword;

private:
	UFUNCTION(Exec)
	// Grants Superuser permissions.
	void Su(const FString& Password);

	UFUNCTION(Exec)
	// Move between directories, here in-game target points
	void Cd(const FString& Name);

	UFUNCTION(Exec)
	// The alias used is './'. Lets us view an Interactable by name. Also supports passwords.
	void Less(const FString& Name);

	UFUNCTION(Exec)
	// View all files in a directory, here AInteractable and related objects
	void Ls(const FString& Args);

	UFUNCTION(Exec)
	// Clears the terminal debug log
	void Clear();

	UFUNCTION(Exec)
	// Starts the game.
	void Start();

	// quit is already an in-built command.

	UFUNCTION(Exec)
	// Switches the Terminal for the Credits window, which has a button.
	void Credits();

	UFUNCTION(Exec)
	// Displays hints in case people feel scared.
	void NoobFriendlyHelp();

	// Behaviour executed when a level starts loading
	void BeginLoadingScreen(const FString& InMapName);
	
	// Behaviour executed when a level ends loading
	void EndLoadingScreen(UWorld* InLoadedWorld);
};
