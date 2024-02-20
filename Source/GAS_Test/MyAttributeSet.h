// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEST_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	UMyAttributeSet();

public:	

	// Health Attribute
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FGameplayAttribute HealthAttribute;

	// Speed Attribute
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FGameplayAttribute SpeedAttribute;

protected:

	const FGameplayAttribute GetHealthAttribute() { return HealthAttribute; }
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Health;

	const FGameplayAttribute GetSpeedAttribute() { return SpeedAttribute; }
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Speed;

public:


	float GetHealth() const;
	void SetHealth(float NewVal);

	float GetSpeed() const;
	void SetSpeed(float NewVal);

};
