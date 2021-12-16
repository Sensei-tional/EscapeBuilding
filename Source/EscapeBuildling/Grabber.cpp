// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get Player Viewpoint
	FVector PlayerLocation = GetOwner()->GetActorLocation();
	FRotator PlayerRotation = GetOwner()->GetActorRotation();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	
	UE_LOG(LogTemp, Warning, TEXT("The %s is located at: %s , rotation is: %s"),
	*GetOwner()->GetName(), *PlayerLocation.ToString(), *PlayerRotation.ToString()
	);

	// Draw a debug line
	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerLocation,
		LineTraceEnd,
		FColor(155, 0, 255),
		false,
		0.f,
		0,
		5.f
	);
}

