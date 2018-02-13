// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorsController.h"
#include "Drone_follower.h"
#include "Messages.h"
#include "CustomData.h"
#include "EngineUtils.h"


// Sets default values
AActorsController::AActorsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Istantiate Communication Component
	OurCommunicationComponent = CreateDefaultSubobject<UUDP_Component>(TEXT("Communication Component"));

}

// Called when the game starts or when spawned
void AActorsController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get Data
	OurCommunicationComponent->GetData(&ReceivedData);	//Store in a local variable the received data

	//Serialize received data
	/*for (i = 0; i < sizeof(struct FCustomPoseData); i++){
		*((uint8*)(&VehiclePose) + i) = ReceivedData[i];
	}*/
}

void AActorsController::ReturnNewData(FCustomPoseData *NewData) {
	*NewData = ReceivedData;
}

void AActorsController::PreInitializeComponents() {
	Super::PreInitializeComponents();
	OurCommunicationComponent->StartUDPComm("PawnCommunicationComponent");
}

