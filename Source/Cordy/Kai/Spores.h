// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spores.generated.h"

UCLASS()
class CORDY_API ASpores : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpores();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
