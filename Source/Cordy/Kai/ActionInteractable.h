// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionLocation.h"
#include "ITargetableInterface.h"
#include "GameFramework/Actor.h"
#include "ActionInteractable.generated.h"

class AActionLocation;

UCLASS()
class CORDY_API AActionInteractable : public AActor, public ITargetableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActionInteractable();

	//ITargetableInterface
	virtual bool CanBeTargetedBy_Implementation(AActor* Other) override;

	UPROPERTY(VisibleAnywhere)
	AActionLocation* ActionLocation{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Cordy")
	EActionType InteractionAction{EActionType::Work};
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
