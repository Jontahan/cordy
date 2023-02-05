// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionLocation.h"
#include "GameFramework/Character.h"
#include "BaseCreature.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWasCaptured, ABaseCreature*, Creature);

UCLASS()
class CORDY_API ABaseCreature : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseCreature();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetCaptured();
	bool IsCaptured() const { return bIsCaptured; }

	float DoWork(AActionLocation* Location);
	bool Pickup(AActionLocation* Location);
	bool Drop(AActionLocation* Location);

	FVector GetSpawnLocation() const { return SpawnLocation; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cordy")
	bool bMoveRandomly{true};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cordy")
	FVector2D RandomMoveInterval{1, 10};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cordy")
	float RandomMoveDistance{1500};

	UPROPERTY(BlueprintAssignable)
	FWasCaptured WasCaptured;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FActionReplay> Actions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentActionIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<EResourceType> ResourcesCarried;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cordy")
	bool bIsCaptured{true};
	
private:
	void MoveForward(float Value);
	void Turn(float Value);

	FVector SpawnLocation;
};
