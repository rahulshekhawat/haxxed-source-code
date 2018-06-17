// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/*
	Mesh->SetRenderCustomDepth(false);
	*/
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	InGameName = GetName();
}

FString AInteractable::GetInGameName()
{
	return InGameName.Replace(TEXT("_"), TEXT("."));
}
