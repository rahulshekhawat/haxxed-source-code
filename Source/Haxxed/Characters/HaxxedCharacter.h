// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HaxxedCharacter.generated.h"

UCLASS()
class HAXXED_API AHaxxedCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool bMove;
	FVector TargetPointLocation;

	bool bRotate;
	bool bSpin360;
	FRotator TargetRotation;
	
	UPROPERTY(Category = CharacterComponent, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

public:
	// Sets default values for this character's properties
	AHaxxedCharacter();

	static FRotator UnsafeRInterpConstantTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed);

protected:
	UPROPERTY(EditAnywhere, Category = "EnvironmentDetection")
	float TargetPointDetectionRange = 5000.f;

	UPROPERTY(EditAnywhere, Category = "EnvironmentDetection")
	float InteractableDetectionRange = 500.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	Convert angle from range of 0/360 to -180/180.
	Controller rotation yaw / pitch are in between 0 / 360 whereas character rotation are in between - 180 / 180
	*/
	float WrapAngleTo180(float Angle);

	/*
	Needs to be called from a function that is called every tick, i.e., AxisInput functions, Character Tick, or AnimInstance::NativeUpdateAnimation
	Don't call from character Tick if custom tickrate for character will be used.
	The function returns true when character has successfully reached the desired location. Stop function call when function returns true.
	*/
	bool DeltaMoveToLocation(FVector Location, float DeltaTime);
	bool DeltaRotateToRotation(FRotator Rotation, float DeltaTime);

	template<typename ActorType = AActor>
	// templated version for compactness
	TSet<ActorType*> DetectAllActorsOfType(float Range);

	 UFUNCTION(BlueprintCallable, category="EnvironmentDetection")
	// UFunctions to extend behaviour of DetectAllActorsOfType() in blueprints
	TSet<class AInteractable*> DetectAllInteractablesAroundPlayer();

	UFUNCTION(BlueprintCallable, category = "EnvironmentDetection")
	// UFunctions to extend behaviour of DetectAllActorsOfType() in blueprints
	TSet<class AHaxxedTargetPoint*> DetectAllMoveToLocations();

	// Move Character to a desired location. Function returns false if player is unable to move to desired location right now.
	UFUNCTION(BlueprintCallable)
	bool MoveCharacterToLocation(FVector Destination);

	UFUNCTION(BlueprintCallable)
	void FocusOnInteractable(AInteractable* Interactable);

	// Made it to test rotation
	UFUNCTION(BlueprintCallable)
	void LookAtTarget(FVector Target);

	void SpinAround();
	
	UFUNCTION(BlueprintImplementableEvent)
	void MoveCharToLocation(FVector Destination);

	void CameraYawInput(float Value);
	//void CameraPitchInput(float Value);
};
