// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/SporeDispenserComponent.h"

#include "Spores.h"

// Sets default values for this component's properties
USporeDispenserComponent::USporeDispenserComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USporeDispenserComponent::TrySpawnSpores()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(TimerManager.IsTimerActive(CooldownHandle))
	{
		return;
	}
	
	if(!ensure(SporesClass != nullptr))
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = GetOwner();
	ASpores* Spores = GetWorld()->SpawnActor<ASpores>(SporesClass, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, Params);
	if(!Spores)
	{
		return;
	}

	
	TimerManager.SetTimer(CooldownHandle, CooldownTime, false);
}

