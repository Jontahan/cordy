// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

protected:
	void StartTasks();
	void StartRandomMovement();
	
	FTimerHandle MovementTimer;
};
