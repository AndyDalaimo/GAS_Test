// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasePickup.generated.h"

UCLASS()
class GAS_TEST_API ABasePickup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionComp;
	
public:	
	// Sets default values for this actor's properties
	ABasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
