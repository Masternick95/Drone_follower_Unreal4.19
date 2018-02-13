#pragma once

#include "CustomData.generated.h"
USTRUCT()
struct FCustomPoseData
{
	GENERATED_USTRUCT_BODY()

	//DRONE
	// Position [m]
	float drone_X = 0.0f;
	float drone_Y = 0.0f;
	float drone_Z = 0.0f;

	// Attitude [rad]
	float drone_Roll = 0.0f;
	float drone_Pitch = 0.0f;
	float drone_Yaw = 0.0f;
	
	//Ball position
	float ball_X = 0.0f;
	float ball_Y = 0.0f;
	float ball_Z = 0.0f;

	FCustomPoseData() {}
};



USTRUCT()
struct FCustomImpactData
{
	GENERATED_USTRUCT_BODY()

		// Object ID
		int Id = 0;

	// Collision Direction
	float Nx = 0;
	float Ny = 0;
	float Nz = 0;

	// Penetration [m]
	float Penetration = 0.0f;

	FCustomImpactData() {}
};


FORCEINLINE FArchive& operator<<(FArchive &Ar, FCustomPoseData& TheStruct)
{
	Ar << TheStruct.drone_X;
	Ar << TheStruct.drone_Y;
	Ar << TheStruct.drone_Z;
	Ar << TheStruct.drone_Roll;
	Ar << TheStruct.drone_Pitch;
	Ar << TheStruct.drone_Yaw;
	Ar << TheStruct.ball_X;
	Ar << TheStruct.ball_Y;
	Ar << TheStruct.ball_Z;

	return Ar;
}


FORCEINLINE FArchive& operator<<(FArchive &Ar, FCustomImpactData& TheStruct)
{
	Ar << TheStruct.Id;
	Ar << TheStruct.Nx;
	Ar << TheStruct.Ny;
	Ar << TheStruct.Nz;
	Ar << TheStruct.Penetration;

	return Ar;
}
