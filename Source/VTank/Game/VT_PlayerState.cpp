// Fill out your copyright notice in the Description page of Project Settings.


#include "VT_PlayerState.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"
#include "VTPlayerController.h"


AVT_PlayerState::AVT_PlayerState()
{

}

void AVT_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVT_PlayerState, Username);
	DOREPLIFETIME(AVT_PlayerState, UserScore);
}


void AVT_PlayerState::OnRep_UserScore()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVTPlayerController::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AVTPlayerController* AVTPC = Cast< AVTPlayerController>(FoundActors[i]);
		if(AVTPC)
			AVTPC->CallUpdateUI();
	}
}

void AVT_PlayerState::AddScore()
{
	if (HasAuthority())
	{
		UserScore++;

		if(GetNetMode() != NM_DedicatedServer)
		OnRep_UserScore();
	}
}

void AVT_PlayerState::SetUsername(FString NewName)
{
	if (HasAuthority())
	{
		Username = *NewName;
	}
}

FString AVT_PlayerState::GetUsername() const
{
	return Username;
}

int AVT_PlayerState::GetUserScore() const
{
	return UserScore;
}
