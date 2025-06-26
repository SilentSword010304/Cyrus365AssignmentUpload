// Copyright Epic Games, Inc. All Rights Reserved.


#include "Cyros365AssignmentPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Cyros365AssignmentCameraManager.h"

ACyros365AssignmentPlayerController::ACyros365AssignmentPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = ACyros365AssignmentCameraManager::StaticClass();
}

void ACyros365AssignmentPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
