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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	FindInputHandle();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReach());
	}
}

// Grabbed Object
void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBody();
	UPrimitiveComponent *GrabComponent = HitResult.GetComponent();
	AActor *HitActor = HitResult.GetActor();

	if (HitActor)
	{
		if (!HitActor){return;}
		//TODO Check distance from player to object
		PhysicsHandle->GrabComponentAtLocation
		(
			GrabComponent,
			NAME_None,
			GetReach()
		);
	}
}

// Released Object
void UGrabber::Release()
{
	if(!PhysicsHandle){return; }
	PhysicsHandle->ReleaseComponent();
}

// Find physics component
void UGrabber::FindPhysicsHandle()
{
	//Check for physics handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component Attached to %s"), *GetOwner()->GetName());
	}
}

// Find input component
void UGrabber::FindInputHandle()
{
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputHandle)
	{
		InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputHandle->BindAction("Release", IE_Released, this, &UGrabber::Release);
	}
}

// Finds object to grab
FHitResult UGrabber::GetFirstPhysicsBody() const
{
	// Logging out test for when object within range
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

// Gets reach of player
FVector UGrabber::GetReach() const
{
	FVector PlayerLocation = GetOwner()->GetActorLocation();
	FRotator PlayerRotation = GetOwner()->GetActorRotation();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	return PlayerLocation + PlayerRotation.Vector() * Reach;
}

// Gets player world location
FVector UGrabber::GetPlayerLocation() const
{
	FVector PlayerViewLocation;
	FRotator PLayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewLocation,
		OUT PLayerViewRotation 
	);

	return PlayerViewLocation;
}