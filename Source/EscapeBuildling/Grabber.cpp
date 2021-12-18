// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

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
	
	FVector PlayerLocation = GetOwner()->GetActorLocation();
	FRotator PlayerRotation = GetOwner()->GetActorRotation();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

// Grabbed Object
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));

	FVector PlayerLocation = GetOwner()->GetActorLocation();
	FRotator PlayerRotation = GetOwner()->GetActorRotation();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;


	FHitResult HitResult = GetFirstPhysicsBody();
	UPrimitiveComponent *GrabComponent = HitResult.GetComponent();

	if (HitResult.GetActor())
	{
		//TODO Check distance from player to object
		PhysicsHandle->GrabComponentAtLocation
		(
			GrabComponent,
			NAME_None,
			LineTraceEnd
		);
	}
}

// Released Object
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release"));

	FHitResult HitResult = GetFirstPhysicsBody();
	UPrimitiveComponent *GrabComponent = HitResult.GetComponent();

	PhysicsHandle->ReleaseComponent();
}

// Find physics component
void UGrabber::FindPhysicsHandle()
{
		//Check for physics handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component Attached to %s"), *GetOwner()->GetName());
	}
	else
	{
		//Physics handle found
	}
}

// Find input component
void UGrabber::FindInputHandle()
{
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!InputHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Handle component on: %S"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input handle found"));
		InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputHandle->BindAction("Release", IE_Released, this, &UGrabber::Release);
	}
}

// Finds object to grab
FHitResult UGrabber::GetFirstPhysicsBody() const
{
	FVector PlayerLocation = GetOwner()->GetActorLocation();
	FRotator PlayerRotation = GetOwner()->GetActorRotation();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	// Logging out test for when object within range
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor *Impactor = Hit.GetActor();

	if (!Impactor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing is grabbed"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is grabbed"), *Impactor->GetName());
	}
	return Hit;
}
