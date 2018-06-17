// Fill out your copyright notice in the Description page of Project Settings.

#include "HaxxedCharacter.h"
#include "Interactables/Interactable.h"
#include "Interactables/HaxxedTargetPoint.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AHaxxedCharacter::AHaxxedCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	PlayerCamera->bUsePawnControlRotation = true;

	/*
	Since we're using AddMovementInput(Direction, Value) to move the character, 
	acceleration must be set high enough for player to stop instantly when we stop adding movement input.
	*/
	GetCharacterMovement()->MaxAcceleration = 10000.f;
	// GetCharacterMovement()->bOrientRotationToMovement = true;

	bMove = false;

}

// Called when the game starts or when spawned
void AHaxxedCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHaxxedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMove)
	{
		bMove = !(DeltaMoveToLocation(TargetPointLocation, DeltaTime));
	}

	if (bRotate)
	{
		bRotate = bSpin360 = !(DeltaRotateToRotation(TargetRotation, DeltaTime));
	}
}

template<typename ActorType>
TSet<ActorType*> AHaxxedCharacter::DetectAllActorsOfType(float Range)
{
	TSet<ActorType*> ReturnSet;
	FCollisionShape Shape = FCollisionShape::MakeSphere(Range);
	TArray<FHitResult> SweepResults;

	bool bResult = GetWorld()->SweepMultiByChannel(
		SweepResults,
		GetActorLocation(),
		GetActorLocation(),
		GetActorRotation().Quaternion(),
		ECC_Camera,
		Shape
	);
	if (bResult)
	{
		for (const FHitResult& SweepResult : SweepResults)
		{
			ActorType* TestObject{ Cast<ActorType>(SweepResult.GetActor()) };
			if (TestObject)
			{
				ReturnSet.Add(TestObject);
			}
		}
	}

	return ReturnSet;
}

TSet<AInteractable*> AHaxxedCharacter::DetectAllInteractablesAroundPlayer()
{
	return DetectAllActorsOfType<AInteractable>(InteractableDetectionRange);
}

TSet<AHaxxedTargetPoint*> AHaxxedCharacter::DetectAllMoveToLocations()
{
	return DetectAllActorsOfType<AHaxxedTargetPoint>(TargetPointDetectionRange);
}

// Called to bind functionality to input
void AHaxxedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// PlayerInputComponent->BindAxis("Turn", this, &AHaxxedCharacter::CameraYawInput);
	// PlayerInputComponent->BindAxis("LookUp", this, &AHaxxedCharacter::CameraPitchInput);
}

float AHaxxedCharacter::WrapAngleTo180(float Angle)
{
	if (0 <= Angle && Angle <= 180)
	{
		return Angle;
	}
	else if (180 < Angle && Angle <= 360)
	{
		return (Angle - 360.f);
	}
	else
	{
		return Angle;
	}
}

bool AHaxxedCharacter::DeltaMoveToLocation(FVector Location, float DeltaTime)
{
	// Since there is no movement in Z direction
	Location.Z = GetActorLocation().Z;

	FVector Direction = Location - GetActorLocation();
	// Direction.Z = GetActorLocation().Z;
	if (Direction.Size() <= GetCharacterMovement()->MaxWalkSpeed * DeltaTime)
	{
		SetActorLocation(Location);
		return true;
	}
	else
	{
		AddMovementInput(Direction, 1.f);
		return false;
	}
}

bool AHaxxedCharacter::DeltaRotateToRotation(FRotator Rotation, float DeltaTime)
{
	FRotator ResultingRotation = bSpin360?
		UnsafeRInterpConstantTo(GetControlRotation(), TargetRotation, DeltaTime, 50.f) :
		FMath::RInterpConstantTo(GetControlRotation(), TargetRotation, DeltaTime, 100.f);

	Controller->SetControlRotation(ResultingRotation);
	
	return ResultingRotation.Equals(TargetRotation);
}

FRotator AHaxxedCharacter::UnsafeRInterpConstantTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed)
{
	
	// if DeltaTime is 0, do not perform any interpolation (Location was already calculated for that frame)
	if( DeltaTime == 0.f || Current == Target )
	{
		return Current;
	}

	// If no interp speed, jump to target value
	if( InterpSpeed <= 0.f )
	{
		return Target;
	}

	const float DeltaInterpSpeed = InterpSpeed * DeltaTime;
	
	const FRotator DeltaMove = (Target - Current);
	FRotator Result = Current;
	Result.Pitch += FMath::Clamp(DeltaMove.Pitch, -DeltaInterpSpeed, DeltaInterpSpeed);
	Result.Yaw += FMath::Clamp(DeltaMove.Yaw, -DeltaInterpSpeed, DeltaInterpSpeed);
	Result.Roll += FMath::Clamp(DeltaMove.Roll, -DeltaInterpSpeed, DeltaInterpSpeed);
	return Result;
}

bool AHaxxedCharacter::MoveCharacterToLocation(FVector Destination)
{
	// If Player is already moving somewhere else
	if (bMove)
	{
		return false;
	}
	// If Player is not moving
	else
	{
		TargetPointLocation = Destination;
		bMove = true;
		return true;
	}
}

void AHaxxedCharacter::FocusOnInteractable(AInteractable * Interactable)
{
	if (Interactable)
	{
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Interactable->GetActorLocation());
		bRotate = true;
	}
}

void AHaxxedCharacter::LookAtTarget(FVector Target)
{
	TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	bRotate = true;
}

void AHaxxedCharacter::SpinAround()
{
	TargetRotation = GetControlRotation();
	TargetRotation.Yaw += 360.f;
	bRotate = true;
	bSpin360 = true;
}


//void AHaxxedCharacter::CameraYawInput(float Value)
//{
//	if (Value != 0.f)
//	{
//		float PlayerYaw = GetActorRotation().Yaw;
//		float ControllerYaw = WrapAngleTo180(Controller->GetControlRotation().Yaw);
//		float DeltaYaw = FMath::FindDeltaAngleDegrees(PlayerYaw, ControllerYaw);
//
//
//		if ((Value > 0.f && DeltaYaw < 135.f) || (Value < 0.f && -135.f < DeltaYaw))
//		{
//			AddControllerYawInput(Value);
//		}
//	}
//}

//void AHaxxedCharacter::CameraPitchInput(float Value)
//{
//	if (Value != 0.f)
//	{
//		float PlayerPitch = GetActorRotation().Pitch;
//		float ControllerPitch = WrapAngleTo180(Controller->GetControlRotation().Pitch);
//		float DeltaPitch = FMath::FindDeltaAngleDegrees(PlayerPitch, ControllerPitch);
//
//		if ((Value < 0.f && DeltaPitch < 45.f) || (Value > 0.f && -45.f < DeltaPitch))
//		{
//			AddControllerPitchInput(Value);
//		}
//	}
//}

