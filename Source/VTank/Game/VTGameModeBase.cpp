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

void AVTGameModeBase::StopGame(FString WinnerName)
{
	//UGameplayStatics::SetGamePaused(GetWorld(), true);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVTPlayerController::StaticClass(), FoundActors);

	FString WinText = WinnerName + FString(" has reached ") + FString::FromInt(PointsToWin) + FString(" and won");

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AVTPlayerController* otherAVTPC = Cast< AVTPlayerController>(FoundActors[i]);
		if (otherAVTPC)
		{
			otherAVTPC->StopAllMovementFromThisPoint();
			otherAVTPC->UpdateUIWinText(WinText);
			otherAVTPC->OpenReadyScreen();
		}
	}
}

void AVTGameModeBase::RestartTheGame()
{
	FString LevelName = GetWorld()->GetMapName();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Loading: %s"), *LevelName));
	//GetWorld()->ServerTravel(LevelName + FString("?listen"), true);
	
	//UGameplayStatics::OpenLevel(GetWorld(), FName(LevelName));

	GetWorld()->ServerTravel(TEXT("/Game/Levels/GameWorld?listen"), true); 
}



void AVTGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}



