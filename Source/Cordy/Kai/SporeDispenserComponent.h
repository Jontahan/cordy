// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SporeDispenserComponent.generated.h"

class ASpores;

UCLASS( Abstract, Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CORDY_API USporeDispenserComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USporeDispenserComponent();

protected:
	UFUNCTION(BlueprintCallable)
	void TrySpawnSpores();

	UPROPERTY(EditDefaultsOnly, Category = "Cordy")
	TSubclassOf<ASpores> SporesClass{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Cordy")
	float CooldownTime{2.5f};

private:
	FTimerHandle CooldownHandle;
};
