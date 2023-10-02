// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Game.h"
#include "../Game/VTGameModeBase.h"
#include "../Game/VTGameStateBase.h"

FString UUW_Game::GetScoreboardText()
{
	if (FindGameState())
	{
		FString String;

		TArray<FPlayerScoreboardInfo> Info = StoredGameState->GetScoreboardInfo();

		for (int i = 0; i < Info.Num(); i++)
		{
			String = String + FString(Info[i].Name) + FString(" | ") + FString::FromInt(Info[i].Score) + FString("\n");
		}
		return String;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Text: %s"), String));
	return FString("Can't find, sorry");
}

bool UUW_Game::FindGameState()
{
	if (StoredGameState)
		return true;

	AGameStateBase* GameState = (GetWorld()->GetGameState());

	AVTGameStateBase* AVTState = Cast<AVTGameStateBase>(GameState);

	if (AVTState)
	{
		StoredGameState = AVTState;
		return true;
	}
	return false;
}
