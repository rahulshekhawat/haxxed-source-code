// Fill out your copyright notice in the Description page of Project Settings.

#include "Haxxed.h"
#include "HaxxedGameInstance.h"

#include "Modules/ModuleManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Haxxed, "Haxxed" );


bool FHaxxedCmdParser::PreParse(FString& OutCommand)
{
	OutCommand.ToLowerInline();

	if (OutCommand == TEXT("quit"))
	{
		return true;
	}
	else if (OutCommand == TEXT("help"))
	{
		OutCommand = TEXT("NoobFriendlyHelp");
	}
	
	TChar<FString::ElementType>::ToUpper(OutCommand[0]);
	
	// Propagates to ParseHelper
	return false;
}

bool FHaxxedCmdParser::ParseHelper(FString& OutCommand)
{
	FString Left, Right;
	OutCommand.Split(TEXT(" "), &Left, &Right);

	static TArray<FNativeFunctionLookup>
		HaxxedConsoleUFunctions{ UHaxxedGameInstance::StaticClass()->NativeFunctionLookupTable };
	
	// the command is the left string if run with arguments
	const FName CommandAsFName{ Left.IsEmpty()? *OutCommand : *Left };

	return HaxxedConsoleUFunctions.ContainsByPredicate(
		[&CommandAsFName](const FNativeFunctionLookup& Func) -> bool
		{
			return Func.Name == CommandAsFName;
		}
	);
}

bool FHaxxedCmdParser::ParseCommand(FString& OutCommand, UWorld* World)
{
	OutCommand.TrimStartAndEndInline();

	if (OutCommand.IsEmpty())
	{
		return false;
	}

	bool bIsAHaxxedCommand{ PreParse(OutCommand) || ParseHelper(OutCommand) };

	APlayerController* PlayerController{ World->GetFirstPlayerController() };

	if (PlayerController && bIsAHaxxedCommand)
	{
		PlayerController->ConsoleCommand(*OutCommand);
	}

	return bIsAHaxxedCommand;
}
