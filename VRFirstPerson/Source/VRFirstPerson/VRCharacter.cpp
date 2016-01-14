// Fill out your copyright notice in the Description page of Project Settings.

#include "VRFirstPerson.h"

/* VR Includes */
#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"

#include "VRCharacter.h"


// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VROriginComp = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraOrigin"));
	VROriginComp->AttachParent = RootComponent;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	/* Assign to the VR origin component so any reset calls to the HMD can reset to 0,0,0 relative to this component */
	CameraComp->AttachParent = VROriginComp;

	LeftHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHandComponent->Hand = EControllerHand::Left;
	LeftHandComponent->AttachParent = VROriginComp;

	RightHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHandComponent->Hand = EControllerHand::Right;
	RightHandComponent->AttachParent = VROriginComp;

	bPositionalHeadTracking = false;
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetupVROptions();
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("ToggleTrackingSpace", IE_Pressed, this, &AVRCharacter::ToggleTrackingSpace);
	InputComponent->BindAction("ResetHMDOrigin", IE_Pressed, this, &AVRCharacter::ResetHMDOrigin);
}

void AVRCharacter::SetupVROptions()
{
	IHeadMountedDisplay* HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
	if (HMD && HMD->IsStereoEnabled())
	{
		/* Disable/Enable positional movement to pin camera translation */
		HMD->EnablePositionalTracking(bPositionalHeadTracking);

		/* Remove any translation when disabling positional head tracking */
		if (!bPositionalHeadTracking)
		{
			CameraComp->SetRelativeLocation(FVector(0, 0, 0));
		}
	}
}


void AVRCharacter::ResetHMDOrigin()
{
	IHeadMountedDisplay* HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
	if (HMD && HMD->IsStereoEnabled())
	{
		HMD->ResetOrientationAndPosition();
	}
}


void AVRCharacter::ToggleTrackingSpace()
{
	// TODO: Fix module includes for SteamVR

	//@todo Make this safe once we can add something to the DeviceType enum.  For now, make the terrible assumption this is a SteamVR device.
// 	FSteamVRHMD* SteamVRHMD = (FSteamVRHMD*)(GEngine->HMDDevice.Get());
// 	if (SteamVRHMD && SteamVRHMD->IsStereoEnabled())
// 	{
// 		ESteamVRTrackingSpace TrackingSpace = SteamVRHMD->GetTrackingSpace();
// 		SteamVRHMD->SetTrackingSpace(TrackingSpace == ESteamVRTrackingSpace::Seated ? ESteamVRTrackingSpace::Standing : ESteamVRTrackingSpace::Seated);
// 	}
}

