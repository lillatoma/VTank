// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VTCannonBall.generated.h"

UCLASS()
class VTANK_API AVTCannonBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVTCannonBall();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION(Server, Reliable)
	void CallAddScore();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void SetForcedVelocity(FVector NewForcedVelocity) { ForcedVelocity = NewForcedVelocity; }

private:


	UPROPERTY()
		FVector ForcedVelocity = FVector(100.f,0.f,0.f);

	bool CanScore = true;

};
