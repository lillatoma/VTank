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

void AVTPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	//Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AVTPlayerController, CachedDestination)
}

AVTPlayerController::AVTPlayerController()
{
	bReplicates = true;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector + FVector(100,0,0);
	
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

void AVTPlayerController::CallSimpleMove_Implementation()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
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
	StopMovement();
}

// Triggered every frame when the input is held down
void AVTPlayerController::OnSetDestinationTriggered()
{
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
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation());// .GetSafeNormal();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.2f|%.2f|%.2f"), CachedDestination.X, CachedDestination.Y, CachedDestination.Z));
	}
}


void AVTPlayerController::OnSetDestinationReleased()
{
	CallSimpleMove();
}

void AVTPlayerController::OnLaunchCannonTriggered()
{
	APawn* ControlledPawn = GetPawn();

	AVTTankCharacter* Tank = Cast<AVTTankCharacter>(ControlledPawn);

	if (Tank)
		Tank->TryShootCannon();
}
