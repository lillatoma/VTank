// Fill out your copyright notice in the Description page of Project Settings.


#include "VTTankCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void AVTTankCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{

	FRotator Rotation = GetActorRotation();
	FVector Unit = Rotation.Vector();

	if(FVector::DotProduct(Unit,WorldDirection) / (Unit.Length()*WorldDirection.Length()) < 0.9)
		ScaleValue *= 0.025f;

	UPawnMovementComponent* MovementComponent = GetMovementComponent();
	if (MovementComponent)
	{
		MovementComponent->AddInputVector(WorldDirection * ScaleValue, bForce);
	}
	else
	{
		Internal_AddMovementInput(WorldDirection * ScaleValue, bForce);
	}
}

// Sets default values
AVTTankCharacter::AVTTankCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	//Default spawning components
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	BaseMesh->SetupAttachment(RootComponent);
	//RootComponent = BaseMesh;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	//Default setting up parameters for components
	SpringArm->TargetArmLength = 1500.0f;
	SpringArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	FRotator DefaultRotation = FRotator(-75, 0, 0);
	SpringArm->SetRelativeRotation(DefaultRotation);


	//Default setting character
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 0.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxAcceleration = 1000; //max accel 1000
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;//brakingfriction factor 1.0
	GetCharacterMovement()->GravityScale = 1.5f;//gravity scale 1.5
	GetCharacterMovement()->BrakingDecelerationWalking = 1000; //braking deceleration 1000
	//GetCharacterMovement()-> //no crouch, no jump, no swim, yes walk, no fly
	//GetCharacterMovement()-> //use accel for paths
	//GetCharacterMovement()-> //use fixed braking distance
	//GetCharacterMovement()-> //fixed path braking distance 200




}

// Called when the game starts or when spawned
void AVTTankCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVTTankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rotation = GetActorRotation();
	if (Rotation != CachedRotation)
	{
		CachedRotation = Rotation;
		Multicast_Rotation(Rotation);
	}
}

// Called to bind functionality to input
void AVTTankCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVTTankCharacter::Multicast_Rotation_Implementation(FRotator Rotation)
{
	SetActorRotation(Rotation, ETeleportType::None);
}

void AVTTankCharacter::ShareRotationToServer_Implementation(FRotator Rotation)
{

}

