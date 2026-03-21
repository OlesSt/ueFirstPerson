// Copyright Epic Games, Inc. All Rights Reserved.

#include "ueFirstPersonGameMode.h"
#include "ueFirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AueFirstPersonGameMode::AueFirstPersonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
