// Fill out your copyright notice in the Description page of Project Settings.


#include "MPCrow.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework\GameModeBase.h"

AMPCrow::AMPCrow()
{
	PrimaryActorTick.bCanEverTick = true;

	CrowMesh = CreateDefaultSubobject<UStaticMeshComponent>("CrowMesh");
	CrowMesh->SetupAttachment(RootComponent);
}

void AMPCrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(BenchmarkPoi))
	{
		if (!bWait) {
			FVector PoiLocation = BenchmarkPoi->GetActorLocation();
			FVector DiffLocation = PoiLocation - GetActorLocation();
			DiffLocation.Normalize();

			// Set mesh rotation
			FRotator NewRotation = FMath::RInterpConstantTo(CrowMesh->GetComponentRotation(), DiffLocation.Rotation(), DeltaTime, RotationSpeed);
			CrowMesh->SetWorldLocationAndRotationNoPhysics(CrowMesh->GetComponentLocation(), NewRotation);

			// Set actor location
			FHitResult HitResult;
			FVector NewLocation = GetActorLocation() + DiffLocation * (MovementSpeed * DeltaTime);
			SetActorLocation(NewLocation, true, &HitResult);

			if (WaitingTime > 0.0f)
			{
				// If the crow collides with something, it stops moving for a certain amount of time
				if (HitResult.bBlockingHit)
				{
					bWait = true;

					FTimerHandle Timer;
					GetWorld()->GetTimerManager().SetTimer(Timer, [this]()
					{
						bWait = false;
					}, WaitingTime, false);
				}
			}
		}
	}
}
