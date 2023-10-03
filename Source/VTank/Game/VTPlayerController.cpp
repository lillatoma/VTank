// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "VTTankCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "NavigationSystem.h"
#include "Net/UnrealNetwork.h"

#include "VTTankCharacter.h"
#include "VTGameModeBase.h"

#include "../UI/VT_UISpawner.h"
#include "Kismet/GameplayStatics.h"

#include "VT_PlayerState.h"


void AVTPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AVTPlayerController, CachedDestination)
}

AVTPlayerController::AVTPlayerController()
{
	bReplicates = true;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	
}

void AVTPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}


void AVTPlayerController::ServerUpdateCachedDestination_Implementation(FVector Dest)
{
	CachedDestination = Dest;
}


void AVTPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AVTPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AVTPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AVTPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AVTPlayerController::OnSetDestinationReleased);
		
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AVTPlayerController::OnLaunchCannonTriggered);
	}
}

void AVTPlayerController::OnInputStarted()
{
	if (!bPossibleToMove)
		return;
	StopMovement();
}

// Triggered every frame when the input is held down
void AVTPlayerController::OnSetDestinationTriggered()
{
	if (!bPossibleToMove)
		return;
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;

	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

	// If we hit a surface, cache the location   
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
		ServerUpdateCachedDestination(CachedDestination);
		
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
	}
}


void AVTPlayerController::OnSetDestinationReleased()
{
	if (!bPossibleToMove)
		return;

	CallSimpleMove();
}

void AVTPlayerController::OnLaunchCannonTriggered()
{
	if (!bPossibleToMove)
		return;

	APawn* ControlledPawn = GetPawn();

	AVTTankCharacter* Tank = Cast<AVTTankCharacter>(ControlledPawn);

	if (Tank)
		Tank->TryShootCannon();
}



void AVTPlayerController::GenerateRandomUsername()
{
	if (HasAuthority())
	{

		char* First[] = { "Elegant","Clever","Lazy","Awesome","Adorable","Basic","Better","Cute","Cautious","Calm","Desirable","Eternal","Fuzzy","Great" };
		char* Second[] = { "Mushroom", "Dolphin", "Engine", "Pizza", "Kitten", "Actor", "Bee", "Cicada", "Tornado", "Lasso" };

		int FirstNameMaxLength = 14;
		int SecondNameMaxLength = 10;

		AVT_PlayerState* State = GetPlayerState<AVT_PlayerState>();


		FString Username = FString(First[FMath::RandRange(0, FirstNameMaxLength - 1)])
			+ FString("_") + FString(Second[FMath::RandRange(0, SecondNameMaxLength - 1)]) + FString("_") + FString::FromInt(FMath::RandRange(0, 99));
		if (State)
			State->SetUsername(Username);
		else UE_LOG(LogTemp, Warning, TEXT("Server: GenerateRandomUsername: State invalid"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Client called GenerateRandomUsername"));
}

void AVTPlayerController::CallUpdateUI_Implementation()
{
	if (Spawner)
		Spawner->CallUpdateScoreboard();
}

void AVTPlayerController::OpenReadyScreen_Implementation()
{
	if (Spawner)
		Spawner->CallOpenReadyScreen();
}

void AVTPlayerController::StopAllMovementFromThisPoint_Implementation()
{
	bPossibleToMove = false;
}

void AVTPlayerController::UpdateUIWinText_Implementation(const FString& WinText)
{
	if (Spawner)
		Spawner->CallSetWinText(WinText);
}

void AVTPlayerController::UpdateReadiedCount_Implementation(int Count, int TotalCount)
{
	if (Spawner)
		Spawner->CallUpdateReadiedCount(Count, TotalCount);
}

void AVTPlayerController::DelayedCallUpdateUI_Implementation()
{
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AVTPlayerController::CallUpdateUI, 1.f, false);
}


void AVTPlayerController::CallSimpleMove_Implementation()
{
	if(bPossibleToMove)
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
}


void AVTPlayerController::SetReadiedForNextGame_Implementation()
{
	AVT_PlayerState* State = GetPlayerState<AVT_PlayerState>();
	if (State)
		State->SetReadyForNextGame();
}

void AVTPlayerController::SpawnUISpawner_Implementation()
{
	FTransform SpawnTransform(FRotator(), FVector(0,0,0), FVector(1, 1, 1));

	Spawner = Cast<AVT_UISpawner>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, UISpawnerActor, SpawnTransform));
	//Spawning the cannonball with the target passed
	if (Spawner)
	{

		UGameplayStatics::FinishSpawningActor(Spawner, SpawnTransform);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Spawner spawned")));
	}
}

void AVTPlayerController::FindUISpawner_Implementation()
{
	

	FTransform SpawnTransform(FRotator(), FVector(0, 0, 0), FVector(1, 1, 1));

	TArray<AActor*> PiecesArray;
	UGameplayStatics::GetAllActorsOfClass(this, AVT_UISpawner::StaticClass(), PiecesArray);

	if (PiecesArray.Num() == 0)
		return;

	Spawner = Cast<AVT_UISpawner>(PiecesArray[0]);

}