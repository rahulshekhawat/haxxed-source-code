// Fill out your copyright notice in the Description page of Project Settings.

#include "HaxxedGameInstance.h"
#include "Characters/HaxxedCharacter.h"
#include "Interactables/Interactable.h"
#include "Interactables/HaxxedTargetPoint.h"
#include "UI/TerminalWidget.h"

#include "Engine/World.h"
#include "MoviePlayer.h"


const FString UHaxxedGameInstance::SuPassword{ TEXT("admin") };

void UHaxxedGameInstance::Su(const FString& Password)
{
	if (Terminal && !IsMainMenuLevel())
	{
		if (Password.IsEmpty())
		{
			Terminal->Println(TEXT("su: needs a password."));
		}
		else if (Password.Equals(SuPassword))
		{
			Terminal->SetSuperUser(true);
			Terminal->Println(TEXT("Welcome, CoolHaxxor. Login time: now."));
		}
		else
		{
			Terminal->Println(TEXT("su: incorrect password."));
		}
	}
	else
	{
		Terminal->Println(TEXT("su: You're not in yet!"));
	}
}

void UHaxxedGameInstance::Cd(const FString& Name)
{
	if (Terminal && !IsMainMenuLevel())
	{
		if (Name.IsEmpty())
		{
			Terminal->Println(TEXT("cd: enter a directory name"));
			return;
		}

		if (Character)
		{
			TSet<AHaxxedTargetPoint*> AllTargetPoints{ Character->DetectAllMoveToLocations() };
			
			/*AHaxxedTargetPoint** TargetPoint{ AllTargetPoints.FindByPredicate(
				[&Name](const AHaxxedTargetPoint* Point)
				{
					return Point->GetInGameName() == Name;
				}
			) };*/
			AHaxxedTargetPoint* TargetPoint{ nullptr };
			for (auto* Point : AllTargetPoints)
			{
				if (Point->GetInGameName() == Name)
				{
					TargetPoint = Point;
					break;
				}
			}
			
			if (TargetPoint)
			{
				AHaxxedTargetPoint* Point{ TargetPoint };
				FVector Goal{ Point->GetActorLocation().X, Point->GetActorLocation().Y, 100 };
				Character->LookAtTarget(Point->GetActorLocation());
				Character->MoveCharToLocation(Goal);
			}
			else
			{
				Terminal->Println(FString::Printf(TEXT("cd: %s: No such directory."), *Name));
			}
		}
	}
	else
	{
		Terminal->Println(TEXT("cd: You're not in yet!"));
	}
}

void UHaxxedGameInstance::Less(const FString& Name)
{
	if (Terminal && !IsMainMenuLevel())
	{
		if (Name.IsEmpty())
		{
			Terminal->Println(TEXT("less: provide a file name"));
			return;
		}

		TSet<AInteractable*> Interactables{ Character->DetectAllInteractablesAroundPlayer() };

		/*AInteractable** Object{ Interactables.FindByPredicate(
			[&Name](AInteractable* Interactable)
			{
				return Interactable->GetInGameName() == Name;
			}
		) };*/
		AInteractable* Object{ nullptr };
		for (auto* Point : Interactables)
		{
			if (Point->GetInGameName() == Name)
			{
				Object = Point;
				break;
			}
		}

		if (Object)
		{
			AInteractable* Interactable{ Object };
			Character->FocusOnInteractable(Interactable);
			FString Message{ Interactable->GetDescription() };
			
			if (Interactable->RequiresSu())
			{
				if (Terminal->IsSuperUser())
				{
					Interactable->OnInteract();
				}
				else
				{
					Message += TEXT("\nIt's locked.");
				}
			}

			Terminal->Println(Message);
		}
		else
		{
			Terminal->Println(FString::Printf(TEXT("less: %s doesn't exist."), *Name));
		}
	}
	else
	{
		Terminal->Println(TEXT("Run: You're not in yet!"));
	}
}

void UHaxxedGameInstance::Ls(const FString& ScanType)
{
	if (Terminal && !IsMainMenuLevel() && Character)
	{
		// for TargetPoints
		if (ScanType == TEXT("-d"))
		{
			TSet<AHaxxedTargetPoint*> TargetPoints{ Character->DetectAllMoveToLocations() };
			FString Names;

			for (AHaxxedTargetPoint* Point : TargetPoints)
			{
				Names += TEXT("\n") + Point->GetInGameName();
			}

			if (Names.IsEmpty())
			{
				Terminal->Println(TEXT("ls: found nothing"));
			}
			else
			{
				Terminal->Println(TEXT("Directories found:") + Names);
			}
		}
		// for Interactables
		else if (ScanType == TEXT("-f"))
		{
			TSet<AInteractable*> Interactables{ Character->DetectAllInteractablesAroundPlayer() };
			FString Header{ TEXT("[Name, RestrictedAccess]") };
			FString Details;

			for (AInteractable* Object : Interactables)
			{
				Details += FString::Printf(
					TEXT("\n%s, %s"),
					*Object->GetInGameName(),
					Object->RequiresSu()? TEXT("true") : TEXT("false")
				);
			}

			if (Details.IsEmpty())
			{
				Terminal->Println(TEXT("ls: found nothing"));
			}
			else
			{
				Character->SpinAround();
				Terminal->Println(
					TEXT("Files found:\n") +
					Header +
					Details
				);
			}
		}
		// For everything else
		else
		{
			Terminal->Println(
				TEXT("ls <view-option>:\n")
				TEXT("-d: Access directories\n")
				TEXT("-f: Access files")
			);
		}
	}
	// Inside the Main Menu
	else
	{
		Terminal->Println(TEXT("ls: You're not in yet!"));
	}
}

void UHaxxedGameInstance::Clear()
{
	Terminal->Println();
}

void UHaxxedGameInstance::Start()
{
	if (IsMainMenuLevel())
	{
		UGameplayStatics::OpenLevel(GetWorld(), PlayableLevel);
	}
	else
	{
		Terminal->Println(TEXT("start: You're already inside!"));
	}
}

void UHaxxedGameInstance::Credits()
{
	if (Terminal)
	{
		Terminal->SwitchToCredits();
	}
}

void UHaxxedGameInstance::NoobFriendlyHelp()
{
	if (!Terminal)
	{
		return;
	}

	if (IsMainMenuLevel())
	{
		Terminal->Println(
			TEXT("noobfriendlyhelp: real haxxors hammer menu options in the keyboard.\n")
			TEXT("Was that hard?")
		);
	}
	else
	{
		Terminal->Println(
			TEXT("noobfriendlyhelp: this is a dumbed down Linux terminal.\n")
			TEXT("Wish this had a game journalist difficulty level?\n")
			TEXT("Too bad. You're playing just that.")
		);
	}
}

void UHaxxedGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	if (!IsRunningDedicatedServer())
	{
 		FLoadingScreenAttributes LoadingScreen;
	 	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
 		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
 
	 	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
    }
}
 
void UHaxxedGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	// can be overriden to have custom effects
}
