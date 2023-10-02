// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UW_Game.h"

#include "VT_UISpawner.generated.h"

UCLASS()
class VTANK_API AVT_UISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVT_UISpawner();

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<class UUserWidget> ScoreboardWidgetClass;

	UPROPERTY(EditAnywhere)
		class UUW_Game* ScoreboardWidget;

	//UFUNCTION(Client, Reliable)
		virtual void CallUpdateScoreboard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UFUNCTION(Client, Reliable)
	virtual void SpawnScoreboardWidget();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
