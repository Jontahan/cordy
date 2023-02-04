// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/CordyController.h"

#include "BaseCreature.h"
#include "TargetSelectionComponent.h"

void ACordyController::BeginPlay()
{
	Super::BeginPlay();
}

void ACordyController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Switch", IE_Pressed, this, &ACordyController::TrySwitch);
}

void ACordyController::TrySwitch()
{
	APawn* CurrentPawn = GetPawn();
	if(!CurrentPawn)
	{
		return;
	}
	
	UTargetSelectionComponent* Targeting = CurrentPawn->FindComponentByClass<UTargetSelectionComponent>();
	if(!Targeting)
	{
		return;
	}

	ABaseCreature* CurrentTarget = Cast<ABaseCreature>(Targeting->GetCurrentTarget());
	if(!CurrentTarget)
	{
		return;
	}

	SwitchTo(CurrentTarget);
}

void ACordyController::SwitchTo(ABaseCreature* Creature)
{
	if(!Creature || bIsSwitching || GetPawn() == Creature)
	{
		return;
	}

	if(!Creature->IsCaptured())
	{
		return;
	}

	bIsSwitching = true;
	SetViewTargetWithBlend(Creature, CameraSwitchTime);
	GetPawn()->DisableInput(this);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerHandle handle;
	TimerManager.SetTimer(handle, [this, Creature]()
	{
		bIsSwitching = false;
		GetPawn()->EnableInput(this);
		Possess(Creature);
	}, CameraSwitchTime, false);
}

void ACordyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABaseCreature* Creature = Cast<ABaseCreature>(InPawn);
	if(!Creature)
	{
		return;
	}

	Creature->SetCaptured();
	
	UTargetSelectionComponent* Targeting = InPawn->FindComponentByClass<UTargetSelectionComponent>();
	if(Targeting)
	{
		Targeting->Activate();
	}

}

void ACordyController::OnUnPossess()
{
	APawn* PrevPawn = GetPawn();
	if(!PrevPawn)
	{
		Super::OnUnPossess();
		return;
	}

	//Disable any targeting
	UTargetSelectionComponent* Targeting = PrevPawn->FindComponentByClass<UTargetSelectionComponent>();
	if(Targeting)
	{
		Targeting->Deactivate();
	}
	
	//Put AI on Old Pawn
	PrevPawn->SpawnDefaultController();
	
	Super::OnUnPossess();
}
