// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TestingGrounds.h"
#include "TestingGroundsGameMode.h"
#include "TestingGroundsHUD.h"
#include "Player/FirstPersonCharacter.h"

ATestingGroundsGameMode::ATestingGroundsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Character/Behaviour/Character_BP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATestingGroundsHUD::StaticClass();
}
