// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionLocation.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Work,
	Pickup,
	Drop
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Wood,
	Stone,
};

USTRUCT(BlueprintType)
struct FActionReplay
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	AActionLocation* Location;

	UPROPERTY(BlueprintReadWrite)
	EActionType Action;
};

UCLASS(Abstract)
class CORDY_API AActionLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	AActionLocation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool CanDoWork();

	UFUNCTION(BlueprintCallable)
	EResourceType GetResourceType() const { return ResourceType; }

	//Do the work for the returned amount of seconds
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	float DoWork();
	
	//Notify we did the work(Increment/Decrement resources)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DidWork();

	UFUNCTION(BlueprintCallable)
	bool Pickup();

	UFUNCTION(BlueprintCallable)
	bool Drop();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cordy")
	EResourceType ResourceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cordy")
	float WorkDuration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cordy")
	int Resources;
};
