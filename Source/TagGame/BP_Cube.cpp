// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_Cube.h"

// Sets default values
ABP_Cube::ABP_Cube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;
}

// Called when the game starts or when spawned
void ABP_Cube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_Cube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

