// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/SlaveController.h"

#include "BaseCreature.h"
#include "Components/CapsuleComponent.h"

void ASlaveController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABaseCreature* Creature = Cast<ABaseCreature>(InPawn);
	if(!Creature)
	{
		return;
	}

	Creature->WasCaptured.AddDynamic(this, &ASlaveController::HandleCaptured);

	if(Creature->IsCaptured())
	{
		StartTasks();
		return;
	}

	if(Creature->bMoveRandomly)
	{
		StartRandomMovement();
		return;
	}
}

void ASlaveController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ASlaveController::StartTasks()
{
	//TODO:
	//Chose the "Next" Task
	//Go to the task location
	//"Do" the task(Chop wood, Repair bridge, pickup item, drop item, etc.)
	//Start timer for Task duration, with modifiers for species?
	//After duration: Tell the Action we did it, allowing it to do whatever the action does(Spawn wood, repair bridge, increment/decrement resources, et.c)
	//Run "StartTask" again for the next Task in the list.
	//Loop.
}

void ASlaveController::StartRandomMovement()
{
	ABaseCreature* Creature = Cast<ABaseCreature>(GetPawn());
	check(Creature);

	//Find a random location in a radius around the spawn location
	const float r = Creature->RandomMoveDistance * sqrt(FMath::FRand());
	const float theta = FMath::FRand() * 2.0f * PI;
	const float x = r * cos(theta);
	const float y = r * sin(theta);
	FVector Target = Creature->GetSpawnLocation() + FVector(x, y, 0.0f);

	//Try to find ground
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Creature);
	if(GetWorld()->LineTraceSingleByProfile(Hit, {Target.X, Target.Y, Target.Z + 1000.0f},
		Target, Creature->GetCapsuleComponent()->GetCollisionProfileName(), Params))
	{
		Target = Hit.Location;
	}
	
	const EPathFollowingRequestResult::Type Result = MoveToLocation(Target, -1, true, true, true);
	
	float Time = 0.0f;
	if(Result != EPathFollowingResult::Success)
	{
		Time = FMath::RandRange(0.1f, 2.0f);
	}
	else
	{
		Time = FMath::RandRange(Creature->RandomMoveInterval.X, Creature->RandomMoveInterval.Y);
	}

	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &ASlaveController::StartRandomMovement, Time, false);
}

void ASlaveController::HandleCaptured(ABaseCreature* Creature)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(MovementTimer);
	StopMovement();
}
