// Fill out your copyright notice in the Description page of Project Settings.


#include "VTCannonBall.h"
#include "VTTankCharacter.h"
#include "VTGameModeBase.h"
#include "VTPlayerController.h"

#include "VisualLogger/VisualLogger.h"

#include "VT_PlayerState.h"

// Sets default values
AVTCannonBall::AVTCannonBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AVTCannonBall::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AVTCannonBall::OverlapBegin);

	Mesh->SetPhysicsLinearVelocity(ForcedVelocity);
	
}

// Called every frame
void AVTCannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Forcing velocity, because there was some weird glitch where the cannonball started with 0 velocity
	//and it was set to only overlap everything, but somehow reacted to the original tank moving
	Mesh->SetPhysicsLinearVelocity(ForcedVelocity);
}

void AVTCannonBall::CallAddScore()
{
	AVTTankCharacter* IgnoreTank = Cast<AVTTankCharacter>(GetOwner());

	if (!IgnoreTank)
		return;



	
	AVT_PlayerState* State = IgnoreTank->GetPlayerState<AVT_PlayerState>();
	if (State && CanScore)
	{
		State->AddScore();
		CanScore = false;

		////Calling Update on Spawner:
		//AController* Controller = IgnoreTank->GetController();
		//if (Controller)
		//{
		//	AVTPlayerController* AVTPC = Cast<AVTPlayerController>(Controller);
		//	if (AVTPC)
		//		AVTPC->CallUpdateUI();

		//}

	}
}


void AVTCannonBall::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVTTankCharacter* IgnoreTank = Cast<AVTTankCharacter>(GetOwner());
	AVTTankCharacter* Tank = Cast<AVTTankCharacter>(OtherActor);
	
	if (!IgnoreTank)
		return;

	if (Tank && IgnoreTank && Tank != IgnoreTank)
	{
		//Kill Tank!
		AGameModeBase* GameMode = (GetWorld()->GetAuthGameMode());
		AController* Controller = IgnoreTank->GetController();

		if (Controller && GameMode)
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Respawning")));
			AActor* BestSpot = GameMode->ChoosePlayerStart(Controller);
			GameMode->RestartPlayerAtPlayerStart(Controller, BestSpot);
			Tank->SetActorLocation(BestSpot->GetActorLocation());
			Tank->SetActorRotation(FRotator(0,0,0));


		}

		
		CallAddScore();
		Destroy();
	}
	else if (!Tank)
		Destroy();

}