// Fill out your copyright notice in the Description page of Project Settings.


#include "VTCannonBall.h"
#include "VTTankCharacter.h"

#include "VisualLogger/VisualLogger.h"

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
	
}

// Called every frame
void AVTCannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVTCannonBall::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Collision?")));
	

	AVTTankCharacter* Tank = Cast<AVTTankCharacter>(OtherActor);

	if (Tank && Tank != IgnoreTank)
	{
		// TODO
		//Kill Tank!
		//Add Score
		Destroy();
	}
	else if (!Tank)
		Destroy();

}