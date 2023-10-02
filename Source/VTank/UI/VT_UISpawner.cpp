// Fill out your copyright notice in the Description page of Project Settings.


#include "VT_UISpawner.h"

// Sets default values
AVT_UISpawner::AVT_UISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

}

// Called when the game starts or when spawned
void AVT_UISpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ScoreboardWidget)
		SpawnScoreboardWidget();

	CallUpdateScoreboard();
}

// Called every frame
void AVT_UISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}



void AVT_UISpawner::SpawnScoreboardWidget()
{
	if (ScoreboardWidgetClass)
	{
		ScoreboardWidget = Cast<UUW_Game>(CreateWidget(GetWorld(), ScoreboardWidgetClass));

		if (ScoreboardWidget)
		{
			ScoreboardWidget->AddToViewport();
			ScoreboardWidget->UpdateScoreboard();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("scoreboard spawned")));
		}
	}
}

void AVT_UISpawner::CallUpdateScoreboard()
{
	if (ScoreboardWidget)
		ScoreboardWidget->UpdateScoreboard();
}

void AVT_UISpawner::CallOpenReadyScreen()
{
	if (ScoreboardWidget)
		ScoreboardWidget->OpenReadyScreen();
}

void AVT_UISpawner::CallSetWinText(const FString& WinText)
{
	if (ScoreboardWidget)
		ScoreboardWidget->SetWinText(WinText);
}

void AVT_UISpawner::CallUpdateReadiedCount(int Count, int TotalCount)
{
	if (ScoreboardWidget)
		ScoreboardWidget->UpdateReadiedCount(Count, TotalCount);
}