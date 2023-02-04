// Fill out your copyright notice in the Description page of Project Settings.


#include "Cordy/Kai/TargetSelectionComponent.h"

#include "ITargetableInterface.h"

// Sets default values for this component's properties
UTargetSelectionComponent::UTargetSelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
}

// Called when the game starts
void UTargetSelectionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const TArray<UActorComponent*> Components = GetOwner()->GetComponentsByTag(UPrimitiveComponent::StaticClass(), ColliderTag);
	if(!ensureMsgf(Components.Num() > 0, TEXT("Missing Targeting Collider")))
	{
		return;
	}
	
	TargetingCollider = Cast<UPrimitiveComponent>(Components[0]);
}

void UTargetSelectionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!ensure(TargetingCollider))
	{
		return;
	}

	TArray<AActor*> PotentialTargets;
	TargetingCollider->GetOverlappingActors(PotentialTargets);

	AActor* Closest = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();
	for(AActor* Actor : PotentialTargets)
	{
		if(Actor == GetOwner())
		{
			continue;
		}
		
		if(!Actor->GetClass()->ImplementsInterface(UTargetableInterface::StaticClass()))
		{
			continue;
		}

		if(!ITargetableInterface::Execute_CanBeTargetedBy(Actor, GetOwner()))
		{
			continue;
		}

		const float Distance = FVector::Distance(Actor->GetActorLocation(), GetOwner()->GetActorLocation());
		if(Distance < ClosestDistance)
		{
			Closest = Actor;
			ClosestDistance = Distance;
		}
	}

	SetTarget(Closest);
}

void UTargetSelectionComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	SetComponentTickEnabled(true);
}

void UTargetSelectionComponent::Deactivate()
{
	SetComponentTickEnabled(false);
	SetTarget(nullptr);
	Super::Deactivate();
}

void UTargetSelectionComponent::SetTarget(AActor* NewTarget)
{
	if(CurrentTarget != nullptr)
	{
		ITargetableInterface::Execute_SetIsTargetedBy(CurrentTarget, nullptr);
		CurrentTarget = nullptr;
	}
	
	if(NewTarget != nullptr)
	{
		CurrentTarget = NewTarget;
		ITargetableInterface::Execute_SetIsTargetedBy(CurrentTarget, GetOwner());
	}
}

