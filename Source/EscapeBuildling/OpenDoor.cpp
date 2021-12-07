// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("The Actor %s has OpenDoor but no pressure plate selected!"), *GetOwner()->GetName());
	}

	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpen))
	{
		OpenThatDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f"), DoorLastOpened);
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorDelay)
		{
			CloseThatDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenThatDoor(float DeltaTime)
{
	FRotator OpenDoor(0.f, CurrentYaw, 0.f);
	CurrentYaw = FMath::Lerp(CurrentYaw, -TargetYaw, DeltaTime * DoorOpenSpeed);
	GetOwner()->SetActorRotation(OpenDoor);
}

void UOpenDoor::CloseThatDoor(float DeltaTime)
{
	FRotator CloseDoor(0.f, CurrentYaw, 0.f);
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	GetOwner()->SetActorRotation(CloseDoor);
}

