// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GAS_TestCharacter.h"
#include "GameplayEffectTriggerVolume.generated.h"

UCLASS()
class GAS_TEST_API AGameplayEffectTriggerVolume : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GETriggerVolume", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* CollisionComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GETriggerVolume")
		FGameplayTagContainer IgnoredTags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GETriggerVolume")
		TArray<TSubclassOf<class UGameplayEffect>> EffectClasses;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsActive, Category = "GETriggerVolume")
		bool bIsActive;

	// -----------------------------------------------------
	// ------ Helper Functions for Actor Interactions ------
	// -----------------------------------------------------

	virtual void ApplyEffectTo(AGAS_TestCharacter* Pawn);
	virtual void RemoveEffectFrom(AGAS_TestCharacter* Pawn);

	void OverlappedVolume(AGAS_TestCharacter* Pawn);
	void ExitedVolume(AGAS_TestCharacter* Pawn);

	UFUNCTION()
		virtual void OnRep_IsActive();

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnOverlapped"))
		void K2_OnOverlapped();
	
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnOverlapped"))
		void K2_OnExited();

	
public:	
	// Sets default values for this actor's properties
	AGameplayEffectTriggerVolume();

	// Replicated Objects
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Check if Character should be effected by GE Volume
	virtual bool CanEffectCharacter(AGAS_TestCharacter* TestCharacter) const;

	// Apply Effect to Actor on Begin Overlap
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

	// Remove Effect to Actor on End Overlap
	virtual void NotifyActorEndOverlap(class AActor* Other) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
