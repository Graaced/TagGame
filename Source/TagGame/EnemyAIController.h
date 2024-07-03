// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ball.h"
#include "TagGameCharacter.h"
#include "EnemyAIController.generated.h"


struct FAivState : public TSharedFromThis<FAivState>// rappresenta uno stato 
{
	// gli stati sono entrata, tick, uscita
	TFunction<void(AAIController*)> Enter;
	TFunction<TSharedPtr<FAivState>(AAIController*, const float)> Tick;
	TFunction<void(AAIController*)>  Exit;

public:


	FAivState()
	{
		Enter = nullptr;
		Tick = nullptr;
		Exit = nullptr;

	}

	FAivState(TFunction<void(AAIController*)> InEnter = nullptr, TFunction<TSharedPtr<FAivState>(AAIController*, const float)> InTick = nullptr, TFunction<void(AAIController*)> InExit = nullptr)
	{
		Enter = InEnter;
		Tick = InTick;
		Exit = InExit;
	}

	FAivState(const FAivState& Other) = delete;
	FAivState& operator=(const FAivState& Other) = delete;
	FAivState(FAivState&& Other) = delete;
	FAivState& operator=(FAivState&& Other) = delete;

	void CallEnter(AAIController* AIController)
	{
		if (Enter)
		{
			Enter(AIController);
		}
	}

	TSharedPtr<FAivState> CallTick(AAIController* AIController, const float DeltaTime)
	{
		if (Tick)
		{
			TSharedPtr<FAivState> NewState = Tick(AIController, DeltaTime);

			if (NewState != nullptr && NewState != AsShared())
			{
				CallExit(AIController);
				NewState->CallEnter(AIController);
				return NewState;
			}

		}

		return AsShared();


	}

	void CallExit(AAIController* AIController)
	{
		if (Exit)
		{
			Exit(AIController);
		}
	}

};



UCLASS()
class TAGGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
protected:


	// STATES 
	TSharedPtr<FAivState> CurrentState;

	// TO PLAYER'S STATES
	TSharedPtr<FAivState> GoToPlayer;

	// BALL'S STATES
	TSharedPtr<FAivState> SearchForBall;
	TSharedPtr<FAivState> GoToBall;
	TSharedPtr<FAivState> GrabBall;
	TSharedPtr<FAivState> CantMove;

	// FUNCTIONS
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	
	void InitializeStates();
	

	
	ABall* BestBall;

};




