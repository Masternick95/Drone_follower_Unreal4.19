// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball.h"
#include "Messages.h"
#include "CustomData.h"
#include "ActorsController.h"
#include "Drone_follower.h"


// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorLocation(FVector(300.0f, -270.0f, 630.0f));

	//Instantiate static mesh component
	BallVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallVisibleComponent"));

	//Get actor controller pointer
	//Get ActorsController pointer for receiving position
	for (TObjectIterator<AActorsController> Itr; Itr; ++Itr) {
		if (Itr->IsA(AActorsController::StaticClass())) {
			ActorController = *Itr;
		}
	}
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	FCustomPoseData NewData;

	Super::Tick(DeltaTime);
	
	ActorController->ReturnNewData(&NewData);
	SetPose(&NewData);
}

void ABall::SetPose(FCustomPoseData* ReceivedData) {
	FVector Position;

	//Received measures are in cm --> convert into m
	Position.X = 100 * ReceivedData->ball_X;
	Position.Y = 100 * ReceivedData->ball_Y;
	Position.Z = 100 * ReceivedData->ball_Z;

	SetActorLocation(Position);
}