// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VTTankCharacter.generated.h"

UCLASS()
class VTANK_API AVTTankCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Components
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCameraComponent* Camera;



#pragma endregion

#pragma region Overrides


	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

#pragma endregion


public:
	// Sets default values for this character's properties
	AVTTankCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region RotationCorrection


	FRotator CachedRotation;

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_Rotation(FRotator Rotation);

	UFUNCTION(Server, Reliable)
		void ShareRotationToServer(FRotator Rotation);

#pragma endregion
};
