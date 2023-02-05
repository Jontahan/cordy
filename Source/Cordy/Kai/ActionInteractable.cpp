// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/ActionInteractable.h"

#include "ActionLocation.h"

// Sets default values
AActionInteractable::AActionInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

bool AActionInteractable::CanBeTargetedBy_Implementation(AActor* Other)
{
	if(!ActionLocation)
	{
		return false;
	}

	if(InteractionAction == EActionType::Work && !ActionLocation->CanDoWork())
	{
		return false;
	}

	return true;
}

// Called when the game starts or when spawned
void AActionInteractable::BeginPlay()
{
	Super::BeginPlay();

	AActor* Own = GetAttachParentActor();
	ActionLocation = Cast<AActionLocation>(Own);
	ensure(ActionLocation);
}

