// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
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


private:

	float InitialYaw, CurrentYaw;
	float DoorLastOpened = 0.f;
	float DoorDelay = 0.25f;

	UPROPERTY(EditAnywhere)
		float TargetYaw = 90.f;
	
	UPROPERTY(EditAnywhere)
		ATriggerVolume *PressurePlate;

	UPROPERTY(EditAnywhere)
		AActor* ActorThatOpen;

	UPROPERTY(EditAnywhere)
		float DoorOpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
		float DoorCloseSpeed = 2.f;
};
