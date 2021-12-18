// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (COOP), meta = (BlueprintSpawnableComponent))
class MULTPLAYER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	/**  Ù–‘∏¥÷∆ */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly,Replicated,  BlueprintReadOnly, Category = "HealthComponent")
		int team_numb;
protected:
	virtual void BeginPlay() override;
	
	
public:
	
};
