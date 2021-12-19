// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#define OUT

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

	FindAudio();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > 50.f)
	{
		OpenThatDoor(DeltaTime);
	}
	else if (GetWorld()->GetTimeSeconds()- DoorLastOpened > DoorLastOpened)
	{
		CloseThatDoor(DeltaTime);
	}
}

// Opens Door
void UOpenDoor::OpenThatDoor(float DeltaTime)
{
	FRotator OpenDoor(0.f, CurrentYaw, 0.f);
	CurrentYaw = FMath::Lerp(CurrentYaw, -TargetYaw, DeltaTime * DoorOpenSpeed);
	GetOwner()->SetActorRotation(OpenDoor);

	CloseDoorSound = false;
	if(!AudioComponent){return;}
	if(!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

// Closes Door
void UOpenDoor::CloseThatDoor(float DeltaTime)
{
	FRotator CloseDoor(0.f, CurrentYaw, 0.f);
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	GetOwner()->SetActorRotation(CloseDoor);

	OpenDoorSound = false;
	if (!AudioComponent){return;}
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = true;
	}
}

// Calculate mass of actors to trigger door
float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find overlapping actor
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate){return TotalMass;}
	PressurePlate->GetOverlappingActors(OverlappingActors);

	// Add their masses
	for (AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::FindAudio()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No audio component detected on %s"), *GetOwner()->GetName());
	}
	else
	{
		return;
	}
	
}