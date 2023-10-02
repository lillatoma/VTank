// Fill out your copyright notice in the Description page of Project Settings.


#include "VTGameGameMode.h"

#include "VTPlayerController.h"

#include "Kismet/GameplayStatics.h"


void AVTGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	//if (!ScoreboardWidget)
	//	SpawnScoreboardWidget();

	Super::PostLogin(NewPlayer);
	AVTPlayerController* AVTPC = Cast<AVTPlayerController>(NewPlayer);

	if (AVTPC)
	{
		AVTPC->GenerateRandomUsername();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s connected"), *AVTPC->Username));
		
		//CallUpdateScoreboard();
	}

}