// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/Spores.h"

#include "BaseCreature.h"

// Sets default values
ASpores::ASpores()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpores::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ABaseCreature* Creature = Cast<ABaseCreature>(OtherActor);
	if(!Creature || Creature == GetOwner())
	{
		return;
	}

	ABaseCreature* Original = Cast<ABaseCreature>(GetOwner());
	if(Original)
	{
		Creature->Actions = Original->Actions;
	}
	
	if(!Creature->IsCaptured())
	{
		Creature->SetCaptured();
	}
}

