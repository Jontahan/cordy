// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/CordyController.h"

#include "ActionInteractable.h"
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
	InputComponent->BindAction("Interact", IE_Pressed, this, &ACordyController::TryInteract);
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

void ACordyController::TryInteract()
{
	if(bIsDoingAction)
	{
		return;
	}
	
	ABaseCreature* Creature = Cast<ABaseCreature>(GetPawn());
	if(!Creature)
	{
		return;
	}
	
	UTargetSelectionComponent* Targeting = Creature->FindComponentByClass<UTargetSelectionComponent>();
	if(!Targeting)
	{
		return;
	}
	
	AActionInteractable* ActionInteract = Cast<AActionInteractable>(Targeting->GetCurrentTarget());
	if(!ActionInteract)
	{
		return;
	}

	AActionLocation* Loc = ActionInteract->ActionLocation;
	if(!Loc)
	{
		return;
	}

	switch(ActionInteract->InteractionAction)
	{
	case EActionType::Work:
		if(!Loc->CanDoWork())
		{
			break;
		}
		DoWork(Loc);
		Creature->Actions.Add({Loc, EActionType::Work});
		
		break;
		
	case EActionType::Pickup:
		if(Creature->Pickup(Loc))
		{
			break;
		}
		Creature->Actions.Add({Loc, EActionType::Pickup});
		break;
		
	case EActionType::Drop:
		if(!Creature->Drop(Loc))
		{
			break;
		}
		Creature->Actions.Add({Loc, EActionType::Drop});
		break;
		
	}
}

void ACordyController::SwitchTo(ABaseCreature* Creature)
{
	if(!Creature || bIsDoingAction || GetPawn() == Creature)
	{
		return;
	}

	if(!Creature->IsCaptured())
	{
		return;
	}

	bIsDoingAction = true;
	SetViewTargetWithBlend(Creature, CameraSwitchTime);
	GetPawn()->DisableInput(this);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerHandle handle;
	TimerManager.SetTimer(handle, [this, Creature]()
	{
		bIsDoingAction = false;
		GetPawn()->EnableInput(this);
		Possess(Creature);
	}, CameraSwitchTime, false);
}

void ACordyController::DoWork(AActionLocation* Location)
{
	ABaseCreature* Creature = Cast<ABaseCreature>(GetPawn());
	float WorkDuration = Creature->DoWork(Location);
	bIsDoingAction = true;
	Creature->DisableInput(this);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this, Location, Creature]()
	{
		bIsDoingAction = false;
		Creature->EnableInput(this);
		Location->DidWork();
	}, WorkDuration, false);
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
	
	Super::OnUnPossess();

	//Put AI on Old Pawn
	PrevPawn->SpawnDefaultController();
}
