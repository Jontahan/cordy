// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionLocation.h"
#include "GameFramework/PlayerController.h"
#include "CordyController.generated.h"

class ABaseCreature;

/**
 * 
 */
UCLASS()
class CORDY_API ACordyController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "Cordy")
	float CameraSwitchTime{1};
	
protected:
	void TrySwitch();
	void TryInteract();
	void SwitchTo(ABaseCreature* Creature);
	void DoWork(AActionLocation* Location);
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	bool bIsDoingAction{false};
};
