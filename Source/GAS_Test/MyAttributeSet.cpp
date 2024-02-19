// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeSet.h"

UMyAttributeSet::UMyAttributeSet() : Health(100.0f), Speed(50.0f)
{

}


// ------------------------------------------------------------------------
// -------------- Getters / Setters for Gameplay Attributes ---------------
// ------------------------------------------------------------------------

// Get Health Attribute Data
float UMyAttributeSet::GetHealth() const
{
	return FMath::Max(Health.GetCurrentValue(), 0.0f);
}

// Set health Attribute Data within ASC
void UMyAttributeSet::SetHealth(float NewVal)
{
	// Do not accept values lower than zero.
	NewVal = FMath::Max(NewVal, 0.0f);

	// Reference to Ability System Comp
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (ensure(ASC))
	{
		// Set Base value for Health
		ASC->SetNumericAttributeBase(GetHealthAttribute(), NewVal);
	}
}

// Get Speed Attribute Data
float UMyAttributeSet::GetSpeed() const
{
	return FMath::Max(Speed.GetCurrentValue(), 10.0f);
}

// Set Speed Attribute Data with ASC
void UMyAttributeSet::SetSpeed(float NewVal)
{

	// Do not accept Values lower than 10.f
	NewVal = FMath::Max(NewVal, 10.0f);

	// Reference to Ability System Comp
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (ensure(ASC))
	{
		// Set base Value for Speed
		ASC->SetNumericAttributeBase(GetSpeedAttribute(), NewVal);
	}
}

