// Fill out your copyright notice in the Description page of Project Settings.

#include "UDP_Component.h"

#include "Messages.h"
#include "CustomData.h"



////////////////////////////////////////
//       INITIALIZATION STUFF
////////////////////////////////////////

// Sets default values for this component's properties
UUDP_Component::UUDP_Component() : SourceIP_Address(FString("0.0.0.0")), PortIn(8000),
DestIP_Address(FString("127.0.0.1")), PortOut(9000)
{
	bAutoActivate = false;
	PrimaryComponentTick.bCanEverTick = false; // Disable tick every frame

	ListenSocket = NULL;
	SendSocket = NULL;

	//DataIn.Init(0, sizeof(struct FCustomPoseData));
	DataOut.Init(0, sizeof(struct FCustomImpactData));
}


// Initialize the communication
bool UUDP_Component::StartUDPComm(const FString& YourChosenSocketName)
{
	ScreenMsg("SOCKETS INIT");
	ScreenMsg("Source IP = ", SourceIP_Address);
	ScreenMsg("Source Port = ", PortIn);
	ScreenMsg("Destination IP = ", DestIP_Address);
	ScreenMsg("Destination Port = ", PortOut);

	FIPv4Address SourceAddr, DestAddr;

	// Parse the IP addresses
	FIPv4Address::Parse(SourceIP_Address, SourceAddr);
	FIPv4Address::Parse(DestIP_Address, DestAddr);

	// Create the endpoint object
	FIPv4Endpoint InputEndpoint(SourceAddr, PortIn);
	FIPv4Endpoint OutputEndpoint(DestAddr, PortOut);

	bool valid = false;
	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	RemoteAddr->SetIp(*DestIP_Address, valid);
	RemoteAddr->SetPort(PortOut);

	if (!valid)
	{
		ScreenMsg("Problem with the remote Address!");
		return false;
	}

	// BUFFER SIZE
	int32 BufferSize = 2 * 1024 * 1024;

	//Create Listener Socket
	ListenSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(InputEndpoint)
		.WithReceiveBufferSize(BufferSize);

	// Create Sender Socket
	SendSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.WithSendBufferSize(BufferSize)
		.WithBroadcast();

	// Spaw a receiving thread
	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(4);
	UDPReceiver = new FUdpSocketReceiver(ListenSocket, ThreadWaitTime, TEXT("UDP RECEIVER"));
	UDPReceiver->OnDataReceived().BindUObject(this, &UUDP_Component::Recv);
	ScreenMsg("Starting Listening Thread...");
	UDPReceiver->Start();

	return true;
}


////////////////////////////////////////
//			 FUNCTIONS
////////////////////////////////////////

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
* Receiving Part
* The receiving thread calls this function, which copy the data in the class variable
* DataIn.
*/
void UUDP_Component::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
	//ScreenMsg("Received bytes", ArrayReaderPtr->Num());
	//ArrayReaderPtr->Serialize(DataIn.GetData(), ArrayReaderPtr->Num());
	*ArrayReaderPtr << DataIn;
}

/**
* Return the retrieved data
*/
void UUDP_Component::GetData(FCustomPoseData* RetData)
{
	*RetData = DataIn;
	//ScreenMsg("Value", *((float*)(RetData->GetData()) + 3));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sending Part
//
int UUDP_Component::SendData(TArray<uint8> Array)
{
	int32 byteSent = 0;

	SendSocket->SendTo(Array.GetData(), Array.Num(), byteSent, *RemoteAddr);

	return byteSent;
}

int UUDP_Component::SendData(uint8* Data, int Nbytes)
{
	int32 byteSent = 0;

	SendSocket->SendTo(Data, Nbytes, byteSent, *RemoteAddr);

	return byteSent;
}


////////////////////////////////////////
//			    ENDING
////////////////////////////////////////
void UUDP_Component::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ScreenMsg("Closing Sockets...");
	if (UDPReceiver != nullptr)
	{
		UDPReceiver->Stop();
		delete UDPReceiver;
		UDPReceiver = nullptr;
	}

	//Clear all sockets!
	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}

	if (SendSocket)
	{
		SendSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SendSocket);
	}

}

