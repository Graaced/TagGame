// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameGameMode.h"
#include "TagGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "EnemyAIController.h"
#include "EngineUtils.h"

ATagGameGameMode::ATagGameGameMode()
{
	// set default pawn class to our Blueprinted character
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


}

void ATagGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	ResetMatch();
}

const TArray<class ABall*>& ATagGameGameMode::GetBalls() const 
{
	return GameBalls;
}

void ATagGameGameMode::Tick(float DeltatTime)
{

	Super::Tick(DeltatTime);

	bool AllBallsPickedByPlayer = true;


	for (int32 i = 0; i < GameBalls.Num(); i++)
	{
		if(GameBalls[i]->GetAttachParentActor() != GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			AllBallsPickedByPlayer = false;
			break;
		}
	}

	if (AllBallsPickedByPlayer)
	{
		ResetMatch();
	}

	//ResetMatch();
}

void ATagGameGameMode::ResetMatch()
{
		
	TargetPoints.Empty(); // ripuliamo l'array

	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It) // iteriamo tutti i punti
	{
		TargetPoints.Add(*It);
		
	}

	GameBalls.Empty(); // ripuliamo l'array

	for (TActorIterator<ABall> It(GetWorld()); It; ++It) // iteriamo tutti i punti
	{
		if(It->GetAttachParentActor())
		{
			It->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		}

		GameBalls.Add(*It);

	}

	
	TArray<ATargetPoint*> RandomTargetPoints = TargetPoints; // mi faccio la copia dell'aray di target points ( per evitare che venga riutilizzato lo stesso targetPoint)

	for (int32 i = 0; i < GameBalls.Num(); i++)
	{
		// dobbiamo distribuire ogni ball su un target point diverso 
		const int32 RandomTargetIndex = FMath::RandRange(0, RandomTargetPoints.Num() - 1);

		GameBalls[i]->SetActorLocation(RandomTargetPoints[RandomTargetIndex]->GetActorLocation());
		RandomTargetPoints.RemoveAt(RandomTargetIndex);  // una volta che mi sono preso l'oggetto che mi interessa, lo rimuovo dell'array
	}

	

	
}


