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
		FVector PoiLocation = BenchmarkPoi->GetActorLocation();
		FVector DiffLocation = PoiLocation - GetActorLocation();
		DiffLocation.Normalize();

		float DistanceBetweenCrowAndPoi = FVector::DistSquared2D(GetActorLocation(), PoiLocation);
		if (DistanceBetweenCrowAndPoi < MovementSwitchingDistanceSquared && GetController())
		{
			
			// Set mesh rotation
			FRotator NewRotation = FMath::RInterpConstantTo(CrowMesh->GetComponentRotation(), DiffLocation.Rotation(), DeltaTime, RotationSpeed);
			CrowMesh->SetWorldLocationAndRotationNoPhysics(CrowMesh->GetComponentLocation(), NewRotation);
			
			
			AddMovementInput(DiffLocation);
			
		}
		else if (!bWait)
		{
			// Set mesh rotation
			FRotator NewRotation = FMath::RInterpConstantTo(CrowMesh->GetComponentRotation(), DiffLocation.Rotation(), DeltaTime, RotationSpeed);
			CrowMesh->SetWorldLocationAndRotationNoPhysics(CrowMesh->GetComponentLocation(), NewRotation);

			FHitResult HitResult;

			// Set actor location
			FVector NewLocation = GetActorLocation() + DiffLocation * (MovementSpeed * DeltaTime);
			SetActorLocation(NewLocation, true, &HitResult);

			if (WaitingTime > 0.0f)
			{
				// If the crow collides with something, it stops moving for a certain amount of time
				if (HitResult.bBlockingHit)
				{
					AActor* HitActor = HitResult.GetActor();
					HitActor->SetActorLocation(HitActor->GetActorLocation() + DiffLocation * (MovementSpeed * 1.5f * DeltaTime));

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
