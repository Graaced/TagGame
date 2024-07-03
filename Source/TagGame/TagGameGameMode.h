// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/TargetPoint.h"
#include "Ball.h"
#include "TagGameGameMode.generated.h" // da lasciare per ultimo !

UCLASS(minimalapi)
class ATagGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	TArray<ATargetPoint*> TargetPoints; // elenco di tutti i point di dove spawnerà la palla
	TArray<ABall*> GameBalls;

public:
	ATagGameGameMode();


	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void ResetMatch();

	const TArray<class ABall*>& GetBalls() const;

};



