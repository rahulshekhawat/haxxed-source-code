// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class HAXXED_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	bool bRequiresSu;

	UPROPERTY(BlueprintReadWrite)
	FString Description;

	UPROPERTY(BlueprintReadWrite)
	FString InGameName;

public:
	/*
		For outlines to work-
		* Put the entire level inside a postprocess volume.
		* Set PP_Outliner_Inst as postprocess material.
	*/

	UFUNCTION(BlueprintImplementableEvent)
	void ShowOutline();

	UFUNCTION(BlueprintImplementableEvent)
	void HideOutline();

	// Renamed function from 'GetName' to 'GetInteractableName' to avoid conflict with AActor's inherited GetName function
	UFUNCTION(BlueprintCallable)
	FString GetInGameName();

	UFUNCTION(BlueprintCallable)
	void SetInGameName(FString Name) { InGameName = Name; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteract();
	
	FORCEINLINE bool RequiresSu() const { return bRequiresSu; }

	FORCEINLINE const FString& GetDescription() const { return Description; }
};
