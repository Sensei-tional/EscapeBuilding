// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/PrimitiveComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEBUILDLING_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenThatDoor(float DeltaTime);
	void CloseThatDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudio();

private:

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;
	float InitialYaw, CurrentYaw;
	float DoorLastOpened = 0.f;
	float DoorDelay = 0.25f;

	UPROPERTY()
	UAudioComponent *AudioComponent = nullptr;

	UPROPERTY(EditAnywhere)
		float TargetYaw = 90.f;
	
	UPROPERTY(EditAnywhere)
		ATriggerVolume *PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		float DoorOpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
		float DoorCloseSpeed = 2.f;

	UPROPERTY(EditAnywhere)
		float TotalMass = 10.f;
};
