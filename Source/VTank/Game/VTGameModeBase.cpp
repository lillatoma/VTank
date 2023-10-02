// Fill out your copyright notice in the Description page of Project Settings.


#include "VTGameModeBase.h"

#include "VTPlayerController.h"

#include "Kismet/GameplayStatics.h"

void AVTGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);



	AVTPlayerController* AVTPC = Cast<AVTPlayerController>(NewPlayer);

	if (AVTPC)
	{
		AVTPC->GenerateRandomUsername();
		AVTPC->FindUISpawner(); 
	
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVTPlayerController::StaticClass(), FoundActors);

		for (int i = 0; i < FoundActors.Num(); i++)
		{
			AVTPlayerController* otherAVTPC = Cast< AVTPlayerController>(FoundActors[i]);
			if(otherAVTPC)
				otherAVTPC->DelayedCallUpdateUI();
		}

	}

}



void AVTGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}



