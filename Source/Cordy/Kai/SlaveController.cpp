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
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TaskTimer);
	StopMovement();
	
	Super::OnUnPossess();
}

void ASlaveController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	ABaseCreature* Creature = Cast<ABaseCreature>(GetPawn());
	if(!Creature || !Creature->IsCaptured())
	{
		return;
	}
	
	if(CurrentLocation)
	{
		DoWork(CurrentLocation);
	}
}

void ASlaveController::StartTasks()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(TaskTimer))
	{
		return;
	}
	
	ABaseCreature* Creature = Cast<ABaseCreature>(GetPawn());
	if(!Creature)
	{
		return;
	}

	if(Creature->Actions.Num() == 0)
	{
		return;
	}
	
	if(Creature->CurrentActionIndex >= Creature->Actions.Num())
	{
		Creature->CurrentActionIndex = 0;
	}
	const FActionReplay& Replay = Creature->Actions[Creature->CurrentActionIndex];
	++Creature->CurrentActionIndex;

	AActionLocation* Loc = Replay.Location;
	CurrentLocation = Loc;
	if(!IsValid(Loc))
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			StartTasks();
		});
		return;
	}

	EPathFollowingRequestResult::Type Result;

	switch(Replay.Action)
	{
	case EActionType::Work:
		if(!Loc->CanDoWork())
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				StartTasks();
			});
			break;
		}
		Result = MoveTo(Loc->GetActorLocation(), 500.0f);
		if(Result != EPathFollowingRequestResult::RequestSuccessful && Result != EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				StartTasks();
			});
		}
		
		break;
		
	case EActionType::Pickup:
		Creature->Pickup(Loc);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			StartTasks();
		});
		break;
		
	case EActionType::Drop:
		Creature->Drop(Loc);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			StartTasks();
		});
		break;
		
	}
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

	EPathFollowingRequestResult::Type Result = MoveTo(Target, -1.0f);
	
	float Time = 0.0f;
	if((int)Result != (int)EPathFollowingResult::Success)
	{
		Time = FMath::RandRange(0.1f, 2.0f);
	}
	else
	{
		Time = FMath::RandRange(Creature->RandomMoveInterval.X, Creature->RandomMoveInterval.Y);
	}

	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &ASlaveController::StartRandomMovement, Time, false);
}

float ASlaveController::DoWork(AActionLocation* Location)
{
	ABaseCreature* Creature = Cast<ABaseCreature>(GetPawn());
	float WorkDuration = Creature->DoWork(Location);
	GetWorld()->GetTimerManager().SetTimer(TaskTimer, [this, Location, Creature]()
	{
		Location->DidWork();
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			StartTasks();
		});
	}, WorkDuration, false);

	return WorkDuration;
}

EPathFollowingRequestResult::Type ASlaveController::MoveTo(FVector Location, float Radius)
{
	ABaseCreature* Creature = Cast<ABaseCreature>(GetPawn());
	
	//Try to find ground
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Creature);
	if(GetWorld()->LineTraceSingleByProfile(Hit, {Location.X, Location.Y, Location.Z + 1000.0f},
		{Location.X, Location.Y, Location.Z - 500.0f}, Creature->GetCapsuleComponent()->GetCollisionProfileName(), Params))
	{
		Location = Hit.Location;
	}
	
	return MoveToLocation(Location, Radius, true, true, true);
}

void ASlaveController::HandleCaptured(ABaseCreature* Creature)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(MovementTimer);
	StopMovement();

	StartTasks();
}
