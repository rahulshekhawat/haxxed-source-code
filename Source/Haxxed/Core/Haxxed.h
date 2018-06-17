// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * The command parser used by the game's terminal.
 */
struct FHaxxedCmdParser
{
	static bool ParseCommand(FString& Command, class UWorld* World);

	static bool ParseHelper(FString& OutCommand);

	static bool PreParse(FString& OutCommand);
};
