// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "MPCrow.generated.h"

UCLASS()
class MOBILEPROTOTYPE_API AMPCrow : public APawn
{
	GENERATED_BODY()

protected:
	AMPCrow();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* CrowMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	AActor* BenchmarkPoi = nullptr;

	/** If the crow collides with something, it will not move for the specified time (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crow")
	float WaitingTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crow")
	float MovementSpeed = 100.0f;
	
	/** Rotation speed towards the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crow")
	float RotationSpeed = 80.0f;

private:
	bool bWait = false;
};
