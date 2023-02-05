// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/BaseCreature.h"

#include "SAdvancedRotationInputBox.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseCreature::ABaseCreature()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCreature::BeginPlay()
{
	Super::BeginPlay();

	SpawnLocation = GetActorLocation();
}

// Called every frame
void ABaseCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCreature::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCreature::Turn);
}

void ABaseCreature::SetCaptured()
{
	bIsCaptured = true;
	WasCaptured.Broadcast(this);
}

float ABaseCreature::DoWork(AActionLocation* Location)
{
	return Location->DoWork();
}

bool ABaseCreature::Pickup(AActionLocation* Location)
{
	if(!Location->Pickup())
	{
		return false;
	}
	ResourcesCarried.Add(Location->GetResourceType());
	
	return true;
}

bool ABaseCreature::Drop(AActionLocation* Location)
{
	if(!ResourcesCarried.Contains(Location->GetResourceType()))
	{
		return false;
	}

	if(!Location->Drop())
	{
		return false;
	}

	ResourcesCarried.Remove(Location->GetResourceType());
	return true;
}

void ABaseCreature::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ABaseCreature::Turn(float Value)
{
	if(Value != 0.0f)
	{
		AController* CurrentController = GetController();
		if(!ensure(CurrentController))
		{
			return;
		}

		UCharacterMovementComponent* Movement = GetCharacterMovement();
		if(!ensure(Movement))
		{
			return;
		}
		
		const FRotator currentRotation = CurrentController->GetControlRotation();
		const float newYaw = currentRotation.Yaw + Value * Movement->RotationRate.Yaw * GetWorld()->DeltaTimeSeconds;
		CurrentController->SetControlRotation({currentRotation.Pitch, newYaw, currentRotation.Roll});
	}
}

