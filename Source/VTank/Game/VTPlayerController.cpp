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
	DOREPLIFETIME(AVTPlayerController, Score)
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

void AVTPlayerController::GenerateRandomUsername()
{
	if (HasAuthority())
	{

		char* First[] = { "Elegant","Clever","Lazy","Awesome","Adorable","Basic","Better","Cute","Cautious","Calm","Desirable","Eternal","Fuzzy","Great" };
		char* Second[] = { "Mushroom", "Dolphin", "Engine", "Pizza", "Kitten", "Actor", "Bee", "Cicada", "Tornado", "Lasso" };

		int FirstNameMaxLength = 14;
		int SecondNameMaxLength = 10;

		AVT_PlayerState* State = GetPlayerState<AVT_PlayerState>();


		Username = FString(First[FMath::RandRange(0, FirstNameMaxLength - 1)])
			+ FString("_") + FString(Second[FMath::RandRange(0, SecondNameMaxLength - 1)]) + FString("_") + FString::FromInt(FMath::RandRange(0, 99));
		if (State)
			State->SetUsername(Username);
		else UE_LOG(LogTemp, Warning, TEXT("Server: GenerateRandomUsername: State invalid"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Client called GenerateRandomUsername"));
}

void AVTPlayerController::CallUpdateUI()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Authorized CallUpdateUI on %s"), *Username);
		if (Spawner)
			Spawner->CallUpdateScoreboard();
	}
	else CallUpdateUIClient();
}

void AVTPlayerController::CallUpdateUIClient_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("CallUpdateUI on %s"), *Username);
	if (Spawner)
		Spawner->CallUpdateScoreboard();
}

void AVTPlayerController::DelayedCallUpdateUI_Implementation()
{
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AVTPlayerController::CallUpdateUI, 1.f, false);
}


void AVTPlayerController::CallSimpleMove_Implementation()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
}

void AVTPlayerController::AddScore_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s scored"), *Username));
	Score++;

	if (Spawner)
		Spawner->CallUpdateScoreboard();
}


void AVTPlayerController::SpawnUISpawner_Implementation()
{
	FTransform SpawnTransform(FRotator(), FVector(0,0,0), FVector(1, 1, 1));

	Spawner = Cast<AVT_UISpawner>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, UISpawnerActor, SpawnTransform));
	//Spawning the cannonball with the target passed
	if (Spawner)
	{

		UGameplayStatics::FinishSpawningActor(Spawner, SpawnTransform);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Spawner spawned")));
	}
}

void AVTPlayerController::FindUISpawner_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Finding ui")));

	FTransform SpawnTransform(FRotator(), FVector(0, 0, 0), FVector(1, 1, 1));

	TArray<AActor*> PiecesArray;
	UGameplayStatics::GetAllActorsOfClass(this, AVT_UISpawner::StaticClass(), PiecesArray);

	if (PiecesArray.Num() == 0)
		return;

	Spawner = Cast<AVT_UISpawner>(PiecesArray[0]);

	if (Spawner)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Spawner found")));
	}
}