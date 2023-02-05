// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/ActionLocation.h"

// Sets default values
AActionLocation::AActionLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AActionLocation::Pickup()
{
	if(Resources <= 0)
	{
		return false;
	}

	--Resources;
	return true;
}

bool AActionLocation::Drop()
{
	//TODO: Max resources?
	return true;
}

// Called when the game starts or when spawned
void AActionLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

