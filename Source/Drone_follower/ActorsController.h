// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDP_Component.h"
#include "CustomData.h"
#include "ActorsController.generated.h"

UCLASS()
class DRONE_FOLLOWER_API AActorsController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorsController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PreInitializeComponents() override;

	void ReturnNewData(FCustomPoseData* NewData);
    //void ReturnNewData(TArray<uint8> *NewData);

private:

	UPROPERTY(EditAnywhere, Category = "Remote Address")
	class UUDP_Component* OurCommunicationComponent;
	
	FCustomPoseData ReceivedData;
    //TArray<uint8> ReceivedData;
};
