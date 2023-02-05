// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionLocation.h"
#include "AIController.h"
#include "SlaveController.generated.h"

/**
 * 
 */
UCLASS()
class CORDY_API ASlaveController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;

protected:
	void StartTasks();
	void StartRandomMovement();
	float DoWork(AActionLocation* Location);

	EPathFollowingRequestResult::Type MoveTo(FVector Location, float Radius);

	UFUNCTION()
	void HandleCaptured(ABaseCreature* Creature);

	UPROPERTY()
	AActionLocation* CurrentLocation;
	
	FTimerHandle MovementTimer;
	FTimerHandle TaskTimer;
	
};
