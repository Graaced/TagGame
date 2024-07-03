// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "TagGameGameMode.h"
#include "Ball.h"


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	
	InitializeStates();

	// Imposta lo stato iniziale
	if (CurrentState)
	{
		CurrentState->CallEnter(this);
	}

}

void AEnemyAIController::InitializeStates()
{

	BestBall = nullptr;

	GoToPlayer = MakeShared<FAivState>(
		[](AAIController* AIController) {

			AIController->MoveToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn(), 100.f);
		},
		[this](AAIController* AIController, const float DeltaTime)->TSharedPtr<FAivState> {

			EPathFollowingStatus::Type State = AIController->GetMoveStatus();

			ATagGameCharacter* GameCharacter = Cast<ATagGameCharacter>(AIController->GetWorld()->GetFirstPlayerController()->GetPawn());

			if (GameCharacter->bHasCube)
			{
				return CantMove;

			}

			if (State == EPathFollowingStatus::Moving)
			{
				return nullptr;
			}

			if (BestBall)
			{

				BestBall->AttachToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
				BestBall->SetActorRelativeLocation(FVector(0, 0, 0));
				BestBall = nullptr;
			}
			return SearchForBall;
		},
		nullptr

	);

	CantMove = MakeShared<FAivState>(
		[this](AAIController* AIController) {
			
			AIController->StopMovement();
		},
		[this](AAIController* AIController, const float DeltaTime)->TSharedPtr<FAivState> {
			
			ATagGameCharacter* GameCharacter = Cast<ATagGameCharacter>(UGameplayStatics::GetPlayerCharacter(AIController->GetWorld(), 0));
			if(!GameCharacter->bHasCube)
			{
				return GoToPlayer;
			}

			
			return nullptr;
		},
		nullptr
	);



	SearchForBall = MakeShared<FAivState>(
		[this](AAIController* AIController) {
			// Prendo il Gamemode
			AGameModeBase* GameMode = AIController->GetWorld()->GetAuthGameMode();
			ATagGameGameMode* AIGameMode = Cast<ATagGameGameMode>(GameMode);
			const TArray <ABall*>& BallsList = AIGameMode->GetBalls();

			// trovo la palla più vicina ( del pawn non a me stesso che sono un controller  ) 

			ABall* NearestBall = nullptr;

			for (int32 i = 0; i < BallsList.Num(); i++)
			{
				if (!BallsList[i]->GetAttachParentActor() &&
					(!NearestBall ||
						FVector::Distance(AIController->GetPawn()->GetActorLocation(), BallsList[i]->GetActorLocation()) <
						FVector::Distance(AIController->GetPawn()->GetActorLocation(), NearestBall->GetActorLocation())))
				{
					NearestBall = BallsList[i];
				}
			}

			BestBall = NearestBall;

		},
		[this](AAIController* AIController, const float DeltaTime)->TSharedPtr<FAivState> {

			if (BestBall)
			{

				return GoToBall;
			}
			else
			{
				return SearchForBall;
			}



		},
		nullptr

	);


	GoToBall = MakeShared<FAivState>(
		[this](AAIController* AIController) {

			AIController->MoveToActor(BestBall, 100.f);
		},
		[this](AAIController* AIController, const float DeltaTime)->TSharedPtr<FAivState> {

			EPathFollowingStatus::Type State = AIController->GetMoveStatus();

			if (State == EPathFollowingStatus::Moving)
			{
				return nullptr;
			}

			return GrabBall;
		},
		nullptr

	);

	GrabBall = MakeShared<FAivState>(
		[this](AAIController* AIController) {

			if (BestBall->GetAttachParentActor()) // se la palla è gia agganciata ( presa) da qualcuno non va più bene come palla da prendere 
			{
				BestBall = nullptr;
			}
		},
		[this](AAIController* AIController, const float DeltaTime)->TSharedPtr<FAivState> {


			if (BestBall == nullptr)
			{
				return SearchForBall;
			}

			// se la prendo la palla poi vado dal player
			BestBall->AttachToActor(AIController->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
			BestBall->SetActorRelativeLocation(FVector(0, 0, 0));

			return GoToPlayer;
		},
		nullptr

	);

	CurrentState = SearchForBall;
	CurrentState->CallEnter(this);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState)
	{
		CurrentState = CurrentState->CallTick(this, DeltaTime);
	}

}


