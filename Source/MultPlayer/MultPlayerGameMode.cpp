// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultPlayerGameMode.h"
#include "MultPlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultPlayerGameMode::AMultPlayerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
