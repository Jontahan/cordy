// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITargetableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargetableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CORDY_API ITargetableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	bool CanBeTargetedBy(AActor* Other);
	
	UFUNCTION(BlueprintNativeEvent)
	void SetIsTargetedBy(AActor* Other);
};
