// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/SlaveController.h"

#include "BaseCreature.h"

void ASlaveController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABaseCreature* Creature = Cast<ABaseCreature>(InPawn);
	if(!Creature)
	{
		return;
	}

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
}

void ASlaveController::StartRandomMovement()
{
	ABaseCreature* Creature = Cast<ABaseCreature>(GetPawn());
	check(Creature);

	const float r = Creature->RandomMoveDistance * sqrt(FMath::FRand());
	const float theta = FMath::FRand() * 2.0f * PI;
	const float x = 0 + r * cos(theta);
	const float y = 0 + r * sin(theta);
	const FVector Target = Creature->GetSpawnLocation() + FVector(x, y, 0.0f);
	
	const EPathFollowingRequestResult::Type Result = MoveToLocation(Target, -1, true, true, true);
	
	float Time = 0.0f;
	if(Result != EPathFollowingResult::Success)
	{
		Time = FMath::RandRange(0, 2);
	}
	else
	{
		Time = FMath::RandRange(Creature->RandomMoveInterval.X, Creature->RandomMoveInterval.Y);
	}

	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &ASlaveController::StartRandomMovement, Time, false);
}
