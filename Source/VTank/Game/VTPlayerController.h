// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "VTPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class VTANK_API AVTPlayerController : public APlayerController
{
	GENERATED_BODY()



public:
	AVTPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const override;



	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ShootAction;

	UFUNCTION(Server, Reliable)
		void CallSimpleMove();

	UFUNCTION(Server, Reliable)
		void ServerUpdateCachedDestination(FVector Dest);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		FVector CachedDestination;

	void GenerateRandomUsername();
public:
	UFUNCTION(Client, Reliable)
		virtual void SpawnUISpawner();

	UFUNCTION(Client, Reliable)
		virtual void FindUISpawner();
	UFUNCTION(Client, Reliable)
		virtual void CallUpdateUI();

	UFUNCTION(Client, Reliable)
			virtual void DelayedCallUpdateUI();

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		TSubclassOf<AActor> UISpawnerActor;

	UPROPERTY()
		class AVT_UISpawner* Spawner;

protected:
	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();

	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	void OnLaunchCannonTriggered();

	
};


