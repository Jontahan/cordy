// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetSelectionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CORDY_API UTargetSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetSelectionComponent();

	AActor* GetCurrentTarget() const { return CurrentTarget; }
	virtual void Activate(bool bReset=false) override;
	virtual void Deactivate() override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void SetTarget(AActor* NewTarget);

	UPROPERTY()
	UPrimitiveComponent* TargetingCollider{nullptr};

private:
	UPROPERTY(VisibleAnywhere)
	AActor* CurrentTarget{nullptr};

	UPROPERTY(EditDefaultsOnly)
	FName ColliderTag{"Targeting"};
};
