// Fill out your copyright notice in the Description page of Project Settings.


#include "VTGameStateBase.h"
#include "VT_PlayerState.h"

TArray<FPlayerScoreboardInfo> AVTGameStateBase::GetScoreboardInfo()
{
	TArray<FPlayerScoreboardInfo> Info;

	for (int i = 0; i < PlayerArray.Num(); i++)
	{
		APlayerState* PS = PlayerArray[i].Get();
		if (PS)
		{
			AVT_PlayerState* State = Cast<AVT_PlayerState>(PS);

			if (State)
			{
				FPlayerScoreboardInfo Bit;
				Bit.Name = State->GetUsername();
				Bit.Score = State->GetUserScore();

				Info.Add(Bit);
			}
		}

	}

	return Info;
}
